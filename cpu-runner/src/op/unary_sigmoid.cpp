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

#include "unary_sigmoid.hpp"

namespace vart {
namespace cpu {

template <typename DType>
void UnarySigmoid<DType>::calculate() {
  for (auto i = 0; i < fmap_o_.num(); i++) {
    data_out_ptr_[i] = 1.0 / (1.0 + std::exp(-data_in_ptr_[i]));
  }
}

INSTANTIATE_TPCLASS(UnarySigmoid);
REG_OP_INSTANCE_FUNC("sigmoid", UnarySigmoid);

}  // namespace cpu
}  // namespace vart
