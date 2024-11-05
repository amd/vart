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

#include "concat.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class ConcatFix : public Concat<DType> {
 public:
  ConcatFix(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
            CPUTBPtr_t output);
  ~ConcatFix() = default;

  virtual void run() override final;

  virtual void print_param() override;

  virtual void read() override final;

 private:
  virtual void concat_normal() override;
  virtual void concat_thread() override;

  int fix_width_{8};
  vector<int> fp_inputs_;
  int fp_output_;
  vector<std::int32_t> shift_read;

  using Concat<DType>::fmap_i_;
  using Concat<DType>::input_num_;
  using Concat<DType>::data_in_;
  using Concat<DType>::data_out_;
  using Concat<DType>::fmap_o_;
  using Concat<DType>::axis_;
};

}  // namespace cpu
}  // namespace vart

