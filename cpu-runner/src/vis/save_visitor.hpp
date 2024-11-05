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

#include "cpu_base_inc.hpp"
#include "cpu_op_visitor.hpp"

class CPUOPBase;

namespace vart {
namespace cpu {

class SaveVisitor : public CPUOPVisitor {
public:
  explicit SaveVisitor() = default;
  VART_BIG_THREE_LAW(SaveVisitor);

public:
  virtual void run(CPUOPBase *op) {
    print_overview(op);

    op->save();
  }

  static unique_ptr<SaveVisitor> make() {
    return make_unique<SaveVisitor>();
  }
  static const VisitorType type = VisitorType::SaveVisitor;

private:
  void print_overview(CPUOPBase *op) {
    VART_DEBUG_GO_ON();

    auto *xir_op = op->get_xir_op();

    UNI_LOG_DEBUG_INFO << "SaveVisitor running op "
      << xir_op->get_attr<int>(CPU_RUN_ID) << endl;
    PRINT_DIVIDING_LINE();
    print_xir_op(op->get_xir_op());
  }

};

} // namespace cpu
} // namespace vart
