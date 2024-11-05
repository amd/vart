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
#ifndef __SAVE_HPP__
#define __SAVE_HPP__

#include "buffer/DDR.hpp"
#include "pub/InstBase.hpp"
#include "xv2dpu/simUtil.hpp"

template <DPUVersion T>
class Save : public InstBase {
 public:
  explicit Save(int inst_type, int instid, vector<string>& inst_str,
                vector<uint32_t>& inst_val);
  ~Save();

 public:
  // excute load instruction
  void Exec() override final;
  void GenInstStr(int inst_fmt) override final;

 private:
  void debug_tick();
  void target_check();
  void combine(bool is_8_bit, vector<DPU_DATA_TYPE> data_in,
               DPU_DATA_TYPE* data_out);
  void save_ofm_PL() const {};
  void save_gen_ctrlpkg() const {};
  DPU_DATA_TYPE argmax(const vector<DPU_DATA_TYPE>& data_in);

 private:
  int32_t hp_id_;
  int32_t bank_id_;
  int32_t bank_addr_;
  int32_t jump_write_;
  int32_t jump_write_endl_;
  int32_t jump_read_;
  int32_t jump_read_endl_;
  int32_t length_;
  int32_t channel_;
  int32_t reg_id_;
  uint32_t ddr_addr_;

  int32_t block_num_{1};
  int32_t length_mid_;
  int32_t length_right_;
  int32_t tile_h_;
  int32_t tile_w_;
  uint32_t mt_addr_h_skip_;
  int32_t mt_addr_offset_w_right_;
  int32_t mt_addr_offset_w_mid_;
  int32_t ddr_addr_offset_right_;
  int32_t ddr_addr_offset_mid_;
  int32_t b0_loc_w_;
  int32_t b0_loc_h_;
  int32_t b1_loc_w_;
  int32_t b1_loc_h_;
  int32_t b2_loc_w_;
  int32_t b2_loc_h_;
  int32_t b3_loc_w_;
  int32_t b3_loc_h_;
  int32_t b4_loc_w_;
  int32_t b4_loc_h_;
  int32_t b5_loc_w_;
  int32_t b5_loc_h_;
  int32_t b6_loc_w_;
  int32_t b6_loc_h_;
  int32_t b7_loc_w_;
  int32_t b7_loc_h_;
  int32_t b8_loc_w_;
  int32_t b8_loc_h_;
  int32_t b9_loc_w_;
  int32_t b9_loc_h_;
  int32_t b10_loc_w_;
  int32_t b10_loc_h_;
  int32_t b11_loc_w_;
  int32_t b11_loc_h_;
  int32_t b12_loc_w_;
  int32_t b12_loc_h_;
  int32_t b13_loc_w_;
  int32_t b13_loc_h_;
  int32_t b14_loc_w_;
  int32_t b14_loc_h_;
  int32_t b15_loc_w_;
  int32_t b15_loc_h_;
  uint32_t mt_addr_skip_valid_{0};

  int32_t const_en_{0};
  int32_t const_value_{0};

  int32_t quant_lth_;
  int32_t argmax_{0};
};

template <>
Save<DPUVersion::DPUV2>::Save(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::DPUV3E>::Save(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::DPUV3ME>::Save(int inst_type, int instid,
                                vector<string>& inst_str,
                                vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::XVDPU>::Save(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::XV2DPU>::Save(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::XV3DPU>::Save(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::DPUV4E>::Save(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val);
template <>
Save<DPUVersion::DPU4F>::Save(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val);
template <>
void Save<DPUVersion::DPU4F>::Exec();
template <>
void Save<DPUVersion::DPU4F>::debug_tick();

#endif /* __SAVE_HPP__ */
