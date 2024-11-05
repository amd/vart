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

#include <cmath>
#include "unary_tanh_fix.hpp"
namespace vart {
namespace cpu {

template <typename DType>
UnaryTanhFix<DType>::UnaryTanhFix(const xir::Subgraph* subg, const xir::Op* op,
                              IMapTBs_t inputs, CPUTBPtr_t output)
    : Eltwise<DType>(subg, op, inputs, output) {
  auto* xir_tensor_i = CPUOPBase::xir_op_->get_input_tensor("input");
  UNI_LOG_CHECK(xir_tensor_i != nullptr, VART_NULL_PTR);
  fp_input_ = xir_tensor_i->get_attr<int>("fix_point");

  auto* xir_tensor_o = CPUOPBase::xir_op_->get_output_tensor();
  UNI_LOG_CHECK(xir_tensor_o != nullptr, VART_NULL_PTR);
  fp_output_ = xir_tensor_o->get_attr<int>("fix_point");

  shift_read_ = fp_input_;
  shift_write_ = -fp_output_;
}

template <typename DType>
void UnaryTanhFix<DType>::print_param() {
  fmap_i_[0].print_param("fmap_i");
  fmap_o_.print_param("fmap_o");
  if (CPU_RUN_MODE == CPURunMode::NORMAL_THREAD ||
      CPU_RUN_MODE == CPURunMode::GEMM_THREAD) {
    UNI_LOG_DEBUG_INFO << "THREAD_NUM = " << THREAD_NUM << endl;
    UNI_LOG_DEBUG_INFO << "THREAD_WORKLOAD = " << THREAD_WORKLOAD << endl;
  }
  UNI_LOG_DEBUG_INFO << "fp_input = " << fp_input_ << endl;
  UNI_LOG_DEBUG_INFO << "fp_output = " << fp_output_ << endl;
  UNI_LOG_DEBUG_INFO << "shift_read = " << shift_read_ << endl;
  UNI_LOG_DEBUG_INFO << "shift_write = " << shift_write_ << endl;
}

template <typename DType>
void UnaryTanhFix<DType>::eltwise(std::uint32_t start_index,
                                 std::uint32_t end_index) {
  for (auto pos_iter = start_index; pos_iter < end_index; pos_iter++) {
    float tmp = (float)data_in_[0][pos_iter] / (float)pow(2.0f, shift_read_);
    tmp = tanh(tmp) / (float)pow(2.0f, shift_write_);
    data_out_[pos_iter] =
        round_normal<DType>(CPUOPBase::round_mode_, tmp, CPUOPBase::data_min_,
                            CPUOPBase::data_max_);
  }
}

INSTANTIATE_TPCLASS(UnaryTanhFix);
REG_OP_INSTANCE_FUNC("tanh-fix", UnaryTanhFix);

}  // namespace cpu
}  // namespace vart
