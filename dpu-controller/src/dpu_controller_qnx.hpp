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
#include "vitis/ai/qnx_dpu.h"
#include "xir/dpu_controller.hpp"
#include <memory>
namespace {
class DpuControllerQnx : public xir::DpuController {
public:
  DpuControllerQnx();
  virtual ~DpuControllerQnx();
  DpuControllerQnx(const DpuControllerQnx &other) = delete;
  DpuControllerQnx &operator=(const DpuControllerQnx &rhs) = delete;

public:
  virtual void run(const uint64_t code, const std::vector<uint64_t> &gen_reg,
                   int device_id = 0) override;

private:
  task_handle_t allocate_task_id();

private:
  std::shared_ptr<vitis::ai::QnxDpu> dpu_;
  unsigned long task_id_;
};
} // namespace