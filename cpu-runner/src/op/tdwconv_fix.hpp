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

#include "tdwconv.hpp"

namespace vart {
namespace cpu {

template <typename DType, typename WType>
class TDWConvFix : public TDWConv<DType, WType> {
 public:
  TDWConvFix(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
             CPUTBPtr_t output);
  ~TDWConvFix() = default;

  virtual void run() override final;

  virtual void print_param() override;
  virtual void check_param() override;

 private:
  void fix();

 private:
  int fp_input_;
  int fp_weights_;
  int fp_bias_;
  int fp_output_;

  int shift_bias_{0};
  int shift_cut_{0};
  int hsigmoid_in_{0};
  int shift_hsigmoid_{0};
  int shift_hswish_{0};
  double leakyrelu_alpha_{0.1015625};

  int nonlinear_type_;

  using DWConvBase<DType, WType>::has_bias_;
  using DWConvBase<DType, WType>::fmap_o_;

  using DWConvBase<DType, WType>::bias_ptr_;
  using DWConvBase<DType, WType>::data_out_ptr_;
};

}  // namespace cpu
}  // namespace vart

