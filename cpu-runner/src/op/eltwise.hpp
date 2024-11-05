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

#include "cpu_op_base.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class Eltwise : public CPUOPBase {
 public:
  enum InputTensorType {
    INPUT,
  };
  // input tensor name
  const static vector<string> ITName;

 public:
  Eltwise(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
          CPUTBPtr_t output);
  virtual ~Eltwise() = default;

  virtual void run() override;

  virtual void print_param() override;
  virtual void check_param() override;

  virtual void read() override final;

  virtual uint64_t get_workload() override final;

 private:
  void align_dim();

 protected:
  virtual void eltwise(std::uint32_t start_index, std::uint32_t end_index);
  vector<Dimension> fmap_i_;
  Dimension fmap_o_;

  std::string elt_type_;
  int input_num_;
  bool broadcast_;

  // caculate buffer
  vector<DType*> data_in_;
  DType* data_out_{nullptr};
  vector<float> data_in_buf_;
  float* data_in_buf_ptr_{nullptr};

  uint32_t THREAD_NUM;
  uint32_t THREAD_WORKLOAD;
  void eltwise_normal();
  void eltwise_thread();

 private:
  bool shift;
  vector<std::int32_t> fp_inputs_;
  vector<std::int32_t> shift_factor_;
};

}  // namespace cpu
}  // namespace vart
