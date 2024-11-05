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

#include "downsample_fix.hpp"

namespace vart {
namespace cpu {

template <typename DType>
DownSampleFix<DType>::DownSampleFix(const xir::Subgraph* subg,
                                    const xir::Op* op, IMapTBs_t inputs,
                                    CPUTBPtr_t output)
    : DownSample<DType>(subg, op, inputs, output) {
  auto xir_tensor_i = CPUOPBase::xir_op_->get_input_tensor("input");
  auto xir_tensor_o = CPUOPBase::xir_op_->get_output_tensor();

  fp_input_ = xir_tensor_i->get_attr<int>("fix_point");
  fp_output_ = xir_tensor_o->get_attr<int>("fix_point");
  shift_ = fp_output_ - fp_input_;
}

template <typename DType>
void DownSampleFix<DType>::run() {
  // // do necessary check
  // print_param();
  // this->check_param();

  // // read data
  // this->read();

  // calc downsample
  this->downsample();
  downsample_fix();

  // // do save, debug...
  // this->save();
}

template <typename DType>
void DownSampleFix<DType>::print_param() {
  DownSample<DType>::print_param();

  UNI_LOG_DEBUG_INFO << "fix_width = " << fix_width_ << endl;
  UNI_LOG_DEBUG_INFO << "fp_input = " << fp_input_ << endl;
  UNI_LOG_DEBUG_INFO << "fp_output = " << fp_output_ << endl;
  UNI_LOG_DEBUG_INFO << "shift = " << shift_ << endl;
}

template <typename DType>
void DownSampleFix<DType>::downsample_fix() {
  double factor = pow(2, shift_);
  for (auto i = 0; i < fmap_o_.num(); i++) {
    data_out_ptr_[i] =
        round_normal<DType>(CPUOPBase::round_mode_, data_out_ptr_[i] * factor,
                            CPUOPBase::data_min_, CPUOPBase::data_max_);
  }
}

INSTANTIATE_TPCLASS(DownSampleFix);
REG_OP_INSTANCE_FUNC("downsample-fix", DownSampleFix);

}  // namespace cpu
}  // namespace vart
