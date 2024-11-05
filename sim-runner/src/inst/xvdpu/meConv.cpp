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

#include "Conv.hpp"
#include "MEUtil.hpp"

#define SAVE_XVDPU_CTL_PACKET(fname, strName)                                  \
  do {                                                                         \
    if (saveAIEData) {                                                         \
      if (saveAIEDataHex) MEUtil::appendStr2File(fname + ".hex", strName[1]);  \
      if (saveAIEDataTxt) MEUtil::appendStr2File(fname, strName[0]);           \
    }                                                                          \
  } while (0);

double dr(double x) {
  if (x - std::floor(x) >= 0.5)
    return double(std::min(127.0, std::max(-128.0, double(std::ceil(x)))));
  else
    return double(std::min(127.0, std::max(-128.0, double(std::floor(x)))));
}

template <>
void Conv<DPUVersion::XVDPU>::xvdpu_transform() {
  bool log_debug = SimCfg::Instance().get_dump_instr();
  bool saveAIEData = SimCfg::Instance().get_gen_aie_data();
  int aie_data_format = SimCfg::Instance().get_gen_aie_data_format();
  bool saveAIEDataTxt =
      saveAIEData && (aie_data_format == 1 || aie_data_format == 3);
  bool saveAIEDataHex =
      saveAIEData && (aie_data_format == 2 || aie_data_format == 3);
  if (log_debug) {
    cout << "[ME_CONV]: c. output ofm file start..." << endl;
    UNI_LOG_INFO << "[ME_CONV]: c. output ofm file start...";
  }
  for (int i = 0; i < dst_h_; i++) {
    for (int j = 0; j < dst_w_; j++) {
      for (int k = 0; k < oc_; k++) {
        int addr = i * dst_w_ * oc_ + j * oc_ + k;
        double tmp = rlt_s64_[addr];
        if (act_type_ == Calc::RELU_TYPE_LEAKY_RELU) {
          tmp = (tmp < 0) ? (tmp * 26. / 256.) : tmp;
        }
        tmp /= pow(2, (shift_cut_ + 1));
        if (act_type_ == Calc::RELU_TYPE_HSIGMOID) {
          tmp = dr(tmp);
          tmp =
            std::min(
              pow(2, 32),
              std::max(0.0, (tmp * 2731 + 3 * 2731 * pow(2, hsigmoid_in_)))) *
            pow(2, -shift_hsigmoid_);
        } else if (act_type_ == Calc::RELU_TYPE_HSWISH) {
          auto x = dr(tmp);
          auto hsigmoid_x =
            dr(std::min(
                 pow(2, 32),
                 std::max(0.0, (x * 2731 + 3 * 2731 * pow(2, hsigmoid_in_)))) *
               pow(2, -shift_hsigmoid_));
          tmp = x * hsigmoid_x * pow(2, -shift_hswish_);
        }
        xvdpu_sum_dtype_[addr] = Calc::DPURound<DPU_DATA_TYPE>(tmp);
      }
    }
  }

  int OCP = 8;
  int OWP = 4;
  int cut_h = 2, cut_w = tile_owg_ * OWP;
  int oc_iter_ = 1;
  for (int idx_cut_owg = 0; idx_cut_owg < ow_iter_; idx_cut_owg++) {
    for (int idx_cut_ocg = 0; idx_cut_ocg < oc_iter_; idx_cut_ocg++) {
      for (int idx_oh = 0; idx_oh < dst_h_; idx_oh += 2) {
        for (int idx_oce = 0; idx_oce < xvdpu_OCpg_; idx_oce++) {
          vector<DPU_DATA_TYPE> rltSlice(cut_h * cut_w * tile_ocg_ * OCP);
          uint32_t dstaddr = 0;
          for (int idx_owg = 0; idx_owg < tile_owg_; idx_owg++) {
            for (int idx_ocg = 0; idx_ocg < tile_ocg_; idx_ocg++) {
              for (int idx_owp = 0; idx_owp < OWP; idx_owp++) {
                uint32_t addr_a =
                    idx_oh * dst_w_ * oc_ +
                    (idx_cut_owg * tile_owg_ * OWP + idx_owg * OWP + idx_owp) *
                        oc_ +
                    ((xvdpu_OCpg_ * idx_cut_ocg) * tile_ocg_ +
                     xvdpu_OCpg_ * idx_ocg + idx_oce) *
                    OCP;
                UNI_LOG_CHECK(addr_a < xvdpu_sum_dtype_.size(),
                              SIM_OUT_OF_RANGE)
                    << "addr overflow, when tile xvdpu_sum_dtype_";
                std::copy_n(xvdpu_sum_dtype_.data() + addr_a, OCP,
                            rltSlice.data() + dstaddr);
                dstaddr += OCP;
                uint32_t addr_b = addr_a + dst_w_ * oc_;
                UNI_LOG_CHECK(addr_b < xvdpu_sum_dtype_.size(),
                              SIM_OUT_OF_RANGE)
                    << "addr overflow, when tile xvdpu_sum_dtype_";
                std::copy_n(xvdpu_sum_dtype_.data() + addr_b,
                            OCP,
                            rltSlice.data() + dstaddr);
                dstaddr += OCP;
              }
            }
          }
          if (saveAIEData) {
            int row = idx_oh + idx_oce % 2;
            int col = ((idx_oce / 2) * 2 + (idx_oce % 2 ? 0 : 1)) % 2;
            string fname = target_data_dir_ + MECfg::fn_rlt_prefix + "n" +
                           to_string(idx_oce / 2) + "_r" + to_string(row) +
                           "_c" + to_string(col) + ".txt";
            uint32_t* uptr = reinterpret_cast<uint32_t*>(rltSlice.data());
            if (saveAIEDataTxt)
              MEUtil::appendImg2IntFile(fname, uptr, rltSlice.size() / 4,
                                        MECfg::Instance().rlt_port_width());
            if (saveAIEDataHex)
              MEUtil::appendImg2HexFile(fname + ".hex", rltSlice.data(),
                                        rltSlice.size(), 8);
          }
        }
      }
    }
  }
  if (log_debug) {
    cout << "[ME_CONV]: c. output ofm file successfully..." << endl;
    UNI_LOG_INFO << "[ME_CONV]: c. output ofm file successfully...";
  }
}

template <>
void Conv<DPUVersion::XVDPU>::xvdpu_genWLut(int kernel_w, int kernel_h,
                                            int stride_w, int cut_icg, int iw,
                                            int32_t* wlut) {
  // KW->KH->ICG
  int idx = 0;
  const int PP_IC = 32;
  for (int idx_icg = 0; idx_icg < cut_icg; idx_icg++) {
    for (int idx_kh = 0; idx_kh < kernel_h; idx_kh++) {
      for (int idx_kw = 0; idx_kw < kernel_w - 1; idx_kw++) {
        wlut[idx++] = (-3 * stride_w + 1) * PP_IC;
      }
      wlut[idx++] = ((iw - (3 * stride_w + kernel_w - 1)) * PP_IC);
    }
  }
}

template <>
string Conv<DPUVersion::XVDPU>::xvdpu_getWgtfname(int g, int l) {
  string fn_wgt = target_data_dir_ + MECfg::fn_wgt_prefix + "g" + to_string(g) +
                  "_l" + to_string(l) + ".txt";
  return fn_wgt;
}

template <>
int32_t Conv<DPUVersion::XVDPU>::xvdpu_getExecType(unsigned int coreidx,
                                                   int idx_iter, int iterNum,
                                                   int inner_count,
                                                   int out_loop) {
  int32_t exec_type = 0;
  UNI_LOG_CHECK(coreidx >= 0 && coreidx < 4, SIM_OUT_OF_RANGE);
  bool isCoreB = (coreidx == 1 || coreidx == 2);
  bool isInnerGE3 = (inner_count >= 3);
  bool isInnerEQ2 = (inner_count == 2);
  bool isInnerEQ1 = (inner_count == 1);
  bool isOutGE3 = (out_loop >= 3);
  bool isOutGE2 = (out_loop >= 2);
  UNI_LOG_CHECK(iterNum >= 1, SIM_OUT_OF_RANGE);
  UNI_LOG_CHECK(idx_iter < iterNum && idx_iter >= 0, SIM_OUT_OF_RANGE);
  bool isSingleIter = (iterNum == 1);
  bool isMultIterFirst = (iterNum != 1 && idx_iter == 0);
  bool isMultIterLast = (iterNum != 1 && idx_iter == (iterNum - 1));
  bool isMultIterMid =
      (iterNum != 1 && idx_iter > 0 && idx_iter < (iterNum - 1));

  if (isSingleIter) {
    if (isCoreB && isOutGE2 && isInnerEQ1)
      exec_type = MECfg::ET_B_INaCASC2OUT_IL1;
    else if (isCoreB && isOutGE2 && isInnerEQ2)
      exec_type = MECfg::ET_B_INaCASC2OUT_IL2;
    else if (isCoreB && isOutGE2 && isInnerGE3)
      exec_type = MECfg::ET_B_INaCASC2OUT;
    else if (isCoreB && !isOutGE2 && isInnerGE3)
      exec_type = MECfg::ET_B_INaCASC2OUT_OL1;
    else if (!isCoreB && isInnerEQ1)
      exec_type = MECfg::ET_A_IN2CASC_IL1;
    else if (!isCoreB && isInnerEQ2)
      exec_type = MECfg::ET_A_IN2CASC_IL2;
    else if (!isCoreB && isInnerGE3)
      exec_type = MECfg::ET_A_IN2CASC;
    else {
      cout << "could not find exec_type for [coreidx, idx_iter, iterNum, "
              "inner_count, out_loop, act_type] = "
           << endl;
      cout << "\t\t[" << coreidx << ", " << idx_iter << ", " << iterNum << ", "
           << inner_count << ", " << out_loop << ", " << act_type_ << "]"
           << endl;
      UNI_LOG_CHECK(0, SIM_ALIGN_ERROR)
          << "could not find exec_type for [coreidx, idx_iter, iterNum, "
             "inner_count, out_loop, act_type] = "
          << "\t\t[" << coreidx << ", " << idx_iter << ", " << iterNum << ", "
          << inner_count << ", " << out_loop << ", " << act_type_ << "]";
    }
  } else {
    if (inner_count < 3) {
      cout << "[ME_CONV]: xvdpu_getExecType error, ic_iter > 1, require "
              "inner_count(kw * kh * tile_icg) >=3, "
           << inner_count << " provided." << endl;
      UNI_LOG_CHECK(0, SIM_ALIGN_ERROR)
        << "xvdpu_getExecType error, ic_iter > 1, require inner_count(kw * kh "
           "* tile_icg) >=3, "
        << inner_count << " provided.";
    }
    if (isMultIterFirst && isCoreB && isOutGE3)
      exec_type = MECfg::ET_B_IN2DM;
    else if (isMultIterFirst && isCoreB && !isOutGE3)
      exec_type = MECfg::ET_B_IN2DM_OLNP;
    else if (isMultIterMid && isCoreB && isOutGE3)
      exec_type = MECfg::ET_B_INaDM2DM;
    else if (isMultIterMid && isCoreB && !isOutGE3)
      exec_type = MECfg::ET_B_INaDM2DM_OLNP;
    else if (isMultIterLast && isCoreB && isOutGE3)
      exec_type = MECfg::ET_B_INaDMaCASC2OUT;
    else if (isMultIterLast && isCoreB && !isOutGE3)
      exec_type = MECfg::ET_B_INaDMaCASC2OUT;
    else if (isMultIterFirst && !isCoreB && isOutGE3)
      exec_type = MECfg::ET_A_IN2DM;
    else if (isMultIterFirst && !isCoreB && !isOutGE3)
      exec_type = MECfg::ET_A_IN2DM_OLNP;
    else if (isMultIterMid && !isCoreB && isOutGE3)
      exec_type = MECfg::ET_A_INaDM2DM;
    else if (isMultIterMid && !isCoreB && !isOutGE3)
      exec_type = MECfg::ET_A_INaDM2DM_OLNP;
    else if (isMultIterLast && !isCoreB && isOutGE3)
      exec_type = MECfg::ET_A_INaDM2CASC;
    else if (isMultIterLast && !isCoreB && !isOutGE3)
      exec_type = MECfg::ET_A_INaDM2CASC_OLNP;
    else {
      cout << "could not find exec_type for [coreidx, idx_iter, iterNum, "
              "inner_count, out_loop, act_type] = "
           << endl;
      cout << "\t\t[" << coreidx << ", " << idx_iter << ", " << iterNum << ", "
           << inner_count << ", " << out_loop << ", " << act_type_ << "]"
           << endl;
      UNI_LOG_CHECK(0, SIM_ALIGN_ERROR)
        << "could not find exec_type for [coreidx, idx_iter, iterNum, "
           "inner_count, out_loop, act_type] = "
        << "\t\t[" << coreidx << ", " << idx_iter << ", " << iterNum << ", "
        << inner_count << ", " << out_loop << ", " << act_type_ << "]";
    }
  }
  return exec_type;
}

template <>
void Conv<DPUVersion::XVDPU>::xvdpu_trasWgtOri2ME(const int row, const int col,
                                                  const DPU_DATA_TYPE* mat,
                                                  DPU_DATA_TYPE* mat_out) {
  UNI_LOG_CHECK(row == 8 && col == 8, SIM_ALIGN_ERROR);
  int transMap[64] = {0,  8,  16, 24, 32, 40, 48, 56, 1,  9,  17, 25, 33,
                      41, 49, 57, 2,  10, 18, 26, 34, 42, 50, 58, 3,  11,
                      19, 27, 35, 43, 51, 59, 4,  12, 20, 28, 36, 44, 52,
                      60, 5,  13, 21, 29, 37, 45, 53, 61, 6,  14, 22, 30,
                      38, 46, 54, 62, 7,  15, 23, 31, 39, 47, 55, 63};

  for (int idx = 0; idx < 64; idx++) {
    mat_out[transMap[idx]] = mat[idx];
  }
}

template <>
void Conv<DPUVersion::XVDPU>::xvdpu_conv() {
  bool saveAIEData = SimCfg::Instance().get_gen_aie_data();
  int aie_data_format = SimCfg::Instance().get_gen_aie_data_format();
  bool saveAIEDataTxt =
      saveAIEData && (aie_data_format == 1 || aie_data_format == 3);
  bool saveAIEDataHex =
      saveAIEData && (aie_data_format == 2 || aie_data_format == 3);
  // a. deblock
  const int ICP = 8;
  const int OCP = 8;
  const int ICE =
      ICP * 2;  // one MAC function need 2x8 ic element from one pixel
  const int OWP = 4;
  const int OHP = 8;
  // const int OCE = OCP*xvdpu_OCpg_; //64 core need 8 group OCP
  // ONE_CUT is the once fire needed data
  const int ONE_CUT_ICG = tile_icg_;
  const int ONE_CUT_OCG = tile_ocg_;
  const int ONE_CUT_OWG = tile_owg_;
  const int OWG_NUM = ow_iter_;
  const int OCG_NUM = 1;
  const int ICG_NUM = ic_iter_ * 2;
  UNI_LOG_CHECK(dst_w_ % (ONE_CUT_OWG * OWP) == 0, SIM_ALIGN_ERROR);
  UNI_LOG_CHECK(oc_ % (ONE_CUT_OCG * OCP) == 0, SIM_ALIGN_ERROR);
  UNI_LOG_CHECK(ic_ % (ONE_CUT_ICG * ICP) == 0, SIM_ALIGN_ERROR);

  int oneCutIC = ONE_CUT_ICG * ICE;
  int oneCutW = ((ONE_CUT_OWG * OWP - 1) * stride_w_ + kernel_w_);
  int oneCutSize = kernel_h_ * oneCutW * oneCutIC;
  int iterNum = ICG_NUM / 2;  // 2 core
  uint32_t img_byte_size = oneCutSize;
  UNI_LOG_CHECK(img_byte_size % 4 == 0, SIM_ALIGN_ERROR);  // word aligned
  uint32_t img_word_size = img_byte_size / 4;
  uint32_t img_wnd_byte_size =
      img_byte_size * 2;  // one img window has 2 pixel in OH
  uint32_t img_wnd_word_size = img_word_size * 2;
  bool log_debug = SimCfg::Instance().get_dump_instr();
  bool log_tile_info = xvdpu_needParamPkts_ && log_debug;
  if (log_tile_info) {
    cout << "[ME_CONV]: deblock info, "
         << "\n\tsrc size[ih, iw, ic] = " << src_h_ << ", " << src_w_ << ", "
         << ic_ << ", "
         << "\n\tone tiled block size: [tile_owg, tile_ocg, tile_icg] = "
         << ONE_CUT_OWG << ", " << ONE_CUT_OCG << ", " << ONE_CUT_ICG << ", "
         << "\n\ttiled block num: [ow_iter, ic_iter] = " << OWG_NUM << ", "
         << iterNum << endl;
    UNI_LOG_INFO
        << "[ME_CONV]: deblock info, "
        << "\n\tsrc size[ih, iw, ic] = " << src_h_ << ", " << src_w_ << ", "
        << ic_
        << ", "
        // << "\n\tparallel : [OWP, OCP, ICP] = "<< OWP <<", "<<OCP<<", "<<ICE
        // << "\n\tone tiled block size: [ONE_CUT_OWG, ONE_CUT_OCG, ONE_CUT_ICG]
        // =
        // "
        << "\n\tone tiled block size: [tile_owg, tile_ocg, tile_icg] = "
        << ONE_CUT_OWG << ", " << ONE_CUT_OCG << ", " << ONE_CUT_ICG << ", "
        << "\n\ttiled block num: [ow_iter, ic_iter] = " << OWG_NUM << ", "
        << iterNum;
  }

  if (log_debug) {
    cout << "[ME_CONV]: a. img file generating..." << endl;
    UNI_LOG_INFO << "[ME_CONV]: a. img file generating...";
  }

  uint32_t src_size = src_h_ * src_w_ * ic_;
  auto ipw_word = MECfg::Instance().img_port_width();
  for (int idx_owg_cut = 0; idx_owg_cut < OWG_NUM; idx_owg_cut++) {
    int w_offset = (idx_owg_cut * ONE_CUT_OWG * OWP) * stride_w_;
    for (int idx_ocg_cut = 0; idx_ocg_cut < OCG_NUM; idx_ocg_cut++) {
      for (int idx_iter = 0; idx_iter < ICG_NUM; idx_iter += 2) {  // 2 core
        for (int idx_oh = 0; idx_oh < OHP; idx_oh += 2) {
          // LOG(INFO)<<"[ME_CONV]: packing img [idx_owg_cut, idx_ocg_cut,
          // idx_iter, idx_oh] = "; LOG(INFO)<<idx_owg_cut<<", "<<idx_ocg_cut<<",
          // "<<idx_iter<<", "<<idx_oh<<endl;
          vector<DPU_DATA_TYPE> img2CoreA(img_wnd_byte_size);
          vector<DPU_DATA_TYPE> img2CoreB(img_wnd_byte_size);
          int idx_dst = 0;
          for (int idx_icg = 0; idx_icg < ONE_CUT_ICG; idx_icg++) {
            for (int idx_kh = 0; idx_kh < kernel_h_; idx_kh++) {
              for (int idx_w = 0; idx_w < oneCutW; idx_w++) {
                uint32_t baseA = (idx_oh * stride_h_ + idx_kh) * src_w_ * ic_ +
                                 (w_offset + idx_w) * ic_ +
                                 (idx_iter * ONE_CUT_ICG + idx_icg * 2) * ICE;
                uint32_t baseAh = baseA + stride_h_ * src_w_ * ic_;  // stride_h
                UNI_LOG_CHECK(baseA < src_size && baseAh < src_size,
                              SIM_OUT_OF_RANGE);
                uint32_t baseB = baseA + ICP;
                uint32_t baseBh = baseB + stride_h_ * src_w_ * ic_;  // stride_h
                UNI_LOG_CHECK(baseB < src_size && baseBh < src_size,
                              SIM_OUT_OF_RANGE);

                std::copy_n(img_.data() + baseA, ICP,
                            img2CoreA.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseAh, ICP,
                            img2CoreA.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseA + ICE, ICP,
                            img2CoreA.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseAh + ICE, ICP,
                            img2CoreA.data() + idx_dst);
                idx_dst += ICP;
                idx_dst -= 2 * ICE;
                std::copy_n(img_.data() + baseB, ICP,
                            img2CoreB.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseBh, ICP,
                            img2CoreB.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseB + ICE, ICP,
                            img2CoreB.data() + idx_dst);
                idx_dst += ICP;
                std::copy_n(img_.data() + baseBh + ICE, ICP,
                            img2CoreB.data() + idx_dst);
                idx_dst += ICP;
              }
            }
          }
          if (saveAIEData) {
            uint32_t* uptr = reinterpret_cast<uint32_t*>(img2CoreA.data());
            string fn_img = target_data_dir_ + MECfg::fn_img_prefix + "r" +
                            to_string(idx_oh) + "_";
            if (saveAIEDataHex)
              MEUtil::appendImg2HexFile(fn_img + "c0.txt.hex", img2CoreA.data(),
                                        img2CoreA.size(), ipw_word * 4);
            if (saveAIEDataTxt)
              MEUtil::appendImg2IntFile(fn_img + "c0.txt", uptr,
                                        img_wnd_word_size, ipw_word);

            uptr = reinterpret_cast<uint32_t*>(img2CoreB.data());
            if (saveAIEDataHex)
              MEUtil::appendImg2HexFile(fn_img + "c1.txt.hex", img2CoreB.data(),
                                        img2CoreB.size(), ipw_word * 4);
            if (saveAIEDataTxt)
              MEUtil::appendImg2IntFile(fn_img + "c1.txt", uptr,
                                        img_wnd_word_size,
                                        MECfg::Instance().img_port_width());
          }
        }
      }
    }
  }
  if (log_debug) {
    cout << "[ME_CONV]: a. img file generated successfully" << endl;
    UNI_LOG_INFO << "[ME_CONV]: a. img file generated successfully";

    cout << "[ME_CONV]: b. weights file generating..." << endl;
    UNI_LOG_INFO << "[ME_CONV]: b. weights file generating...";
  }
  // b.0 calculate wLut param
  int ofm_count = ONE_CUT_OCG;
  int out_count = ONE_CUT_OWG;  // stride_h
  int inner_count = kernel_w_ * kernel_h_ * ONE_CUT_ICG;
  int out_loop = ofm_count * out_count;
  unsigned int wLut_word_size = inner_count;
  unsigned int wLut_byte_size = wLut_word_size * 4;
  int32_t* wLut = new int32_t[wLut_word_size];
  UNI_LOG_CHECK(wLut_word_size <= MECfg::Instance().MAX_WLUT_WORD_SIZE(),
                SIM_EXCEED_HARDWARE_LIMIT)
      << "error, wLut buffer is not enough!!!";
  std::fill(wLut, wLut + wLut_word_size, 0);
  xvdpu_genWLut(kernel_w_, kernel_h_, stride_w_, ONE_CUT_ICG, oneCutW, wLut);
  if (log_tile_info) {
    string paramString("");
    for (auto i = 0u; i < wLut_word_size; i++) {
      paramString += " " + to_string(wLut[i]);
    }
    // std::for_each(wLut, wLut+wLut_word_size, [](const int32_t &n){ cout<<"
    // "<<n;});
    cout << "[ME_CONV]: b.1 wLut generated, length is " << wLut_word_size
         << ": " << paramString.c_str() << endl;
    UNI_LOG_INFO << "[ME_CONV]: b.1 wLut generated, length is "
                 << wLut_word_size << ": " << paramString.c_str();
  }
  // cout<<endl;

  // b.1 declare iterPram
  unsigned int ITER_PARAM_WORD_SIZE = MECfg::Instance().ITER_PARAM_WORD_SIZE();
  unsigned int* iterPram = new unsigned int[ITER_PARAM_WORD_SIZE];
  std::fill(iterPram, iterPram + ITER_PARAM_WORD_SIZE, 0);
  unsigned int iterparam_word_size = ITER_PARAM_WORD_SIZE;
  unsigned int iterparam_byte_size = iterparam_word_size * 4;

  iterPram[1] = act_type_ == 3 ? shift_cut_ + 8 + 1 : shift_cut_ + 1;
  iterPram[2] = shift_bias_;

  uint32_t bias_byte_size = ofm_count * 16;

  // b.2 calculate layer_param_buf
  unsigned int LAYER_PARAM_WORD_SIZE =
      MECfg::Instance().LAYER_PARAM_WORD_SIZE();
  uint32_t layer_param_word_size = LAYER_PARAM_WORD_SIZE;
  uint32_t layer_param_byte_size = layer_param_word_size * 4;
  int32_t layer_param_buf[LAYER_PARAM_WORD_SIZE];
  uint32_t real_param_word_size = 14;
  uint32_t once_weights_byte_size =
      ONE_CUT_OCG * OCP * kernel_h_ * kernel_w_ * oneCutIC;
  uint64_t rlt_byte_size = 2 * (ONE_CUT_OWG * OWP) * (ONE_CUT_OCG * OCP);
  uint32_t wgt_window_size =
    iterparam_byte_size + bias_byte_size + once_weights_byte_size;
  const int STACK_ADDR = 14336;
  std::fill(layer_param_buf, layer_param_buf + layer_param_word_size, 0);
  layer_param_buf[0] = out_count;                     // out_count
  layer_param_buf[1] = ofm_count;                     // ofm_count
  layer_param_buf[2] = inner_count;                   // inner_count
  int32_t iter_param_offset = img_wnd_byte_size + 4;  // add 32 bits to align
  layer_param_buf[3] = iter_param_offset;
  layer_param_buf[4] =
      iter_param_offset + iterparam_byte_size + bias_byte_size;  // w_buf_offset
  layer_param_buf[5] =
      STACK_ADDR - (iterNum == 1 ? rlt_byte_size : rlt_byte_size * 4);
  layer_param_buf[6] = conv_num_ * iterNum * OWG_NUM * OCG_NUM;  // iter_count
  int32_t Aw_stride = stride_w_ * ICE * 2;
  int32_t Aw_mRCifm = -kernel_h_ * oneCutW * ICE * 2 * ONE_CUT_ICG;
  layer_param_buf[7] = Aw_stride;
  layer_param_buf[8] = Aw_mRCifm;
  layer_param_buf[9] = Aw_mRCifm + 4 * Aw_stride;  // Aw_mRCifm_4stride
  layer_param_buf[10] = act_type_;
  layer_param_buf[11] = hsigmoid_in_;
  layer_param_buf[12] = shift_hsigmoid_;
  layer_param_buf[13] = shift_hswish_;
  if (log_tile_info) {
    string paramString("");
    for (auto i = 0u; i < layer_param_word_size; i++) {
      paramString += " " + to_string(layer_param_buf[i]);
    }
    // std::for_each(layer_param_buf, layer_param_buf+layer_param_word_size,
    // [](const int32_t &n){ cout<<" "<<n;});
    cout << "[ME_CONV]: b.2 layer_param_buf generated: " << paramString.c_str()
         << endl;
    UNI_LOG_INFO << "[ME_CONV]: b.2 layer_param_buf generated: "
                 << paramString.c_str();
  }
  // cout<<endl;

  if (log_tile_info) {
    cout << "[ME_CONV]: b. buffer size sumary: "
         << "\n\titerNum: " << iterNum
         << "\n\tlayer_param_byte_size: " << layer_param_byte_size
         << "\n\twLut_byte_size: " << wLut_byte_size
         << "\n\timg_wnd_byte_size: " << img_wnd_byte_size
         << "\n\trlt_byte_size: " << rlt_byte_size
         << "\n\twgt_wnd_byte_size: " << wgt_window_size
         << "\n\t\t--iterparam_byte_size: " << iterparam_byte_size
         << "\n\t\t--bias_byte_size: " << bias_byte_size
         << "\n\t\t--real weights byte size: " << once_weights_byte_size
         << endl;
    UNI_LOG_INFO << "[ME_CONV]: b. buffer size sumary: "
                 << "\n\titerNum: " << iterNum
                 << "\n\tlayer_param_byte_size: " << layer_param_byte_size
                 << "\n\twLut_byte_size: " << wLut_byte_size
                 << "\n\timg_wnd_byte_size: " << img_wnd_byte_size
                 << "\n\trlt_byte_size: " << rlt_byte_size
                 << "\n\twgt_wnd_byte_size: " << wgt_window_size
                 << "\n\t\t--iterparam_byte_size: " << iterparam_byte_size
                 << "\n\t\t--bias_byte_size: " << bias_byte_size
                 << "\n\t\t--real weights byte size: "
                 << once_weights_byte_size;
  }
  if (iterNum == 1) {
    UNI_LOG_CHECK(img_wnd_byte_size + wgt_window_size < STACK_ADDR,
                  SIM_EXCEED_HARDWARE_LIMIT)
        << "img_wnd_byte_size+wgt_window_size need < 0x3800";
  } else {
    UNI_LOG_CHECK(
        img_wnd_byte_size + wgt_window_size + rlt_byte_size * 4 < STACK_ADDR,
        SIM_EXCEED_HARDWARE_LIMIT)
        << "img_wnd_byte_size + wgt_window_size + interbuf_bytes need < 0x3800";
  }

  // b.3 start shoot
  if (log_debug) {
    cout << "[ME_CONV]: b. start packing packets" << endl;
    UNI_LOG_INFO << "[ME_CONV]: b. start packing packets";
  }
  const int wgt_size = oc_ * kernel_h_ * kernel_w_ * ic_;
  int rltPing = MECfg::Instance().rltPing();
  // int rltPing_B = MECfg::Instance().rltPing_B();
  int useRltB = MECfg::Instance().useRltB();
  int bufPing = MECfg::Instance().bufPing();
  // b.3.0 set layer_param_buf & wLut
  unsigned int PARAM_BASE_ADDR = useRltB ? MECfg::Instance().PARAM_BASE_ADDR_PING() : MECfg::Instance().PARAM_BASE_ADDR_PONG();
  unsigned int WLUT_BASE_ADDR = useRltB?  MECfg::Instance().WLUT_BASE_ADDR_PING() : MECfg::Instance().WLUT_BASE_ADDR_PONG() ;
  if (xvdpu_needParamPkts_) {
    useRltB = useRltB == 1 ? 0 : 1;
    MECfg::Instance().SetuseRltB(useRltB);
    for (int i = 0; i < xvdpu_num_wgt_port_; i++) {
      string fn_wgt = xvdpu_getWgtfname(i / 2, i % 2);
      vector<string> interStr(2);
      MEUtil::setDataMem(PARAM_BASE_ADDR, layer_param_buf, real_param_word_size,
                         interStr);
      SAVE_XVDPU_CTL_PACKET(fn_wgt, interStr);

      MEUtil::setDataMem(WLUT_BASE_ADDR, wLut, wLut_word_size, interStr);
      SAVE_XVDPU_CTL_PACKET(fn_wgt, interStr);
      MEUtil::setBDAddrLen(i % 4, img_wnd_byte_size, wgt_window_size,
                           rlt_byte_size, layer_param_buf[5], interStr,
                           useRltB);

      SAVE_XVDPU_CTL_PACKET(fn_wgt, interStr);
      if (MECfg::Instance().isSetIfmB() == 1) {
        MEUtil::initIfmB(interStr);
        SAVE_XVDPU_CTL_PACKET(fn_wgt, interStr);

        if (i >= xvdpu_num_wgt_port_ - 1) MECfg::Instance().SetIfmB(0);
      }
      if (MECfg::Instance().isSetXYRltB() == 1) {
        // cout <<"--------------------------------------"<<endl;
        MEUtil::initRltB(interStr);
        SAVE_XVDPU_CTL_PACKET(fn_wgt, interStr);
        if (i >= xvdpu_num_wgt_port_ - 1) MECfg::Instance().SetXYRltB(0);
      }
    }
  }
  vector<uint64_t> execTimeList;
  for (int idx_owg_cut = 0; idx_owg_cut < OWG_NUM; idx_owg_cut++) {
    for (int idx_ocg_cut = 0; idx_ocg_cut < OCG_NUM; idx_ocg_cut++) {
      uint64_t cycleThisOfm = 0;
      for (int idx_iter = 0; idx_iter < iterNum; idx_iter++) {
        //     LOG(INFO)<<"[ME_CONV]: packing weights [idx_owg_cut, idx_ocg_cut,
        //     idx_iter] = ";
        //   LOG(INFO)<<idx_owg_cut<<", "<<idx_ocg_cut<<", "<<idx_iter<<endl;

        for (int idx_port = 0; idx_port < xvdpu_num_wgt_port_; idx_port++) {
          int oneNidx = idx_port % 4;
          // d.3.1 BD Ctrl Packet
          vector<string> ctrPktStr(2);
          MEUtil::startInputPkts(bufPing, ctrPktStr, useRltB);
          string fn_wgt = xvdpu_getWgtfname(idx_port / 2, idx_port % 2);
          SAVE_XVDPU_CTL_PACKET(fn_wgt, ctrPktStr);

          // d.3.2 concat iter_param, weights
          vector<char> wgtWndData(wgt_window_size);
          // d.3.2.1 set iterPram
          iterPram[0] = xvdpu_getExecType(oneNidx, idx_iter, iterNum,
                                          inner_count, out_loop);
          memcpy(wgtWndData.data(), iterPram, iterparam_byte_size);
          // d.3.2.2 copy bias
          vector<DPU_DATA_TYPE> bias2CoreA(bias_byte_size);
          std::fill(bias2CoreA.data(), bias2CoreA.data() + bias_byte_size, 0);
          // if(oneNidx ==0 || oneNidx == 3){
          //     memcpy(wgtWndData.data() + iterparam_byte_size,
          //     bias2CoreA.data(), bias_byte_size);
          //  }else{
          int bias_addr = 0;
          for (int idx_ocg = 0; idx_ocg < ONE_CUT_OCG; idx_ocg++) {
            int addrA = ((idx_ocg_cut * xvdpu_OCpg_) * ONE_CUT_OCG +
                         idx_ocg * xvdpu_OCpg_ + idx_port / 2) *
                        OCP;
            UNI_LOG_CHECK(addrA < oc_, SIM_OUT_OF_RANGE);
            std::copy_n(bias_.data() + addrA, OCP,
                        bias2CoreA.data() + bias_addr);
            bias_addr += OCP;
            std::copy_n(bias_.data() + addrA, OCP,
                        bias2CoreA.data() + bias_addr);
            bias_addr += OCP;
          }
          memcpy(wgtWndData.data() + iterparam_byte_size, bias2CoreA.data(),
                 bias_byte_size);
          //}
          if (idx_port == 1) {
            cycleThisOfm +=
              MEUtil::calCycle(out_count, inner_count, iterPram[0]);
          }

          // d.3.2.3 copy weights
          vector<DPU_DATA_TYPE> wgt2CoreA(once_weights_byte_size);
          vector<DPU_DATA_TYPE> wgt2CoreAOri(once_weights_byte_size);
          int idx_dst = 0;
          for (int idx_ocg = 0; idx_ocg < ONE_CUT_OCG; idx_ocg++) {
            for (int idx_icg = 0; idx_icg < ONE_CUT_ICG; idx_icg++) {
              for (int idx_kh = 0; idx_kh < kernel_h_; idx_kh++) {
                for (int idx_kw = 0; idx_kw < kernel_w_; idx_kw++) {
                  for (int idx_ice = 0; idx_ice < 2; idx_ice++) {  // 2 x 8
                    vector<DPU_DATA_TYPE> wgtOri(ICP * OCP);
                    int tmp_addr = 0;
                    for (int idx_ocp = 0; idx_ocp < OCP; idx_ocp++) {
                      int baseA =
                          ((idx_ocg_cut * xvdpu_OCpg_) * ONE_CUT_OCG +
                           idx_ocg * xvdpu_OCpg_ + idx_port / 2) *
                              OCP * kernel_w_ * kernel_h_ * ic_      // OCG
                          + idx_ocp * kernel_w_ * kernel_h_ * ic_    // OC
                          + idx_kh * kernel_w_ * ic_ + idx_kw * ic_  // KH, KW
                          + ((2 * idx_iter) * ONE_CUT_ICG + idx_icg * 2) * ICE +
                          idx_ice * ICE + (idx_port % 2) * ICP;
                      UNI_LOG_CHECK(baseA < wgt_size, SIM_OUT_OF_RANGE);
                      std::copy_n(weights_.data() + baseA, ICP,
                                  wgtOri.data() + tmp_addr);
                      tmp_addr += ICP;
                    }
                    vector<DPU_DATA_TYPE> wgtTrans(ICP * OCP);
                    xvdpu_trasWgtOri2ME(ICP, OCP, wgtOri.data(),
                                        wgtTrans.data());
                    std::copy_n(wgtOri.data(), ICP * OCP,
                                wgt2CoreAOri.data() + idx_dst);
                    std::copy_n(wgtTrans.data(), ICP * OCP,
                                wgt2CoreA.data() + idx_dst);

                    idx_dst += ICP * OCP;
                  }
                }
              }
            }
          }

          if (debug_) {
            // MEUtil::appendImg2IntFile(fn_wgt+".int.ori", wgt2CoreAOri.data(),
            // wgt2CoreAOri.size(), 16);
            // MEUtil::appendImg2IntFile(fn_wgt+".int", wgt2CoreA.data(),
            // wgt2CoreA.size(), 16);
          }
          memcpy(wgtWndData.data() + iterparam_byte_size + bias_byte_size,
                 wgt2CoreA.data(), once_weights_byte_size);
          unsigned int ID_DATA = MECfg::Instance().ID_DATA();
          FillWindowPkt WgtPktData(ID_DATA, 0, 0, wgt_window_size,
                                   wgtWndData.data());
          string wgtStr = WgtPktData.to_string();
          if (saveAIEData) {
            if (saveAIEDataTxt) MEUtil::appendStr2File(fn_wgt, wgtStr);
            if (saveAIEDataHex)
              MEUtil::appendStr2File(fn_wgt + ".hex", WgtPktData.to_hex());
          }
        }
        bufPing = (bufPing) ? 0 : 1;
        MECfg::Instance().SetbufPing(bufPing);
        // start Rlt BD
        if (idx_iter == (iterNum - 1)) {
          vector<string> enableStr(2);
          // int rltping = (useRltB == 0) ? rltPing : rltPing_B;
          int rltping = rltPing;
          MEUtil::startRltPkts(rltping, rlt_byte_size, enableStr, useRltB);
          string enableRlt = enableStr[0];
          string enableRltHex = enableStr[1];
          if (saveAIEData) {
            for (int idxpot = 0; idxpot < xvdpu_num_wgt_port_ / 2; idxpot++) {
              if (saveAIEDataTxt)
                MEUtil::appendStr2File(
                    xvdpu_getWgtfname(idxpot, 1 - idxpot % 2), enableRlt);
              if (saveAIEDataHex)
                MEUtil::appendStr2File(
                    xvdpu_getWgtfname(idxpot, 1 - idxpot % 2) + ".hex",
                    enableRltHex);
              if (idxpot == 1) {
                execTimeList.push_back(cycleThisOfm / 2);
              }
            }
          }
          rltPing = (rltPing) ? 0 : 1;
          MECfg::Instance().SetrltPing(rltPing);
        }
      }  // end iterNum
    }    // end ocg_num
  }      // end OWG_NUM
  if (saveAIEData) {
    MEUtil::appendTlastList(target_data_dir_ + MECfg::fn_tlast, execTimeList);
  }
  if (log_debug) {
    cout << "[ME_CONV]: b. weights file generated successfully" << endl;
    UNI_LOG_INFO << "[ME_CONV]: b. weights file generated successfully";
  }
  delete[] wLut;
  delete[] iterPram;
}

template <>
void Conv<DPUVersion::XVDPU>::xvdpu_pad() {
  // handle ic offset
  int ic_pad = icg_offset_ * icp_ + channel_offset_;
  if (ic_pad != 0) {
    for (int i = 0; i < real_src_h_; i++) {
      for (int j = 0; j < src_w_; j++) {
        size_t addr = i * src_w_ * ic_ + j * ic_ + ic_ - ic_pad;
        UNI_LOG_CHECK(addr <= img_.size(), SIM_OUT_OF_RANGE)
            << addr << " > " << img_.size();

        for (int k = 0; k < ic_pad; k++) {
          img_[addr + k] = 0;
        }
      }
    }
  }
  // handle iw offset
  int iw_start = src_w_ - ow_offset_ * stride_w_ - kernel_w_ + pad_right_ + 1;
  UNI_LOG_CHECK(iw_start > 0, SIM_OUT_OF_RANGE)
    << "iw pad start " << iw_start << " <=0, please check ow_offset_ ";
  for (int i = 0; i < src_h_; i++) {
    for (int j = iw_start; j < src_w_; j++) {
      for (int k = 0; k < ic_; k++) {
        size_t addr = i * src_w_ * ic_ + j * ic_ + k;
        UNI_LOG_CHECK(addr < img_.size(), SIM_OUT_OF_RANGE)
          << addr << " >= " << img_.size();
        img_[addr] = 0;
      }
    }
  }
}

