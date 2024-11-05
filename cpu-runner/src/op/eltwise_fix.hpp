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

#include "eltwise.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class EltwiseFix : public Eltwise<DType> {
 public:
  enum EltwiseNonlinearType {
    NONLINEAR_NONE = 0,
    NONLINEAR_RELU = 1,
    NONLINEAR_PRELU = 2,
    NONLINEAR_LEAKY_RELU = 3,
    NONLINEAR_RELU6 = 4,
    NONLINEAR_HSIGMOID = 5,
  };

 public:
  EltwiseFix(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
             CPUTBPtr_t output);
  ~EltwiseFix() = default;

  virtual void print_param() override;

  virtual void eltwise(std::uint32_t start_index,
                       std::uint32_t end_index) override final;
  void calculate_pow();
  void run() override;

 private:
  uint32_t broadcast_pos(uint32_t, int);
  EltwiseNonlinearType nonlinear_type_;

  vector<int> fp_inputs_;
  int fp_output_;
  std::string output_round_;
  xir::DataType output_data_type_;

  vector<int> shift_read_;
  int shift_write_;

  std::int32_t hsigmoid_in_ = 0;
  std::int32_t shift_hsigmoid_ = 0;
  std::int32_t shift_hswish_ = 0;
  std::int32_t prelu_in_ = 0;
  std::int32_t prelu_shift_ = 0;

  std::int32_t minval_ = 0;
  std::int32_t maxval_ = 0;
  float alpha_val_ = 0.f;

  float x_scale_ = 0.0f;
  float y_scale_ = 0.0f;
  std::int32_t x_zero_point_ = 0;
  std::int32_t y_zero_point_ = 0;
  double prelu_alpha_ = 0.f;
  std::vector<int32_t> axis_;

  using Eltwise<DType>::broadcast_;
  using Eltwise<DType>::fmap_i_;
  using Eltwise<DType>::fmap_o_;
  using Eltwise<DType>::input_num_;
  using Eltwise<DType>::elt_type_;
  using Eltwise<DType>::data_in_;
  using Eltwise<DType>::data_out_;
  using Eltwise<DType>::data_in_buf_;
  using Eltwise<DType>::data_in_buf_ptr_;

  using Eltwise<DType>::THREAD_NUM;
  using Eltwise<DType>::THREAD_WORKLOAD;
  using Eltwise<DType>::eltwise_thread;
  using Eltwise<DType>::eltwise_normal;
};

}  // namespace cpu
}  // namespace vart
