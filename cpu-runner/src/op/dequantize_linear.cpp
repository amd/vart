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

#include "dequantize_linear.hpp"

#include "cpu_util.hpp"
namespace vart {
namespace cpu {

template <typename DType, typename FixType>
const std::vector<std::string> DequantizeLinear<DType, FixType>::ITName = {
    "input",
    "scale",
    "zero_point",
};

template <typename DType, typename FixType>
DequantizeLinear<DType, FixType>::DequantizeLinear(const xir::Subgraph* subg,
                                                   const xir::Op* op,
                                                   IMapTBs_t inputs,
                                                   CPUTBPtr_t output)
    : CPUOPBase(subg, op, inputs, output) {
  UNI_LOG_CHECK(xir_op_->has_attr("data_type"), VART_FOUND)
      << "attr `data_type` is required";
  data_type = xir_op_->get_attr<std::string>("data_type");
  UNI_LOG_CHECK(xir_op_->has_attr("scale"), VART_FOUND)
      << "attr `scale` is required";
  scale = xir_op_->get_attr<std::vector<float>>("scale");
  UNI_LOG_CHECK(xir_op_->has_attr("zero_point"), VART_FOUND)
      << "attr `zero_point` is required";
  zero_point = xir_op_->get_attr<std::vector<std::int32_t>>("zero_point");
  UNI_LOG_CHECK(xir_op_->has_attr("axis"), VART_FOUND)
      << "attr `axis` is required";
  axis = xir_op_->get_attr<std::int32_t>("axis");
  UNI_LOG_CHECK(zero_point.size() == scale.size() && zero_point.size() == 1,
                VART_SIZE_ERROR)
      << "zero_point's shape must match scale";

  std::vector<std::int32_t> input_shape;
  GET_INPUT_DIMX_FMAP(
      input_shape, input,
      6);  // input_shape is output , input is input tensor's name
  input_len = get_vec_mul(input_shape);
  has_zero_point =
      xir_op_->get_input_num(ITName[ZERO_POINT]) > 0 ? true : false;
}

template <typename DType, typename FixType>
void DequantizeLinear<DType, FixType>::run() {
  // // do necessary check
  // this->print_param();
  // this->check_param();

  // // read data
  // read();

  // do qlinear
  for (auto i = 0u; i < input_len; i++) {
    this->float2fix_dpu_round(i);
  }
  // do save, debug...
  // save();
}

template <typename DType, typename FixType>
void DequantizeLinear<DType, FixType>::read() {
  // read input
  input_input_ptr = GET_CPUTB_DType_PTR(DType, inputs_.at(ITName[INPUT]).at(0));

  // handle output buffer
  output_output_ptr = GET_CPUTB_DType_PTR(FixType, output_);

  // If there is an input, we will prioritize the input scale and zp
  if (xir_op_->get_input_num(ITName[SCALE]) > 0) {
    auto input_scale_ptr =
        GET_CPUTB_DType_PTR(FixType, inputs_.at(ITName[SCALE]).at(0));
    scale.clear();
    scale.push_back(input_scale_ptr[0]);
    if (xir_op_->get_input_num(ITName[ZERO_POINT]) > 0) {
      auto input_zero_point_ptr =
          GET_CPUTB_DType_PTR(DType, inputs_.at(ITName[ZERO_POINT]).at(0));
      zero_point.clear();
      zero_point.push_back(input_zero_point_ptr[0]);
    } else {
      zero_point.clear();
    }
  }
}

class DequantizeLinear_impl : public DequantizeLinear<std::int32_t, float> {
 public:
  DequantizeLinear_impl(const xir::Subgraph* subg, const xir::Op* op,
                        IMapTBs_t inputs, CPUTBPtr_t output)
      : DequantizeLinear<std::int32_t, float>(subg, op, inputs, output) {}
  ~DequantizeLinear_impl() = default;
};

REG_NONTP_OP_INSTANCE_FUNC("dequantize-linear", DequantizeLinear_impl);

}  // namespace cpu
}  // namespace vart