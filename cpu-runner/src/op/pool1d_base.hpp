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
class Pool1dBase : public CPUOPBase {
 public:
  enum PoolType {
    MAX_POOL,
    // AVG_POOL
  };

  enum InputTensorType {
    INPUT,
  };
  // input tensor name
  const static vector<string> ITName;

 public:
  Pool1dBase(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
             CPUTBPtr_t output);
  virtual ~Pool1dBase() = default;

  virtual void run() override {}

  virtual void print_param() override;
  virtual void check_param() override;

  virtual void read() override final;

  virtual uint64_t get_workload() override final;

 protected:
  int pool_type_;

  FMap_t raw_fmap_i_;
  FMap_t fmap_i_;
  FMap_t fmap_o_;

  Pad_t raw_pad_;
  Pad_t pad_;
  Kernel_t kernel_;
  Stride_t stride_;

  bool is_global_;

  DType pad_value_;

  DType* data_in_ptr_{nullptr};
  DType* data_out_ptr_{nullptr};

  uint32_t THREAD_NUM;
  uint32_t THREAD_WORKLOAD;
  uint32_t FMAP_SIZE;
};

template <typename DType>
class MaxPool1d : virtual public Pool1dBase<DType> {
 public:
  MaxPool1d(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
            CPUTBPtr_t output);
  virtual ~MaxPool1d() = default;

  virtual void run() override;

 protected:
  void max_pool();
  void max_pool_normal();
  void max_pool_thread();

  void max_pool_one(int idx_dst_n, int idx_dst_h, int idx_dst_w);

 protected:
  using Pool1dBase<DType>::pool_type_;
  using Pool1dBase<DType>::raw_fmap_i_;
  using Pool1dBase<DType>::fmap_i_;
  using Pool1dBase<DType>::fmap_o_;
  using Pool1dBase<DType>::raw_pad_;
  using Pool1dBase<DType>::pad_;
  using Pool1dBase<DType>::kernel_;
  using Pool1dBase<DType>::stride_;
  using Pool1dBase<DType>::data_in_ptr_;
  using Pool1dBase<DType>::data_out_ptr_;

  using Pool1dBase<DType>::THREAD_NUM;
  using Pool1dBase<DType>::THREAD_WORKLOAD;
  using Pool1dBase<DType>::FMAP_SIZE;
};

}  // namespace cpu
}  // namespace vart