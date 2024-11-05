/*
 * Copyright 2022-2023 Advanced Micro Devices Inc.
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

#include "resize.hpp"

namespace vart {
namespace cpu {

template <typename DType>
class ResizeBilinearFix : public Resize<DType> {
 public:
  ResizeBilinearFix(const xir::Subgraph* subg, const xir::Op* op, IMapTBs_t inputs,
            CPUTBPtr_t output);
  virtual ~ResizeBilinearFix() = default;

  virtual void run() override final;

  virtual void print_param() override final;

 protected:
  void resize_bilinear_fix();

 protected:
  int fix_width_{8};
  int fp_input_;
  int fp_output_;
  int shift_;

 private:
  // inner shifts: upshift -> shift_0 -> upshift -> shift_1
  int upshift_factor = 32768;  // 2^15
  int shift_0 = 8;
  int shift_1 = 22;
  std::string lut_round_mode_ = "STD_ROUND";  // TODO

  using Resize<DType>::mode_;
  using Resize<DType>::align_corners_;
  using Resize<DType>::half_pixel_centers_;

  using Resize<DType>::data_in_ptr_;
  using Resize<DType>::data_out_ptr_;
  using Resize<DType>::fmap_i_;
  using Resize<DType>::fmap_o_;
  using CPUOPBase::round_mode_;
};

}  // namespace cpu
}  // namespace vart

