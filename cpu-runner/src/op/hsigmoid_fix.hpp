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

#pragma once

#include "unary_base.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class UnaryHSigmoidFix : public UnaryBase<DType> {
 public:
  UnaryHSigmoidFix(const xir::Subgraph* subg, const xir::Op* op,
                   IMapTBs_t inputs, CPUTBPtr_t output)
      : UnaryBase<DType>(subg, op, inputs, output) {
    UnaryBase<DType>::unary_type_ = UnaryBase<DType>::SIGMOID;
    hsigmoid_in_ = op->get_attr<int>("hsigmoid_in");
    shift_hsigmoid_ = op->get_attr<int>("shift_hsigmoid");
  }
  virtual ~UnaryHSigmoidFix() = default;

 protected:
  virtual void calculate() override final;

 protected:
  using UnaryBase<DType>::fmap_i_;
  using UnaryBase<DType>::fmap_o_;
  using UnaryBase<DType>::data_in_ptr_;
  using UnaryBase<DType>::data_out_ptr_;

 private:
  int hsigmoid_in_;
  int shift_hsigmoid_;
};

}  // namespace cpu
}  // namespace vart
