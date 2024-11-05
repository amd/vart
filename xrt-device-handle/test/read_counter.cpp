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
#include <glog/logging.h>
#include <iomanip>
#include <iostream>

#include "vitis/ai/parse_value.hpp"
#include "xir/xrt_device_handle.hpp"
using namespace std;

static std::string reg_conf = "/usr/share/vart/reg.conf";
void xdpu_get_counter(const xir::XrtDeviceHandle* handle, const std::string& cu_name,
                      size_t cu_index) {
  struct reg {
    uint32_t addr;
    std::string name;
  };
  std::vector<reg> regs;
  std::ifstream stream(reg_conf);
  std::string name;
  std::string offset;
  while ((stream >> name >> offset).good()) {
    uint64_t offset2;
    // LOG(INFO) << "name=" << name << " offset=" << offset;
    vitis::ai::parse_value(offset, offset2);
    regs.emplace_back(reg{(uint32_t)offset2, name});
  }

  for (const auto& reg : regs) {
    auto value = handle->read_register(cu_name, cu_index, reg.addr);
    LOG_IF(INFO, true) << "0x" << std::hex << (reg.addr) << "\t"  //
                       << std::setfill(' ') << "0x" << std::setw(16)
                       << std::left << value << " "  //
                       << std::dec << std::setw(16) << std::right << value
                       << "\t"  //
                       << reg.name << "\t" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    LOG(INFO) << "usage: " << argv[0] << " <reg_conf> <cu_name> <cu_index> ";
    LOG(INFO) << "eg: " << argv[0] << " /usr/share/vart/reg.conf dpu 0";
    return 1;
  }
  auto h = xir::XrtDeviceHandle::get_instance();
  reg_conf = argv[1];
  auto cu_name = std::string(argv[2]);
  auto index = std::stoi(std::string(argv[3]));
  xdpu_get_counter(h.get(), cu_name, index);
  return 0;
}
