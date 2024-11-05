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

#include "conv2d.hpp"

namespace vart {
namespace cpu {

template <typename DType, typename WType>
class Conv2dFix : public Conv2d<DType, WType> {
 public:
  Conv2dFix(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
            CPUTBPtr_t output);
  ~Conv2dFix() = default;

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
  xir::DataType output_data_type_;

  int shift_bias_{0};
  int shift_cut_{0};
  int hsigmoid_in_{0};
  int shift_hsigmoid_{0};
  int shift_hswish_{0};
  int prelu_in_{0};
  int prelu_shift_{0};
  double prelu_alpha_{0.1015625};

  int nonlinear_type_;
  bool enable_bfp_;
  DType data_min_;
  DType data_max_;

  using ConvBase<DType, WType>::has_bias_;
  using ConvBase<DType, WType>::fmap_i_;
  using ConvBase<DType, WType>::fmap_w_;
  using ConvBase<DType, WType>::fmap_o_;
  using ConvBase<DType, WType>::stride_;
  using ConvBase<DType, WType>::kernel_;
  using ConvBase<DType, WType>::dilation_;

  using ConvBase<DType, WType>::bias_ptr_;
  using ConvBase<DType, WType>::data_in_ptr_;
  using ConvBase<DType, WType>::weights_ptr_;
  using ConvBase<DType, WType>::data_out_ptr_;
  using ConvBase<DType, WType>::has_shift_psum_;
  using ConvBase<DType, WType>::shift_psum_;
  using ConvBase<DType, WType>::has_ic_iter_;
  using ConvBase<DType, WType>::ic_iter_;
  using ConvBase<DType, WType>::is_first_layer_;
  using ConvBase<DType, WType>::enable_conv_dirty_;
  using ConvBase<DType, WType>::xir_op_;
  using CPUOPBase::round_mode_;
};

}  // namespace cpu
}  // namespace vart
