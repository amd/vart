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
class QlinearSwish : public Eltwise<DType> {
 public:
  QlinearSwish(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
               CPUTBPtr_t output);
  ~QlinearSwish() = default;

  virtual void eltwise(std::uint32_t start_index,
                       std::uint32_t end_index) override final;

 private:
  using Eltwise<DType>::xir_op_;
  using Eltwise<DType>::data_in_;
  using Eltwise<DType>::data_out_;
  using Eltwise<DType>::THREAD_NUM;
  using Eltwise<DType>::THREAD_WORKLOAD;

  using CPUOPBase::data_max_;
  using CPUOPBase::data_min_;
};

}  // namespace cpu
}  // namespace vart
