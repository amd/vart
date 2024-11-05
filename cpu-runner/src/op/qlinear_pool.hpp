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

#include "avg_pool.hpp"
#include "max_pool.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class QlinearPool : public AvgPool<DType>, public MaxPool<DType> {
 public:
  QlinearPool(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
              CPUTBPtr_t output);
  ~QlinearPool() = default;

  virtual void run() override final;

  virtual void print_param() override final;

 protected:
  // void post_process();

  void avg_pool_qdq();
  void avg_pool_qdq_normal();
  void avg_pool_qdq_thread();
  inline void avg_pool_qdq_one(int i);

 protected:
  std::int32_t fix_width_{8};

  std::int32_t c_0_;
  std::int32_t c_1_;
  std::int32_t shift_c0_;
  std::int32_t shift_c1_;

  using PoolBase<DType>::pool_type_;
  using PoolBase<DType>::raw_fmap_i_;
  using PoolBase<DType>::fmap_i_;
  using PoolBase<DType>::fmap_o_;
  using PoolBase<DType>::raw_pad_;
  using PoolBase<DType>::pad_;
  using PoolBase<DType>::kernel_;
  using PoolBase<DType>::stride_;
  using PoolBase<DType>::data_in_ptr_;
  using PoolBase<DType>::data_out_ptr_;

  using PoolBase<DType>::THREAD_NUM;
  using PoolBase<DType>::THREAD_WORKLOAD;
  using PoolBase<DType>::FMAP_SIZE;
  using PoolBase<DType>::data_max_;
  using PoolBase<DType>::data_min_;
};

}  // namespace cpu
}  // namespace vart