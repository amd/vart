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

#include "mul.hpp"

namespace vart {
namespace cpu {

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
const vector<string> Mul<DTypeIn0, DTypeIn1, DTypeOut>::ITName = {
    "input",
};

// constructor and deconstructor
template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
Mul<DTypeIn0, DTypeIn1, DTypeOut>::Mul(const xir::Subgraph* subg,
                                       const xir::Op* op, IMapTBs_t inputs,
                                       CPUTBPtr_t output)
    : CPUOPBase(subg, op, inputs, output) {
  // get input tensor's fmap
  auto xir_input_tensors = xir_op_->get_input_tensors(ITName[INPUT]);
  UNI_LOG_CHECK(xir_input_tensors.size() == 2, VART_SIZE_ERROR)
      << ", " << xir_input_tensors.size() << " != 2";

  UNI_LOG_CHECK(xir_input_tensors[0] != nullptr, VART_NULL_PTR);
  raw_fmap_ia_ = xir_input_tensors[0]->get_shape();
  fmap_ia_ = raw_fmap_ia_;

  UNI_LOG_CHECK(xir_input_tensors[1] != nullptr, VART_NULL_PTR);
  raw_fmap_ib_ = xir_input_tensors[1]->get_shape();
  fmap_ib_ = raw_fmap_ib_;

  // get output tensor's fmap
  auto* xir_tensor_o = xir_op_->get_output_tensor();
  UNI_LOG_CHECK(xir_tensor_o != nullptr, VART_NULL_PTR);
  fmap_o_ = xir_tensor_o->get_shape();

  mul_align_dim();

  // print param value, used to debug
  print_param();
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
void Mul<DTypeIn0, DTypeIn1, DTypeOut>::run() {
  for (auto i = 0; i < fmap_o_.num(); i++) {
    auto coord = fmap_o_.pos2coord(i);

    auto coord_a = coord;
    auto coord_b = coord;
    for (auto j = 0; j < fmap_o_.ndims(); j++) {
      if (fmap_ia_[j] == 1) {
        coord_a[j] = 0;
      }

      if (fmap_ib_[j] == 1) {
        coord_b[j] = 0;
      }
    }

    auto pos_a = fmap_ia_.coord2pos(coord_a);
    auto pos_b = fmap_ib_.coord2pos(coord_b);
    data_out_[i] = data_ia_[pos_a] * data_ib_[pos_b];
  }
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
void Mul<DTypeIn0, DTypeIn1, DTypeOut>::print_param() {
  raw_fmap_ia_.print_param("raw_fmap_ia");
  raw_fmap_ib_.print_param("raw_fmap_ib");
  fmap_ia_.print_param("fmap_ia");
  fmap_ib_.print_param("fmap_ib");

  fmap_o_.print_param("fmap_o");
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
void Mul<DTypeIn0, DTypeIn1, DTypeOut>::check_param() {
  UNI_LOG_CHECK(inputs_.size() == 1, VART_SIZE_ERROR)
      << ", Err: inputs size is " << inputs_.size();
  UNI_LOG_CHECK(fmap_ia_.ndims() == fmap_ib_.ndims(), VART_NDIMS_ERROR)
      << ", " << fmap_ia_.ndims() << " != " << fmap_ib_.ndims();
  UNI_LOG_CHECK(fmap_ia_.ndims() == fmap_o_.ndims(), VART_NDIMS_ERROR)
      << ", " << fmap_ia_.ndims() << " != " << fmap_o_.ndims();
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
void Mul<DTypeIn0, DTypeIn1, DTypeOut>::read() {
  // read img data
  auto* img_tba = inputs_.at(ITName[INPUT]).at(0);
  data_ia_ = GET_CPUTB_DType_PTR(DTypeIn0, img_tba);

  auto* img_tbb = inputs_.at(ITName[INPUT]).at(1);
  data_ib_ = GET_CPUTB_DType_PTR(DTypeIn1, img_tbb);

  // handle output buffer
  data_out_ = GET_CPUTB_DType_PTR(DTypeOut, output_);
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
uint64_t Mul<DTypeIn0, DTypeIn1, DTypeOut>::get_workload() {
  return fmap_o_.num();
}

template <typename DTypeIn0, typename DTypeIn1, typename DTypeOut>
void Mul<DTypeIn0, DTypeIn1, DTypeOut>::mul_align_dim() {
  if (fmap_ia_.ndims() > fmap_ib_.ndims()) {
    auto diff_num = fmap_ia_.ndims() - fmap_ib_.ndims();
    for (auto i = 0; i < diff_num; i++) {
      fmap_ib_.insert_dim(0, 1);
    }
  } else if (fmap_ia_.ndims() < fmap_ib_.ndims()) {
    auto diff_num = fmap_ib_.ndims() - fmap_ia_.ndims();
    for (auto i = 0; i < diff_num; i++) {
      fmap_ia_.insert_dim(0, 1);
    }
  }
}

INSTANTIATE_TPCLASS_BINARY(Mul);
REG_BINARY_OP_INSTANCE_FUNC("mul", Mul);

}  // namespace cpu
}  // namespace vart
