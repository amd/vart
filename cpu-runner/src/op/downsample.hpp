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

template<typename DType>
class DownSample : public CPUOPBase {
public:
  enum DownSampleMode {
    NEAREST,
    BILINEAR,
    POOL
  };

  enum InputTensorType {
    INPUT,
  };
  // input tensor name
  const static vector<string> ITName;

public:
  DownSample(const xir::Subgraph* subg, const xir::Op* op,
      IMapTBs_t inputs, CPUTBPtr_t output);
  virtual ~DownSample() = default;

  virtual void run() override;

  virtual void print_param() override;
  virtual void check_param() override;

  virtual void read() override final;

  virtual uint64_t get_workload() override final;

protected:
  void downsample();
  void downsample_one(int idx_n, int idx_h, int idx_w);

protected:
  FMap_t fmap_i_;
  FMap_t fmap_o_;

  int mode_{NEAREST};
  std::vector<float> scale_;

  // buffer
  DType* data_in_ptr_;
  DType* data_out_ptr_;
};

} // namespace cpu
} // namespace vart
