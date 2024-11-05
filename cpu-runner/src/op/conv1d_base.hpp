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

template <typename DType, typename WType>
class Conv1dBase : public CPUOPBase {
 public:
  enum InputTensorType {
    INPUT,
    WEIGHTS,
    BIAS,
  };
  // input tensor name
  const static vector<string> ITName;

 public:
  Conv1dBase(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
           CPUTBPtr_t output);
  virtual ~Conv1dBase() = default;
  VART_DISABLE_COPY_AND_ASSIGN(Conv1dBase);

  virtual void run() override {}

  virtual void print_param() override;
  virtual void check_param() override;

  virtual void read() override final;

  virtual uint64_t get_workload() override final;

 protected:
  void read_input();
  void read_weights();
  void read_bias();

  void conv();
  void conv_normal();
  void conv_normal_thread();
  void conv_gemm();
  void conv_gemm_thread();

  void conv_one(int idx_dst_n, int idx_dst_h, int idx_dst_w, int idx_oc);
  void bias();

 private:
  uint32_t THREAD_NUM;
  uint32_t THREAD_WORKLOAD;

 protected: // fmap = xir_tensor_i_dims;
  FMap_t raw_fmap_i_;
  FMap_t fmap_i_;
  FMap_t raw_fmap_w_;
  FMap_t fmap_w_;
  FMap_t fmap_b_;
  FMap_t fmap_o_;

  Pad_t pad_;
  Kernel_t kernel_;
  // NOTE: must init stride_ and dilation_ using 1,
  // used to handle transposed-conv
  Stride_t stride_{1, 1};
  Dilation_t dilation_{1, 1};

  bool has_bias_;

  DType* data_in_ptr_{nullptr};
  WType* weights_ptr_{nullptr};
  WType* bias_ptr_{nullptr};
  DType* data_out_ptr_{nullptr};
};

template <typename DType, typename WType>
class Conv1d : public Conv1dBase<DType, WType> {
public:
  Conv1d(const xir::Subgraph* subg, const xir::Op* op,
      IMapTBs_t inputs, CPUTBPtr_t output);
  virtual ~Conv1d() = default;

  virtual void run() override;

protected:
  using Conv1dBase<DType, WType>::raw_fmap_i_;
  using Conv1dBase<DType, WType>::fmap_i_;
  using Conv1dBase<DType, WType>::raw_fmap_w_;
  using Conv1dBase<DType, WType>::fmap_w_;
  using Conv1dBase<DType, WType>::fmap_b_;
  using Conv1dBase<DType, WType>::fmap_o_;
  using Conv1dBase<DType, WType>::pad_;
  using Conv1dBase<DType, WType>::kernel_;
  using Conv1dBase<DType, WType>::stride_;
  using Conv1dBase<DType, WType>::dilation_;
  using Conv1dBase<DType, WType>::has_bias_;
};

}  // namespace cpu
}  // namespace vart

