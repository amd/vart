/*
 * Copyright (C) 2022 Xilinx, Inc.
 * Copyright (C) 2023 – 2024 Advanced Micro Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "conv_base.hpp"
#include "align_buf_mgr.hpp"
#include "conv_2_gemm.hpp"
#include "cpu_gemm.hpp"
#include "fast_pad.hpp"

namespace vart {
namespace cpu {

template <typename DType, typename WType>
const vector<string> ConvBase<DType, WType>::ITName = {
    "input",
    "weights",
    "bias",
};

// constructor and deconstructor
template <typename DType, typename WType>
ConvBase<DType, WType>::ConvBase(const xir::Subgraph* subg, const xir::Op* op,
                          IMapTBs_t inputs, CPUTBPtr_t output)
    : CPUOPBase(subg, op, inputs, output) {
  // get i/o fmap
  GET_INPUT_DIM4_FMAP(raw_fmap_i_, input);
  fmap_i_ = raw_fmap_i_;

  GET_INPUT_DIM4_FMAP(raw_fmap_w_, weights);
  fmap_w_ = raw_fmap_w_;

  has_bias_ = xir_op_->get_input_num(ITName[BIAS]) > 0 ? true : false;
  group_ = xir_op_->has_attr("group") ? xir_op_->get_attr<int32_t>("group") : 1;
  has_shift_psum_ = xir_op_->has_attr("shift_psum");
  shift_psum_ = xir_op_->has_attr("shift_psum")
                    ? xir_op_->get_attr<int32_t>("shift_psum")
                    : 0;
  has_ic_iter_ = xir_op_->has_attr("ic_iter");
  ic_iter_ = has_ic_iter_ ? xir_op_->get_attr<uint32_t>("ic_iter") : 1;
  is_first_layer_ =
      xir_op_->has_attr("layer1") ? xir_op_->get_attr<bool>("layer1") : false;
  if (is_first_layer_) {
    enable_conv_dirty_ = false;
  } else if (has_shift_psum_ && has_ic_iter_) {
    enable_conv_dirty_ = true;
  }
  is_autodpu_stride_conversion_ =
      xir_op_->has_attr("is_autodpu_stride_conversion")
          ? xir_op_->get_attr<bool>("is_autodpu_stride_conversion")
          : false;
  if (has_bias_) {
    GET_INPUT_DIM1_FMAP(fmap_b_, bias);
  }

  GET_OUTPUT_DIM4_FMAP(fmap_o_);

  // NOTE+TODO: in fix mode, xy force it to be 0, not know why now
  std::string xir_pad_mode = "FLOOR";
  if (std::is_floating_point<DType>::value) {
    xir_pad_mode = CPUOPBase::xir_op_->get_attr<std::string>("pad_mode");
  }
  if (CPUOPBase::xir_op_->has_attr("pad")) {
    auto xir_pad = CPUOPBase::xir_op_->get_attr<vector<int>>("pad");
    pad_ = Pad_t{xir_pad_mode, xir_pad[0], xir_pad[2], xir_pad[1], xir_pad[3]};
  } else {
    pad_ = Pad_t{xir_pad_mode, 0, 0, 0, 0};
  }

  THREAD_NUM = CPU_NUM;
  THREAD_WORKLOAD = ceil((float)fmap_o_.num() / THREAD_NUM);
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::print_param() {
  if (!CPUCfg::Instance().debug()) return;

  raw_fmap_i_.print_param("raw_fmap_i");
  fmap_i_.print_param("fmap_i");
  raw_fmap_w_.print_param("raw_fmap_w");
  fmap_w_.print_param("fmap_w");
  UNI_LOG_DEBUG_INFO << "has_bias: " << Bool2Str(has_bias_) << endl;
  fmap_b_.print_param("fmap_b");
  fmap_o_.print_param("fmap_o");

  pad_.print_param("pad");
  kernel_.print_param("kernel");
  stride_.print_param("stride");
  dilation_.print_param("dilation");
  if (group_ != 1) {
    UNI_LOG_DEBUG_INFO << "group: " << group_ << endl;
  }
  if (has_shift_psum_) {
    UNI_LOG_DEBUG_INFO << "shift psum: " << shift_psum_ << endl;
  }
  if (has_ic_iter_) {
    UNI_LOG_DEBUG_INFO << "ic iter: " << ic_iter_ << endl;
  }

  if (CPU_RUN_MODE != CPURunMode::NORMAL) {
    UNI_LOG_DEBUG_INFO << "THREAD_NUM = " << THREAD_NUM << endl;
    UNI_LOG_DEBUG_INFO << "THREAD_WORKLOAD = " << THREAD_WORKLOAD << endl;
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::check_param() {
  UNI_LOG_CHECK(inputs_.size() >= 2 && inputs_.size() <= 3, VART_SIZE_ERROR)
      << ", Err: inputs size is " << inputs_.size();

  // check fmap_out height
  int dst_h = 0;
  int dst_w = 0;
  if (pad_.mode == PadAttr::PAD_MODE_FLOOR) {
    dst_h = floor(1.0 * (fmap_i_.h - kernel_.h + stride_.h) / stride_.h);
    dst_w = floor(1.0 * (fmap_i_.w - kernel_.w + stride_.w) / stride_.w);
  } else if (pad_.mode == PadAttr::PAD_MODE_CEIL) {
    dst_h = ceil(1.0 * (fmap_i_.h - kernel_.h + stride_.h) / stride_.h);
    dst_w = ceil(1.0 * (fmap_i_.w - kernel_.w + stride_.w) / stride_.w);
  } else if (pad_.mode == PadAttr::PAD_MODE_SAME) {
    dst_h = ceil(1.0 * raw_fmap_i_.h / stride_.h);
    dst_w = ceil(1.0 * raw_fmap_i_.w / stride_.w);
  } else if (pad_.mode == PadAttr::PAD_MODE_VALID) {
    dst_h = ceil(1.0 * (fmap_i_.h - kernel_.h + 1.0) / stride_.h);
    dst_w = ceil(1.0 * (fmap_i_.w - kernel_.w + 1.0) / stride_.w);
  } else {
    UNI_LOG_ERROR(VART_NOT_SUPPORT)
        << "Not support pad mode " << pad_.mode << endl;
    abort();
  }

  UNI_LOG_CHECK(dst_h == fmap_o_.h, VART_INVALID_VALUE)
      << ", " << dst_h << " != " << fmap_o_.h;
  UNI_LOG_CHECK(dst_w == fmap_o_.w, VART_INVALID_VALUE)
      << ", " << dst_w << " != " << fmap_o_.w;

  // check weights/bias fmap
  UNI_LOG_CHECK(fmap_w_.n == fmap_o_.c, VART_INVALID_VALUE)
      << ", " << fmap_w_.n << " != " << fmap_o_.c;
  UNI_LOG_CHECK(fmap_w_.h == kernel_.h, VART_INVALID_VALUE)
      << ", " << fmap_w_.h << " != " << kernel_.h;
  UNI_LOG_CHECK(fmap_w_.w == kernel_.w, VART_INVALID_VALUE)
      << ", " << fmap_w_.w << " != " << kernel_.w;
  UNI_LOG_CHECK(fmap_w_.c == fmap_i_.c, VART_INVALID_VALUE)
      << ", " << fmap_w_.c << " != " << fmap_i_.c;

  if (has_bias_) {
    UNI_LOG_CHECK(fmap_b_.c == fmap_o_.c, VART_INVALID_VALUE)
        << ", " << fmap_b_.c << " != " << fmap_o_.c;
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::read() {
  read_input();
  read_weights();
  read_bias();

  data_out_ptr_ = GET_CPUTB_DType_PTR(DType, output_);

  std::fill_n(data_out_ptr_, fmap_o_.num(), DType(0));
}

template <typename DType, typename WType>
uint64_t ConvBase<DType, WType>::get_workload() {
  // not consider batch
  ops_ = 2 * fmap_i_.c * kernel_.h * kernel_.w * fmap_o_.c;
  ops_ *= fmap_o_.h * fmap_o_.w;

  return ops_;
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::read_input() {
  // read img data
  auto* data_in_tb = inputs_.at(ITName[INPUT]).at(0);
  auto* tb_input_ptr = GET_CPUTB_DType_PTR(DType, data_in_tb);

  data_in_ptr_ = reinterpret_cast<DType*>(AlignBufMgr::Instance()->allocate(
      fmap_i_.num() * sizeof(DType), ALIGN_SIZE));

  FastPad(raw_fmap_i_, fmap_i_, pad_, dilation_)
      .transform<DType>(tb_input_ptr, data_in_ptr_);
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::read_weights() {
  // weights order after read: oc, h, w, c
  auto* weights_tb = inputs_.at(ITName[WEIGHTS]).at(0);
  auto* tb_weights_ptr = GET_CPUTB_DType_PTR(WType, weights_tb);
  if (kernel_.dilation_h == 1 && kernel_.dilation_w == 1) {
    weights_ptr_ = tb_weights_ptr;
    return;
  }

  weights_ptr_ = reinterpret_cast<WType*>(AlignBufMgr::Instance()->allocate(
      fmap_w_.num() * sizeof(WType), ALIGN_SIZE));

  FastPad(raw_fmap_w_, fmap_w_, Pad_t{0, 0, 0, 0},
          Dilation_t{kernel_.dilation_h, kernel_.dilation_w})
      .transform<WType>(tb_weights_ptr, weights_ptr_);
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::read_bias() {
  if (has_bias_) {
    auto* bias_tb = inputs_.at(ITName[BIAS]).at(0);
    auto* tb_bias_ptr = GET_CPUTB_DType_PTR(WType, bias_tb);
    bias_ptr_ = tb_bias_ptr;
  } else {
    bias_ptr_ = nullptr;
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv() {
  if (!enable_conv_dirty_) {
    if (CPU_RUN_MODE == CPURunMode::NORMAL) {
      conv_normal();
    } else if (CPU_RUN_MODE == CPURunMode::NORMAL_THREAD) {
      conv_normal_thread();
    } else if (CPU_RUN_MODE == CPURunMode::GEMM) {
      conv_gemm();
    } else if (CPU_RUN_MODE == CPURunMode::GEMM_THREAD) {
      conv_gemm_thread();
    } else {
      conv_normal();
    }
  } else {
    conv_dirty();
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_dirty() {
  std::vector<DType> cascade0(fmap_o_.num(), 0);
  std::vector<DType> cascade1(fmap_o_.num(), 0);
  std::vector<DType> tmp_psum(fmap_o_.num(), 0);
  int32_t ifm_sv_size = fmap_i_.c / 2 / ic_iter_; // I don't know how to calculate when ic <= 64
  // int32_t casc0_ic_end = std::ceil(fmap_i_.c / 2.0 / ifm_sv_size) * ifm_sv_size;
  UNI_LOG_CHECK(fmap_i_.c % (2 * ic_iter_) == 0, VART_SIZE_ERROR) << "fmap_i_.c is " << fmap_i_.c << " can't be divided by " << 2 * ic_iter_ << ".";
  UNI_LOG_CHECK(group_ == 1, VART_SIZE_ERROR) << "dirty work doesn't support group > 1";
  std::vector<DType> data_in_vec(data_in_ptr_, data_in_ptr_ + fmap_i_.num());
  std::vector<WType> weights_vec(weights_ptr_, weights_ptr_ + fmap_w_.num());
  if (is_autodpu_stride_conversion_) {
    const int32_t stride_w = 3;
    UNI_LOG_CHECK(fmap_w_.w == 1, VART_SIZE_ERROR)
        << "dirty work is_auto_dpu_stride_conversion requires kw == 1 after "
           "conversion";
    UNI_LOG_CHECK(fmap_w_.c % (stride_w * 2) == 0, VART_SIZE_ERROR)
        << "dirty work is_auto_dpu_stride_conversion requires ci % 6 == 0";
    std::vector<DType> shuffle_data(fmap_i_.num());
    std::vector<WType> shuffle_wgt(fmap_w_.num());
    std::vector<std::int32_t> wgt_order = {0, 1, 3, 2, 4};
    std::vector<std::int32_t> data_order = {0, 1, 2, 4, 3, 5};
    std::vector<std::int32_t> shuffle_wgt_shape = {(int)fmap_w_.n, (int)fmap_w_.h,
                                                   (int)fmap_w_.w * stride_w, 2,
                                                   (int)fmap_w_.c / stride_w / 2};
    std::vector<std::int32_t> shuffle_data_shape = {
        (int)fmap_i_.n, (int)fmap_i_.h, (int)fmap_i_.w, stride_w, 2, (int)fmap_i_.c / stride_w / 2};
    shuffle_data = cpu_tensor_utils::transpose_data(
        data_in_vec, shuffle_data_shape, data_order);
    shuffle_wgt =
        cpu_tensor_utils::transpose_data(weights_vec, shuffle_wgt_shape, wgt_order);
    data_in_vec = shuffle_data;
    weights_vec = shuffle_wgt;
  } // is_autodpu_stride_conversion_

  for (auto ic_i = 0; ic_i < ic_iter_; ic_i++) {
    int32_t ic_begin = ifm_sv_size * ic_i;
    int32_t ic_end = ifm_sv_size * (ic_i + 1);
    for (auto n = 0; n < fmap_o_.n; n++) {
      for (auto h = 0; h < fmap_o_.h; h++) {
        for (auto w = 0; w < fmap_o_.w; w++) {
          for (auto c = 0; c < fmap_o_.c; c++) {
            conv_one(data_in_vec.data(), weights_vec.data(), tmp_psum.data(), n, h, w, c, ic_begin,
                     ic_end);
          }
        }
      }
    }
    if (has_bias_ && ic_i == 0) {
      auto* xir_tensor_i = CPUOPBase::xir_op_->get_input_tensor("input");
      int32_t fp_input = xir_tensor_i->get_attr<int>("fix_point");

      auto* xir_tensor_w = CPUOPBase::xir_op_->get_input_tensor("weights");
      int32_t fp_weights = xir_tensor_w->get_attr<int>("fix_point");

      auto* xir_tensor_b = CPUOPBase::xir_op_->get_input_tensor("bias");
      int32_t fp_bias = xir_tensor_b->get_attr<int>("fix_point");
      int32_t shift_bias = fp_input + fp_weights - fp_bias;

      for (auto n = 0; n < fmap_o_.n; n++) {
        for (auto h = 0; h < fmap_o_.h; h++) {
          for (auto w = 0; w < fmap_o_.w; w++) {
            for (auto c = 0; c < fmap_o_.c; c++) {
              tmp_psum[n * fmap_o_.ncod() + h * fmap_o_.hcod() +
                       w * fmap_o_.wcod() + c] += bias_ptr_[c] * std::pow(2, shift_bias);
            }
          }
        }
      }
    }
    for (auto i = 0; i < fmap_o_.num(); i++) {  
      cascade0[i] += tmp_psum[i];
      if (CPUOPBase::xir_op_->get_output_tensor()->get_data_type().bit_width == 8 && ic_i == ic_iter_ - 1) {
      } else {
        cascade0[i] = STDRound(cascade0[i] / std::pow(2.0, shift_psum_),
                              -32768, 32767) *
                      std::pow(2.0, shift_psum_);
      }
    }
    std::fill(tmp_psum.begin(), tmp_psum.end(), 0);

    for (auto n = 0; n < fmap_o_.n; n++) {
      for (auto h = 0; h < fmap_o_.h; h++) {
        for (auto w = 0; w < fmap_o_.w; w++) {
          for (auto c = 0; c < fmap_o_.c; c++) {
            conv_one(data_in_vec.data(), weights_vec.data(), tmp_psum.data(), n,
                     h, w, c, ic_begin + fmap_i_.c / 2, ic_end + fmap_i_.c / 2);
          }
        }
      }
    }
    for (auto i = 0; i < fmap_o_.num(); i++) {  
      cascade1[i] += tmp_psum[i];
      if (CPUOPBase::xir_op_->get_output_tensor()->get_data_type().bit_width == 8 && ic_i == ic_iter_ - 1) {
      } else {
        cascade1[i] = STDRound(cascade1[i] / std::pow(2.0, shift_psum_),
                              -32768, 32767) *
                      std::pow(2.0, shift_psum_);
      }
    }
    std::fill(tmp_psum.begin(), tmp_psum.end(), 0);
  }  
  // fp_casc0 = fopen("casc0_bias.bin", "wb");
  // for (auto i = 0; i < fmap_o_.num(); i++) {
  //   fwrite(cascade0.data() + i, sizeof(DType), 1, fp_casc0);
  // }
  // fclose(fp_casc0);
  // FILE* fp_casc1 = fopen("casc1.bin", "wb");
  // for (auto i = 0; i < fmap_o_.num(); i++) {
  //   fwrite(cascade1.data() + i, sizeof(DType), 1, fp_casc1);
  // }
  // fclose(fp_casc1);
  for (auto i = 0; i < fmap_o_.num(); i++) {
    data_out_ptr_[i] = cascade0[i] + cascade1[i];
  }
  // FILE* fp_casc0_1 = fopen("casc0_1.bin", "wb");
  // for (auto i = 0; i < fmap_o_.num(); i++) {
  //   fwrite(data_out_ptr_ + i, sizeof(DType), 1, fp_casc0_1);
  // }
  // fclose(fp_casc0_1);
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_normal() {
  for (auto n = 0; n < fmap_o_.n; n++) {
    for (auto h = 0; h < fmap_o_.h; h++) {
      for (auto w = 0; w < fmap_o_.w; w++) {
        for (auto c = 0; c < fmap_o_.c; c++) {
          if (group_ == 1) {
            conv_one(data_in_ptr_, weights_ptr_, data_out_ptr_, n, h, w, c, 0, fmap_i_.c);
          } else {
            conv_one_withgroup(n, h, w, c);
          }
        }
      }
    }
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_normal_thread() {
  vector<std::future<int>> thr_fut(THREAD_NUM);

  for (auto i = 0U; i < THREAD_NUM; i++) {
    // thr_fut[i] = std::async(std::launch::async | std::launch::deferred,
    thr_fut[i] =
        std::async(std::launch::async,
                   [this](decltype(i) i) {
                     auto BASE_POS = i * THREAD_WORKLOAD;
                     auto FMAP_SIZE = fmap_o_.num();
                     for (auto j = 0U; j < THREAD_WORKLOAD; j++) {
                       int pos = BASE_POS + j;
                       if (pos >= FMAP_SIZE) break;
                       auto fmap = fmap_o_.pos2coord(pos);
                       if (group_ == 1) {
                         conv_one(data_in_ptr_, weights_ptr_, data_out_ptr_,
                                  fmap.n, fmap.h, fmap.w, fmap.c, 0, fmap_i_.c);
                       } else {
                         conv_one_withgroup(fmap.n, fmap.h, fmap.w, fmap.c);
                       }
                     }

                     return 0;
                   },
                   i);
  }

  for (auto i = 0U; i < THREAD_NUM; i++) {
    thr_fut[i].wait();
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_gemm() {
  auto WEIGHTS_BATCH_SIZE = fmap_w_.h * fmap_w_.w * fmap_w_.c;

  FMap_t tmp_fmap{group_ * fmap_o_.n, fmap_o_.h, fmap_o_.w, WEIGHTS_BATCH_SIZE};
  DType* tmp_ptr = reinterpret_cast<DType*>(AlignBufMgr::Instance()->allocate(
      tmp_fmap.num() * sizeof(DType), ALIGN_SIZE));

  Conv2Gemm(fmap_i_, tmp_fmap, fmap_w_, kernel_, stride_)
      .transform(data_in_ptr_, tmp_ptr);
  if (group_ > 1) {  // group convolution
    // allocate an extra space for storing tmp output
    DType* tmp_out_ptr =
        reinterpret_cast<DType*>(AlignBufMgr::Instance()->allocate(
            fmap_o_.num() * sizeof(DType), ALIGN_SIZE));
    // do batch matmul, output layout is (g, n, ho, wo, co / g)
    for (int group_iter = 0; group_iter < group_; group_iter++) {
      DType* tmp_ptr_addr = tmp_ptr + group_iter * fmap_o_.n * tmp_fmap.ncod();
      WType* tmp_weights_addr =
          weights_ptr_ + group_iter * (fmap_w_.num() / group_);
      DType* tmp_out_addr = tmp_out_ptr + group_iter * (fmap_o_.num() / group_);
      matmul<DType, WType>(tmp_ptr_addr, tmp_weights_addr, tmp_out_addr,
                    fmap_o_.n * fmap_o_.h * fmap_o_.w, fmap_o_.c / group_,
                    WEIGHTS_BATCH_SIZE);
    }
    // permute_back to (n, ho, wo, g, co / g)
    for (int i = 0; i < fmap_o_.num(); i++) {
      auto fmap = fmap_o_.pos2coord(i);
      int32_t group_iter = fmap.c / (fmap_o_.c / group_);
      int32_t co_iter = fmap.c % (fmap_o_.c / group_);
      int32_t src_offset = group_iter * fmap_o_.num() / group_ +
                           fmap.n * fmap_o_.ncod() / group_ +
                           fmap.h * fmap_o_.hcod() / group_ +
                           fmap.w * fmap_o_.wcod() / group_ + co_iter;
      data_out_ptr_[i] = tmp_out_ptr[src_offset];
    }
    AlignBufMgr::Instance()->release(tmp_out_ptr);
  } else {  // normal convolution
    matmul<DType, WType>(tmp_ptr, weights_ptr_, data_out_ptr_,
                  fmap_o_.n * fmap_o_.h * fmap_o_.w, fmap_o_.c,
                  WEIGHTS_BATCH_SIZE);
  }
  AlignBufMgr::Instance()->release(tmp_ptr);
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_gemm_thread() {
#if 0
  auto WEIGHTS_BATCH_SIZE = fmap_w_.h * fmap_w_.w * fmap_w_.c;

  FMap_t tmp_fmap{fmap_o_.n, fmap_o_.h, fmap_o_.w, WEIGHTS_BATCH_SIZE};
  DType* tmp_ptr = reinterpret_cast<DType*>(AlignBufMgr::Instance()->allocate(
      tmp_fmap.num() * sizeof(DType), ALIGN_SIZE));

  Conv2Gemm(fmap_i_, tmp_fmap, fmap_w_, kernel_, stride_)
      .transform(data_in_ptr_, tmp_ptr);

  matmul_thread<DType, WType>(tmp_ptr, weights_ptr_, data_out_ptr_,
                       fmap_o_.n * fmap_o_.h * fmap_o_.w, fmap_o_.c,
                       WEIGHTS_BATCH_SIZE);

  AlignBufMgr::Instance()->release(tmp_ptr);
#endif

#if 1
  vector<std::future<int>> thr_fut(THREAD_NUM);

  for (auto i = 0U; i < THREAD_NUM; i++) {
    // thr_fut[i] = std::async(std::launch::async | std::launch::deferred,
    thr_fut[i] = std::async(
        std::launch::async,
        [this](decltype(i) i) {
          auto BASE_POS = i * THREAD_WORKLOAD;
          auto FMAP_SIZE = fmap_o_.num();
          for (auto j = 0U; j < THREAD_WORKLOAD; j++) {
            int pos = BASE_POS + j;
            if (pos >= FMAP_SIZE) break;

            auto fmap = fmap_o_.pos2coord(pos);

            auto* tmp_ptr_i = data_in_ptr_ + fmap.n * fmap_i_.ncod() +
                              fmap.h * stride_.h * fmap_i_.hcod() +
                              fmap.w * stride_.w * fmap_i_.wcod();
            auto* tmp_ptr_w = weights_ptr_ + fmap.c * fmap_w_.ncod();
            if (group_ == 1) {
              inner_product_with_kernel_stride<DType, WType>(
                  tmp_ptr_i, tmp_ptr_w, &data_out_ptr_[pos], fmap_i_, fmap_w_,
                  kernel_);
            } else {
              int32_t idx_oc_group = floor(fmap.c * group_ / fmap_o_.c);
              inner_product_with_kernel_stride<DType, WType>(
                  tmp_ptr_i + idx_oc_group * fmap_w_.c, tmp_ptr_w,
                  &data_out_ptr_[pos], fmap_i_, fmap_w_, kernel_);
            }
          }
          return 0;
        },
        i);
  }

  for (auto i = 0U; i < THREAD_NUM; i++) {
    thr_fut[i].wait();
  }
#endif
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_one(DType* src, WType* wts, DType* dst,
                                      int idx_dst_n, int idx_dst_h,
                                      int idx_dst_w, int idx_oc,
                                      int32_t ic_begin, int32_t ic_end) {
  // assign conv value to data_out_ptr_ var
  auto rlt_addr = idx_dst_n * fmap_o_.ncod() + idx_dst_h * fmap_o_.hcod() +
                  idx_dst_w * fmap_o_.wcod() + idx_oc;

  // NOTE: special handling for conv-fix
  dst[rlt_addr] = 0;

  auto idx_src_h = idx_dst_h * stride_.h;
  auto idx_src_w = idx_dst_w * stride_.w;

  auto addr_base_i = idx_dst_n * fmap_i_.ncod() + idx_src_h * fmap_i_.hcod() +
                     idx_src_w * fmap_i_.wcod();
  auto addr_base_w = idx_oc * fmap_w_.ncod();

  // calculate conv value
  for (auto h = 0; h < kernel_.h; ++h) {
    for (auto w = 0; w < kernel_.w; ++w) {
      auto* tmp_ptr_i =
          src + addr_base_i + h * fmap_i_.hcod() + w * fmap_i_.wcod();
      auto* tmp_ptr_w =
          wts + addr_base_w + h * fmap_w_.hcod() + w * fmap_w_.wcod();
      for (auto c = ic_begin; c < ic_end; ++c) {
        dst[rlt_addr] += tmp_ptr_i[c] * tmp_ptr_w[c];
      }
    }
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::conv_one_withgroup(int idx_dst_n, int idx_dst_h,
                                         int idx_dst_w, int idx_oc) {
  // assign conv value to data_out_ptr_ var
  auto rlt_addr = idx_dst_n * fmap_o_.ncod() + idx_dst_h * fmap_o_.hcod() +
                  idx_dst_w * fmap_o_.wcod() + idx_oc;

  // NOTE: special handling for conv-fix
  data_out_ptr_[rlt_addr] = 0;

  auto idx_src_h = idx_dst_h * stride_.h;
  auto idx_src_w = idx_dst_w * stride_.w;

  auto addr_base_i = idx_dst_n * fmap_i_.ncod() + idx_src_h * fmap_i_.hcod() +
                     idx_src_w * fmap_i_.wcod();
  int32_t group_iter = idx_oc / (fmap_o_.c / group_);
  int32_t co_iter = idx_oc % (fmap_o_.c / group_);
  int32_t ic_by_group = fmap_i_.c / group_;
  auto addr_base_w =
      group_iter * (fmap_w_.num() / group_) + co_iter * fmap_w_.ncod();

  // calculate conv value by group
  for (auto h = 0; h < kernel_.h; ++h) {
    for (auto w = 0; w < kernel_.w; ++w) {
      auto* tmp_ptr_i = data_in_ptr_ + addr_base_i + h * fmap_i_.hcod() +
                        w * fmap_i_.wcod() + group_iter * ic_by_group;
      auto* tmp_ptr_w =
          weights_ptr_ + addr_base_w + h * fmap_w_.hcod() + w * fmap_w_.wcod();
      for (auto c = 0; c < fmap_w_.c; ++c) {
        data_out_ptr_[rlt_addr] += tmp_ptr_i[c] * tmp_ptr_w[c];
      }
    }
  }
}

template <typename DType, typename WType>
void ConvBase<DType, WType>::bias() {
  if (!has_bias_) return;
  if (enable_conv_dirty_) return;

  // NOTE: special handling for conv-fix
  if (std::is_floating_point<DType>::value) {
    for (auto i = 0; i < fmap_o_.num(); i++) {
      auto pos = i % fmap_o_.c;
      data_out_ptr_[i] += bias_ptr_[pos];
    }
  } else {
    UNI_LOG_FATAL(VART_EXEC_ERROR)
        << "Pls do not use conv_base's bias while do conv-fix!" << endl;
  }
}

INSTANTIATE_TPCLASS_SPECIFIED_WEIGHTS(ConvBase);

}  // namespace cpu
}  // namespace vart
