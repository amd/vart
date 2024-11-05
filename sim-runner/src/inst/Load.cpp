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

#define ENABLE_VITISAI_LIBBOOST

#include "Load.hpp"
#include "xv2dpu/simCfg.hpp"

// constructor and deconstructor
template <>
Load<DPUVersion::DPUV2>::Load(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::DPUV2>::InstInit(inst_type_, root_debug_path_, debug_,
                                      opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::DPUV2>::inst_table;
  // get load instruction field value
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];

  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];

  // broadcast=4'b0111: load const
  const_en_ = field_val_[TABLE::LOAD_FIELD_BROADCAST] == 7;
  const_value_ = field_val_[TABLE::LOAD_FIELD_CONST_VALUE];

  auto all_bank_num = Buffer<DPU_DATA_TYPE>::Instance().GetBankNum();
  UNI_LOG_CHECK(bank_id_ < all_bank_num, SIM_OUT_OF_RANGE)
      << "Load bank_id should be less than all_bank_num, " << bank_id_
      << " >= " << all_bank_num << endl;
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);
}

template <>
Load<DPUVersion::DPUV3E>::Load(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::DPUV3E>::InstInit(inst_type_, root_debug_path_, debug_,
                                       opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::DPUV3E>::inst_table;
  // get load instruction field value
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];

  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];

  jump_read_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ_ENDL];
  output_channel_num_ = field_val_[TABLE::LOAD_FIELD_OUTPUT_CHANNEL_NUM];
  ddr_mode_ = field_val_[TABLE::LOAD_FIELD_DDR_MODE];
  reg_id_1_ = field_val_[TABLE::LOAD_FIELD_REG_ID_1];
  ddr_addr_1_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR_1];

  target_check();
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);
}

template <>
Load<DPUVersion::DPUV4E>::Load(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::DPUV4E>::InstInit(inst_type_, root_debug_path_, debug_,
                                       opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::DPUV4E>::inst_table;
  // get load instruction field value
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];

  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];

  jump_read_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ_ENDL];
  output_channel_num_ = field_val_[TABLE::LOAD_FIELD_OUTPUT_CHANNEL_NUM];
  ddr_mode_ = field_val_[TABLE::LOAD_FIELD_DDR_MODE];
  load_mode_ = field_val_[TABLE::LOAD_FIELD_LOAD_MODE];

  target_check();
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > 64) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_ != 0" << endl;
  }
  avg_reg_.resize(64);
}

template <>
Load<DPUVersion::DPUV3ME>::Load(int inst_type, int instid,
                                vector<string>& inst_str,
                                vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::DPUV3ME>::InstInit(inst_type_, root_debug_path_, debug_,
                                        opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::DPUV3ME>::inst_table;
  // get load instruction field value
  mode_ = field_val_[TABLE::LOAD_FIELD_MODE];
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];
  pad_top_ = field_val_[TABLE::LOAD_FIELD_PAD_TOP];
  pad_bottom_ = field_val_[TABLE::LOAD_FIELD_PAD_BOTTOM];
  channel_offset_ = field_val_[TABLE::LOAD_FIELD_CHANNEL_OFFSET];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];
  reg_id_1_ = field_val_[TABLE::LOAD_FIELD_REG_ID_1];
  ddr_addr_1_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR_1];
  reg_id_2_ = field_val_[TABLE::LOAD_FIELD_REG_ID_2];
  ddr_addr_2_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR_2];

  auto all_bank_num = Buffer<DPU_DATA_TYPE>::Instance().GetBankNum();
  /*
          if(bank_id_ >= bank_num_[SimCfg::BANK_TYPE_IMG_HBM]
                                  && bank_id_ <
     (bank_num_[SimCfg::BANK_TYPE_IMG_HBM] +
     bank_num_[SimCfg::BANK_TYPE_IMG_CB])){ LOG(ERROR) << " Load is forbidden to
     access CB, this bank_id_ = " << bank_id_
                          <<", CB bank_id range " <<
     bank_num_[SimCfg::BANK_TYPE_IMG_HBM] <<":+" <<
     bank_num_[SimCfg::BANK_TYPE_IMG_CB] << endl; abort();
          }
          */
  UNI_LOG_CHECK(bank_id_ < all_bank_num, SIM_OUT_OF_RANGE)
      << "Load bank_id should be less than all_bank_num, " << bank_id_
      << " >= " << all_bank_num << endl;
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  if (mode_ != MODE_LOAD_SINGLE_REG && channel_offset_ != 0) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel_offset_ is " << channel_offset_
        << " when mode_ !=SINGLE_REG" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);
}

template <>
Load<DPUVersion::XVDPU>::Load(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::XVDPU>::InstInit(inst_type_, root_debug_path_, debug_,
                                      opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::XVDPU>::inst_table;
  // get load instruction field value
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];

  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];

  jump_write_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE_ENDL];
  block_num_ = field_val_[TABLE::LOAD_FIELD_BLOCK_NUM];

  // broadcast=4'b0111: load const
  const_en_ = field_val_[TABLE::LOAD_FIELD_BROADCAST] == 7;
  const_value_ = field_val_[TABLE::LOAD_FIELD_CONST_VALUE];

  auto all_bank_num = Buffer<DPU_DATA_TYPE>::Instance().GetBankNum();
  UNI_LOG_CHECK(bank_id_ < all_bank_num, SIM_OUT_OF_RANGE)
      << "Load bank_id should be less than all_bank_num, " << bank_id_
      << " >= " << all_bank_num << endl;
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);
}

template <>
Load<DPUVersion::XV2DPU>::Load(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::XV2DPU>::InstInit(inst_type_, root_debug_path_, debug_,
                                       opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::XV2DPU>::inst_table;
  // get load instruction field value
  mt_dst_ = field_val_[TABLE::LOAD_FIELD_MT_DST];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_MT_ADDR];

  block_num_ = field_val_[TABLE::LOAD_FIELD_BLOCK_NUM];
  broadcast_ = field_val_[TABLE::LOAD_FIELD_BROADCAST];
  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  const_value_ = field_val_[TABLE::LOAD_FIELD_CONST_VALUE];
  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];

  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  if (2 == mt_dst_) {
    UNI_LOG_CHECK((channel_ % 8) == 0, SIM_OUT_OF_RANGE);
    channel_ = channel_ / 8;
  } else if (3 == mt_dst_) {
    UNI_LOG_CHECK((channel_ % 2) == 0, SIM_OUT_OF_RANGE);
    channel_ = channel_ / 2;
  } else if (4 == mt_dst_) {
    UNI_LOG_CHECK((channel_ % 2) == 0, SIM_OUT_OF_RANGE);
    channel_ = channel_ / 2;
  }

  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];
  jump_write_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE_ENDL];

  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  bank_id_ = (mt_dst_ == 0) ? 0
                            : Buffer<DPU_DATA_TYPE>::Instance()
                                  .GetBank(wgt_bank_group_name, 0)
                                  ->GetID();

  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);

    // update ddr_addr_
    auto engine_id = SimCfg::Instance().get_tiling_engine_id();
    auto ddr_offset_map = SimCfg::Instance().get_ddr_addr_offset_map();
    auto fake_rid_to_real_rid_map = SimCfg::Instance().get_fake_rid_to_real_rid_map();
    auto combine_num_to_str = [] (int32_t v1, int32_t v2) -> 
        std::string { return std::to_string(v1) + "_" + std::to_string(v2); };
    auto key_str = combine_num_to_str(reg_id_, engine_id);
    if (ddr_offset_map.count(key_str)) {
      ddr_addr_ += ddr_offset_map.at(key_str);
      reg_id_ = fake_rid_to_real_rid_map.at(key_str);
    }

}

template <>
Load<DPUVersion::XV3DPU>::Load(int inst_type, int instid,
                               vector<string>& inst_str,
                               vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::XV3DPU>::InstInit(inst_type_, root_debug_path_, debug_,
                                       opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::XV3DPU>::inst_table;
  // get load instruction field value
  mt_dst_ = field_val_[TABLE::LOAD_FIELD_MT_DST];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_MT_ADDR];

  block_num_ = field_val_[TABLE::LOAD_FIELD_BLOCK_NUM];
  broadcast_ = field_val_[TABLE::LOAD_FIELD_BROADCAST];
  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  const_value_ = field_val_[TABLE::LOAD_FIELD_CONST_VALUE];
  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];

  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];

  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];
  jump_write_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE_ENDL];

  jump_read_endl_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ_ENDL];

  trans_mode_ = field_val_[TABLE::LOAD_FIELD_TRANS_MODE];
  tile_stride_h_ = field_val_[TABLE::LOAD_FIELD_TILE_STRIDE_H];
  tile_stride_w_ = field_val_[TABLE::LOAD_FIELD_TILE_STRIDE_W];
  sample_scale_h_ = field_val_[TABLE::LOAD_FIELD_SAMPLE_SCALE_H];
  sample_scale_w_ = field_val_[TABLE::LOAD_FIELD_SAMPLE_SCALE_W];

  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  bank_id_ = (mt_dst_ == 0) ? 0
                            : Buffer<DPU_DATA_TYPE>::Instance()
                                  .GetBank(wgt_bank_group_name, 0)
                                  ->GetID();

  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);

  // update ddr_addr_
  auto engine_id = SimCfg::Instance().get_tiling_engine_id();
  auto ddr_offset_map = SimCfg::Instance().get_ddr_addr_offset_map();
  auto fake_rid_to_real_rid_map =
      SimCfg::Instance().get_fake_rid_to_real_rid_map();
  auto combine_num_to_str = [](int32_t v1, int32_t v2) -> std::string {
    return std::to_string(v1) + "_" + std::to_string(v2);
  };
  auto key_str = combine_num_to_str(reg_id_, engine_id);
  if (ddr_offset_map.count(key_str)) {
    ddr_addr_ += ddr_offset_map.at(key_str);
    reg_id_ = fake_rid_to_real_rid_map.at(key_str);
  }
}

template <>
Load<DPUVersion::DPU4F>::Load(int inst_type, int instid,
                              vector<string>& inst_str,
                              vector<uint32_t>& inst_val)
    : InstBase(inst_type, instid, inst_str, inst_val) {
  // debug
  debug_ = debug_ && SimCfg::Instance().get_debug_instr(SimCfg::DBG_INSTR_LOAD);
  Helper<DPUVersion::DPU4F>::InstInit(inst_type_, root_debug_path_, debug_,
                                      opcode_, debug_path_);
  using TABLE = TableInterface<DPUVersion::DPU4F>::inst_table;
  // get load instruction field value
  hp_id_ = field_val_[TABLE::LOAD_FIELD_HP_ID];
  bank_id_ = field_val_[TABLE::LOAD_FIELD_BANK_ID];
  bank_addr_ = field_val_[TABLE::LOAD_FIELD_BANK_ADDR];

  quant_lth_ = field_val_[TABLE::LOAD_FIELD_QUANT_LTH];  // 0: 4 bit 1: 8 bit
  pad_start_ = field_val_[TABLE::LOAD_FIELD_PAD_START];
  pad_end_ = field_val_[TABLE::LOAD_FIELD_PAD_END];
  pad_idx_ = field_val_[TABLE::LOAD_FIELD_PAD_IDX];

  jump_read_ = field_val_[TABLE::LOAD_FIELD_JUMP_READ];
  jump_write_ = field_val_[TABLE::LOAD_FIELD_JUMP_WRITE];
  length_ = field_val_[TABLE::LOAD_FIELD_LENGTH];
  mode_avg_ = field_val_[TABLE::LOAD_FIELD_MODE_AVG];
  channel_ = field_val_[TABLE::LOAD_FIELD_CHANNEL];
  reg_id_ = field_val_[TABLE::LOAD_FIELD_REG_ID];
  ddr_addr_ = field_val_[TABLE::LOAD_FIELD_DDR_ADDR];

  auto all_bank_num = Buffer<DPU_DATA_TYPE>::Instance().GetBankNum();
  UNI_LOG_CHECK(bank_id_ < all_bank_num, SIM_OUT_OF_RANGE)
      << "Load bank_id should be less than all_bank_num, " << bank_id_
      << " >= " << all_bank_num << endl;
  UNI_LOG_CHECK(mode_avg_ <= MODE_AVG_MAX, SIM_OUT_OF_RANGE)
      << ", mode_avg_ is  " << mode_avg_;
  if (mode_avg_ != 0 && channel_ > MODE_AVG_CHANNEL_LIMIT) {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "channel is " << channel_ << " when mode_avg_!=0" << endl;
  }
  avg_reg_.resize(MODE_AVG_CHANNEL_LIMIT);

  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  auto wgt_bank_start_id = Buffer<DPU_DATA_TYPE>::Instance()
                               .GetBank(wgt_bank_group_name, 0)
                               ->GetID();
  is_img_bank = (bank_id_ < wgt_bank_start_id) ? true : false;
  auto wgt_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(wgt_bank_group_name);
  is_wgt_bank = ((bank_id_ >= wgt_bank_start_id) &&
                 (bank_id_ < (wgt_bank_start_id + wgt_bank_num)))
                    ? true
                    : false;
}

template <DPUVersion T>
Load<T>::~Load() {}

// public funcs
template <DPUVersion T>
void Load<T>::Exec() {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  ddr_range_check(reg_id_);
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;
  auto bank_width = bank->GetW();

  // initial avg_reg_ for minus mean
  if (mode_avg_ != 0) {
    auto mean_bank_group = ArchCfg::Instance()
                               .get_param()
                               .load_engine()
                               .meanvalue_reduction()
                               .meanvalue_bank();
    auto mean_bank =
        Buffer<DPU_DATA_TYPE>::Instance().GetBank(mean_bank_group, 0);
    mean_bank->Read(0, channel_, avg_reg_.data());
  }

  // set pad_mask for channel augment
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);

  if (channel_ >= jump_read_ && channel_ <= MODE_AVG_CHANNEL_LIMIT) {
    auto pad_start_num_this_pixel = 0;
    auto pad_end_num_this_pixel = 0;
    auto pad_end_start_idx = length_ - pad_idx_;
    for (auto idx_length = 0; idx_length < length_; idx_length++) {
      auto mask_idx = idx_length * channel_;
      pad_start_num_this_pixel = pad_start_ - idx_length * jump_read_;
      pad_start_num_this_pixel =
          pad_start_num_this_pixel > 0 ? pad_start_num_this_pixel : 0;
      UNI_LOG_CHECK(pad_start_num_this_pixel < channel_, SIM_OUT_OF_RANGE);
      for (auto idx_channel = 0; idx_channel < pad_start_num_this_pixel;
           idx_channel++) {
        pad_mask[mask_idx + idx_channel] = false;
      }
      if (idx_length >= pad_end_start_idx) {
        pad_end_num_this_pixel =
            pad_end_ + jump_read_ * (idx_length - pad_end_start_idx);
        auto pad_end_start_this_pixel =
            Util::AlignByN(channel_, bank_width) - pad_end_num_this_pixel;
        UNI_LOG_CHECK(pad_end_start_this_pixel > 0, SIM_OUT_OF_RANGE);
        for (int32_t idx_channel = pad_end_start_this_pixel;
             idx_channel < channel_; idx_channel++) {
          pad_mask[mask_idx + idx_channel] = false;
        }
      }
    }
  }

  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      auto ddr_offset = idx_length * jump_read_ + idx_channel;
      auto pad_mask_idx = idx_length * channel_ + idx_channel;
      if (pad_mask[pad_mask_idx] == false) {
        data[idx_channel] = 0;
        continue;
      }
      if (const_en_) {
        data[idx_channel] = const_value_;
        continue;
      }

      // fetch data from ddr and do minus mean
      data[idx_channel] = buf[ddr_offset];
      if (mode_avg_ != 0) {
        uint8_t* orivalue =
            reinterpret_cast<uint8_t*>(data.data() + idx_channel);
        uint8_t* subvalue =
            reinterpret_cast<uint8_t*>(avg_reg_.data() + idx_channel);
        uint32_t delta = (*orivalue & 0x000003ff) - (*subvalue);
        delta = delta >> (mode_avg_ - 1);
        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
      }
    }
    auto bank_addr = bank_addr_ + idx_length * jump_write_;
    bank->Write(bank_addr, channel_, data.data());
  }
  debug_tick();
}

template <>
void Load<DPUVersion::DPUV3E>::Exec() {
  if (bank_id_ != 0x3f) {
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    UNI_LOG_CHECK(
        static_cast<uint64_t>((length_ - 1) * jump_read_ + channel_) <=
            DDR::Instance().GetSize(reg_id_),
        SIM_OUT_OF_RANGE);
    auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
    UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
        << "bank_id " << bank_id_ << " out of range!" << endl;
    auto bank_width = bank->GetW();

    // initial avg_reg_ for minus mean
    if (mode_avg_ != 0) {
      auto mean_bank_group = ArchCfg::Instance()
                                 .get_param()
                                 .load_engine()
                                 .meanvalue_reduction()
                                 .meanvalue_bank();
      auto mean_bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(mean_bank_group, 0);
      mean_bank->Read(0, channel_, avg_reg_.data());
    }

    // set pad_mask for channel augment
    int ll = length_ * channel_;
    vector<bool> pad_mask(ll, true);
    gen_PadMask(pad_mask, bank_width);
    load_and_minusmean(buf, bank, pad_mask);

    debug_tick();
  }

  else {
    int ocp =
        ArchCfg::Instance().get_param().conv_engine().output_channel_parallel();
    UNI_LOG_CHECK(output_channel_num_ <= ocp, SIM_OUT_OF_RANGE);
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    auto* buf1 = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_1_, ddr_addr_1_));
    for (int idx_oc = 0; idx_oc < output_channel_num_; idx_oc++) {
      auto wgt_bank_group =
          ArchCfg::Instance().get_param().conv_engine().weight_bank();
      auto bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(wgt_bank_group, idx_oc);
      for (int idx_length = 0; idx_length < length_; idx_length++) {
        vector<DPU_DATA_TYPE> data(channel_, 0);
        for (int idx_channel = 0; idx_channel < channel_; idx_channel++) {
          auto actual_oc = (idx_oc < ocp / 2) ? idx_oc : (idx_oc - ocp / 2);
          auto ddr_offset = actual_oc * jump_read_endl_ +
                            idx_length * jump_read_ + idx_channel;

          // fetch data from ddr and do minus mean
          data[idx_channel] =
              (idx_oc < ocp / 2) ? buf[ddr_offset] : buf1[ddr_offset];
          // record ddr used
          if (idx_oc < ocp / 2) {
            DDR::Instance().set_addr_used(reg_id_, ddr_addr_ + ddr_offset);
          } else {
            DDR::Instance().set_addr_used(reg_id_1_, ddr_addr_1_ + ddr_offset);
          }
          auto bank_addr = bank_addr_ + idx_length * jump_write_;
          bank->Write(bank_addr, channel_, data.data());
        }
        debug_tick();
      }
    }
  }
}

template <>
void Load<DPUVersion::DPUV4E>::Exec() {
  if (bank_id_ != 0xff) {
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    UNI_LOG_CHECK(
        static_cast<uint64_t>((length_ - 1) * jump_read_ + channel_) <=
            DDR::Instance().GetSize(reg_id_),
        SIM_OUT_OF_RANGE);
    auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
    UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
        << "bank_id " << bank_id_ << " out of range!" << endl;
    auto bank_width = bank->GetW();

    // initial avg_reg_ for minus mean
    if (mode_avg_ != 0) {
      auto mean_bank_group = ArchCfg::Instance()
                                 .get_param()
                                 .load_engine()
                                 .meanvalue_reduction()
                                 .meanvalue_bank();
      auto mean_bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(mean_bank_group, 0);
      mean_bank->Read(0, channel_, avg_reg_.data());
    }

    // set pad_mask for channel augment
    int ll = length_ * channel_;
    vector<bool> pad_mask(ll, true);
    gen_PadMask(pad_mask, bank_width);

    for (int idx_length = 0; idx_length < length_; idx_length++) {
      vector<DPU_DATA_TYPE> data(channel_, 0);
      for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
        auto ddr_offset = idx_length * jump_read_ + idx_channel;
        DDR::Instance().set_addr_used(reg_id_, ddr_addr_ + ddr_offset);
        auto pad_mask_idx = idx_length * channel_ + idx_channel;
        if (pad_mask[pad_mask_idx] == false) {
          data[idx_channel] = 0;
          continue;
        }

        // fetch data from ddr and do minus mean
        if (load_mode_ == 0) {
          data[idx_channel] = buf[ddr_offset];
        } else {
          data[idx_channel] = ddr_addr_ % 256;
        }
        UNI_LOG_CHECK(load_mode_ <= 1, SIM_OUT_OF_RANGE);
        if (mode_avg_ != 0) {
          uint8_t* orivalue =
              reinterpret_cast<uint8_t*>(data.data() + idx_channel);
          uint8_t* subvalue =
              reinterpret_cast<uint8_t*>(avg_reg_.data() + idx_channel);
          uint32_t delta = (*orivalue & 0x000003ff) - (*subvalue);
          delta = delta >> (mode_avg_ - 1);
          data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
        }
      }
      auto bank_addr = bank_addr_ + idx_length * jump_write_;
      bank->Write(bank_addr, channel_, data.data());
    }
    debug_tick();
  }

  else {
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    for (int idx_oc = 0; idx_oc < output_channel_num_; idx_oc++) {
      int group = idx_oc / 16;
      int stride_id =
          (group < 4) ? (group * 16 + idx_oc) : (group * 16 - 112 + idx_oc);
      auto wgt_bank_group =
          ArchCfg::Instance().get_param().conv_engine().weight_bank();
      auto bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(wgt_bank_group, idx_oc);
      for (int idx_length = 0; idx_length < length_; idx_length++) {
        vector<DPU_DATA_TYPE> data(channel_, 0);
        for (int idx_channel = 0; idx_channel < channel_; idx_channel++) {
          auto ddr_offset = stride_id * jump_read_endl_ +
                            idx_length * jump_read_ + idx_channel;
          DDR::Instance().set_addr_used(reg_id_, ddr_addr_ + ddr_offset);
          // fetch data from ddr and do minus mean
          data[idx_channel] = buf[ddr_offset];
          auto bank_addr = bank_addr_ + idx_length * jump_write_;
          bank->Write(bank_addr, channel_, data.data());
        }
        debug_tick();
      }
    }
  }
}

template <>
void Load<DPUVersion::DPUV3ME>::Exec() {
  // initial avg_reg_ for minus mean
  if (mode_avg_ != 0) {
    auto mean_bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank("MVR", 0);
    mean_bank->Read(0, channel_, avg_reg_.data());
  }
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;
  auto bank_width = bank->GetW();

  // set pad_mask for channel augment
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);
  if (channel_ >= jump_read_ && channel_ <= MODE_AVG_CHANNEL_LIMIT) {
    int32_t pad_start_num_this_pixel = 0;
    int32_t pad_end_num_this_pixel = 0;
    int32_t pad_end_start_idx = length_ - pad_idx_;
    for (auto idx_length = 0; idx_length < length_; idx_length++) {
      int32_t mask_idx = idx_length * channel_;
      pad_start_num_this_pixel = pad_start_ - idx_length * jump_read_;
      pad_start_num_this_pixel =
          pad_start_num_this_pixel > 0 ? pad_start_num_this_pixel : 0;
      UNI_LOG_CHECK(pad_start_num_this_pixel < channel_, SIM_OUT_OF_RANGE);
      for (auto idx_channel = 0; idx_channel < pad_start_num_this_pixel;
           idx_channel++) {
        pad_mask[mask_idx + idx_channel] = false;
      }
      if (idx_length >= pad_end_start_idx) {
        pad_end_num_this_pixel =
            pad_end_ + jump_read_ * (idx_length - pad_end_start_idx);
        int32_t pad_end_start_this_pixel =
            Util::AlignByN(channel_, bank_width) - pad_end_num_this_pixel;
        UNI_LOG_CHECK(pad_end_start_this_pixel > 0, SIM_OUT_OF_RANGE);
        for (auto idx_channel = pad_end_start_this_pixel;
             idx_channel < channel_; idx_channel++) {
          pad_mask[mask_idx + idx_channel] = false;
        }
      }
    }
  }
  // exec load
  if (mode_ == MODE_LOAD_SINGLE_REG) {
    exec_mode_single_reg(pad_mask);
  } else if (mode_ == MODE_LOAD_THREE_REG) {
    exec_mode_three_reg(pad_mask);
  }
}

template <>
void Load<DPUVersion::XVDPU>::Exec() {
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;
  auto bank_width = bank->GetW();
  auto bank_depth = bank->GetH();
  // initial avg_reg_ for minus mean
  if (mode_avg_ != 0) {
    auto mean_bank_group = ArchCfg::Instance()
                               .get_param()
                               .load_engine()
                               .meanvalue_reduction()
                               .meanvalue_bank();
    auto mean_bank =
        Buffer<DPU_DATA_TYPE>::Instance().GetBank(mean_bank_group, 0);
    mean_bank->Read(0, channel_, avg_reg_.data());
  }

  // set pad_mask for channel augment
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);
  gen_PadMask(pad_mask, bank_width);

  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  auto wgt_bank_start_id = Buffer<DPU_DATA_TYPE>::Instance()
                               .GetBank(wgt_bank_group_name, 0)
                               ->GetID();
  auto wgt_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(wgt_bank_group_name);
  auto conv_ocp =
      ArchCfg::Instance().get_param().conv_engine().output_channel_parallel();
  auto conv_icp =
      ArchCfg::Instance().get_param().conv_engine().input_channel_parallel();
  is_wgt_bank = (bank_id_ >= wgt_bank_start_id) &&
                (bank_id_ < (wgt_bank_start_id + wgt_bank_num));

  // appointment for XVDPU long load weights
  const int hp_port_num = 4;    // 4 hp-port do load
  const int bank_per_port = 4;  // each hp-port load data for 4 bank
  if (is_wgt_bank) {
    UNI_LOG_CHECK(block_num_ % bank_per_port == 0, SIM_OUT_OF_RANGE)
        << "When load weights, block_num must be aligned to bank_per_port. Now "
           "block_num = "
        << block_num_ << ", hp_port_num = " << bank_per_port;
    UNI_LOG_CHECK(channel_ % bank_width == 0, SIM_OUT_OF_RANGE)
        << "When load weights, channel must be aligned to bank_width. Now "
           "channel = "
        << channel_ << ", bank_width = " << bank_width;
    UNI_LOG_CHECK(channel_ == jump_read_, SIM_OUT_OF_RANGE)
        << "When load weights, channel must equal jump_read. Now channel = "
        << channel_ << ", jump_read = " << jump_read_;
    auto cg = (channel_ + bank_width - 1) / bank_width;
    UNI_LOG_CHECK(cg == jump_write_, SIM_OUT_OF_RANGE)
        << "When load weights, jump_write must equal channel_group. Now "
           "channel_group = "
        << cg << ", jump_write = " << jump_write_;
    UNI_LOG_CHECK(cg * length_ == jump_write_endl_, SIM_OUT_OF_RANGE)
        << "When load weights, jump_write_endl must equal channel_group * "
           "length. Now channel_group * length = "
        << cg * length_ << ", jump_write_endl = " << jump_write_endl_;
    UNI_LOG_CHECK(bank_id_ == wgt_bank_start_id, SIM_OUT_OF_RANGE)
        << "Now bank_id = " << bank_id_ << ". When load weights, it shoud be "
        << wgt_bank_start_id;
  } else {
    UNI_LOG_CHECK(block_num_ == 1, SIM_OUT_OF_RANGE)
        << "Now bank_id = " << bank_id_
        << ". It is not a weights bank, so long load is unsupported. "
           "Block_num shoud be 1.";
  }

  if (!is_wgt_bank) { /*load img*/
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    ddr_range_check(reg_id_);
    auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
    UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE);
    load_and_minusmean(buf, bank, pad_mask);
    debug_tick();
  } else { /*long load weights*/
    auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
        DDR::Instance().GetAddr(reg_id_, ddr_addr_));
    string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                       to_string(instid_classify_) + ".tick";

    int stride = conv_ocp / wgt_bank_num;
    auto one_kernel_size = length_ * channel_;
    auto cg = (channel_ + bank_width - 1) / bank_width;
    auto block_one_iter = hp_port_num * bank_per_port * stride;
    int load_iter = block_num_ / block_one_iter;
#define BANK_VISION_LLW
#ifdef BANK_VISION_LLW
    int hp_data_size = bank_per_port * conv_icp;
    /*step 1. load aligned block*/
    for (int idx_iter = 0; idx_iter < load_iter; idx_iter++) {
      for (int idx_port = 0; idx_port < hp_port_num; idx_port++) {
        // cout<<"[idx_iter, idx_port]"<<idx_iter<<", "<<idx_port<<endl;
        for (int idx_stride = 0; idx_stride < stride; idx_stride++) {
          auto ddr_offset = (idx_iter * block_one_iter +
                             (idx_port * stride + idx_stride) * bank_per_port) *
                            one_kernel_size;
          auto block_inner_offset = 0;
          for (int idx_length = 0; idx_length < length_; idx_length++) {
            for (int idx_cg = 0; idx_cg < cg; idx_cg++) {
              vector<DPU_DATA_TYPE> hp_data(hp_data_size, 0);
              memcpy(hp_data.data(), buf + ddr_offset + block_inner_offset,
                     hp_data_size);
              if (debug_) {
                auto real_ddr_addr = ddr_offset + block_inner_offset;
                Util::SaveHexContSmallEndDDRAddr(
                    save_name, &buf[real_ddr_addr],
                    hp_data_size * sizeof(DPU_DATA_TYPE), hp_width_,
                    real_ddr_addr + ddr_addr_, reg_id_, SM_APPEND);
              }
              // cout<<" load from ddr_addr "<<ddr_offset +
              // block_inner_offset<<endl;
              block_inner_offset += hp_data_size;
              auto block_bank_addr =
                  (idx_iter * stride + idx_stride) * jump_write_endl_ +
                  idx_length * cg + idx_cg;
              for (int idx_b = 0; idx_b < bank_per_port; idx_b++) {
                auto block_bank_id =
                    bank_id_ +
                    (idx_port * bank_per_port + idx_b) % wgt_bank_num;
                auto bank =
                    Buffer<DPU_DATA_TYPE>::Instance().GetBank(block_bank_id);
                bank->Write((bank_addr_ + block_bank_addr) % bank_depth,
                            conv_icp, hp_data.data() + conv_icp * idx_b);
                // cout<<" write to [bank_id, bank_addr] "<<block_bank_id<<",
                // "<<(bank_addr_ + block_bank_addr)%bank_depth<<endl;
              }
            }
          }
        }
      }
    }
    /*step 2. load reminder*/
    // cout<<"load reminder"<<endl;
    int loaded_block = load_iter * block_one_iter;
    for (int idx_block = loaded_block; idx_block < block_num_;
         idx_block += bank_per_port) {
      auto ddr_offset = idx_block * one_kernel_size;
      auto block_bank_addr_base = (idx_block / wgt_bank_num) * jump_write_endl_;
      auto block_inner_offset = 0;
      for (int idx_length = 0; idx_length < length_; idx_length++) {
        for (int idx_cg = 0; idx_cg < cg; idx_cg++) {
          vector<DPU_DATA_TYPE> hp_data(hp_data_size, 0);
          memcpy(hp_data.data(), buf + ddr_offset + block_inner_offset,
                 hp_data_size);
          // cout<<" load from ddr_addr "<<ddr_offset +
          // block_inner_offset<<endl;
          if (debug_) {
            auto real_ddr_addr = ddr_offset + block_inner_offset;
            Util::SaveHexContSmallEndDDRAddr(
                save_name, &buf[real_ddr_addr],
                hp_data_size * sizeof(DPU_DATA_TYPE), hp_width_,
                real_ddr_addr + ddr_addr_, reg_id_, SM_APPEND);
          }
          block_inner_offset += hp_data_size;
          auto block_bank_addr =
              block_bank_addr_base + idx_length * cg + idx_cg;
          for (int idx_b = 0; idx_b < bank_per_port; idx_b++) {
            auto block_bank_id = bank_id_ + (idx_block + idx_b) % wgt_bank_num;
            auto bank =
                Buffer<DPU_DATA_TYPE>::Instance().GetBank(block_bank_id);
            bank->Write((bank_addr_ + block_bank_addr) % bank_depth, conv_icp,
                        hp_data.data() + conv_icp * idx_b);
            // cout<<" write to [bank_id, bank_addr] "<<block_bank_id<<",
            // "<<(bank_addr_ + block_bank_addr)%bank_depth<<endl;
          }
        }
      }
    }
#endif
#ifdef DDR_VISION_LLW
    int interlaced_block = load_iter * block_one_iter;
    for (int idx_block = 0; idx_block < block_num_; idx_block++) {
      /*step 1. find bank*/
      auto bank_id_offset = idx_block % wgt_bank_num;
      auto block_bank_id = bank_id_ + bank_id_offset;
      auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(block_bank_id);
      /*step 2. find this kernel's bank_addr*/
      auto bank_addr_offset = (idx_block / wgt_bank_num) * jump_write_endl_;
      auto block_bank_addr = (bank_addr_ + bank_addr_offset) % bank_depth;
      /*step 3. fill this kernel using ddr data*/
      vector<DPU_DATA_TYPE> kernel_data(one_kernel_size, 0);
      auto hp_iter = idx_block / block_one_iter;
      auto hp_ddr_base = hp_iter * block_one_iter * one_kernel_size;
      if (idx_block >=
          interlaced_block) { /*load reminder, kernel strored one by one*/
        hp_ddr_base += ((idx_block % block_one_iter) / bank_per_port) *
                       bank_per_port * one_kernel_size;
      } else {
        auto hp_port_id = (idx_block % wgt_bank_num) / hp_port_num;
        auto stride_offset = (idx_block % block_one_iter) / wgt_bank_num;
        hp_ddr_base += (hp_port_id * stride + stride_offset % stride) *
                       bank_per_port * one_kernel_size;
      }
      // cout<<"idx_block "<<idx_block <<", bank_id "<<block_bank_id
      //      <<", bank_addr "<< bank_addr_offset<<endl;
      for (int idx_length = 0; idx_length < length_; idx_length++) {
        for (int idx_line = 0; idx_line < cg; idx_line++) {
          auto kernel_inner_offset = (idx_length * cg + idx_line) * conv_icp;
          auto block_ddr_offset =
              hp_ddr_base + ((idx_length * cg + idx_line) * bank_per_port +
                             idx_block % bank_per_port) *
                                conv_icp;
          // cout<<"    [idx_len, idx_cg, ddr_addr]"<<idx_length<<",
          // "<<idx_line<<", "<<block_ddr_offset<<endl;
          memcpy(kernel_data.data() + kernel_inner_offset,
                 buf + block_ddr_offset, conv_icp);
        }
      }
      /*step 4. write kernel_data to bank*/
      bank->Write(block_bank_addr, one_kernel_size, kernel_data.data());
    }
#endif
  }
}

template <>
void Load<DPUVersion::XV2DPU>::Exec() {
  switch (mt_dst_) {
    case 0:
      load_img();
      load_ifm_PL();
      break;
    case 1:
      load_weights_conv();
      load_conv_wgts_PL();
      break;
    case 2:
      load_bias_conv();
      load_conv_bias_PL();
      break;
    case 3:
      load_weights_misc();
      load_misc_wgts_PL();
      break;
    case 4:
      load_bias_misc();
      load_misc_bias_PL();
      break;
    default:
      break;
  }
  load_gen_ctrlpkg();
}

template <>
void Load<DPUVersion::XV3DPU>::Exec() {
  switch (mt_dst_) {
    case 0:
      load_img();
      load_ifm_PL();
      break;
    case 1:
      load_weights_conv();
      load_conv_wgts_PL();
      break;
    case 2:
      load_bias_conv();
      load_conv_bias_PL();
      break;
    case 3:
      load_weights_misc();
      load_misc_wgts_PL();
      break;
    case 4:
      load_bias_misc();
      load_misc_bias_PL();
      break;
    case 5:
      load_layer_param();
      break;
    default:
      break;
  }
  load_gen_ctrlpkg();
}

template <DPUVersion T>
void Load<T>::load_block(const vector<bool>& pad_mask,
                         const int32_t block_ddr_addr,
                         const int32_t block_bank_id,
                         const int32_t block_bank_addr) {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, block_ddr_addr));
  UNI_LOG_CHECK(
      static_cast<uint64_t>(block_ddr_addr + (length_ - 1) * jump_read_ +
                            channel_) <= DDR::Instance().GetSize(reg_id_),
      SIM_OUT_OF_RANGE);
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(block_bank_id);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << block_bank_id << " out of range!" << endl;

  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      auto ddr_offset = idx_length * jump_read_ + idx_channel;
      auto pad_mask_idx = idx_length * channel_ + idx_channel;
      if (pad_mask[pad_mask_idx] == false) {
        data[idx_channel] = 0;
        continue;
      }

      // fetch data from ddr and do minus mean
      data[idx_channel] = buf[ddr_offset];
      if (mode_avg_ != 0) {
        uint8_t* orivalue =
            reinterpret_cast<uint8_t*>(data.data() + idx_channel);
        uint8_t* subvalue =
            reinterpret_cast<uint8_t*>(avg_reg_.data() + idx_channel);
        uint32_t delta = (*orivalue & 0x000003ff) - (*subvalue);
        delta = delta >> (mode_avg_ - 1);
        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
      }
    }
    auto bank_addr = block_bank_addr + idx_length * jump_write_;
    bank->Write(bank_addr, channel_, data.data());
  }
}

template <>
void Load<DPUVersion::DPU4F>::Exec() {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  if (quant_lth_)
    UNI_LOG_CHECK(
        static_cast<uint64_t>((length_ - 1) * jump_read_ + channel_) <=
            DDR::Instance().GetSize(reg_id_),
        SIM_OUT_OF_RANGE);
  else
    UNI_LOG_CHECK(
        static_cast<uint64_t>((length_ - 1) * jump_read_ + channel_ / 2) <=
            DDR::Instance().GetSize(reg_id_),
        SIM_OUT_OF_RANGE);
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;
  auto bank_width = bank->GetW();

  // initial avg_reg_ for minus mean
  if (mode_avg_ != 0) {
    auto mean_bank_group = ArchCfg::Instance()
                               .get_param()
                               .load_engine()
                               .meanvalue_reduction()
                               .meanvalue_bank();
    auto mean_bank =
        Buffer<DPU_DATA_TYPE>::Instance().GetBank(mean_bank_group, 0);
    mean_bank->Read(0, channel_, avg_reg_.data());
  }

  // set pad_mask for channel augment
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);
  if (channel_ >= jump_read_ && channel_ <= MODE_AVG_CHANNEL_LIMIT) {
    auto pad_start_num_this_pixel = 0;
    auto pad_end_num_this_pixel = 0;
    auto pad_end_start_idx = length_ - pad_idx_;
    for (auto idx_length = 0; idx_length < length_; idx_length++) {
      auto mask_idx = idx_length * channel_;
      pad_start_num_this_pixel = pad_start_ - idx_length * jump_read_;
      pad_start_num_this_pixel =
          pad_start_num_this_pixel > 0 ? pad_start_num_this_pixel : 0;
      UNI_LOG_CHECK(pad_start_num_this_pixel < channel_, SIM_OUT_OF_RANGE);
      for (auto idx_channel = 0; idx_channel < pad_start_num_this_pixel;
           idx_channel++) {
        pad_mask[mask_idx + idx_channel] = false;
      }
      if (idx_length >= pad_end_start_idx) {
        pad_end_num_this_pixel =
            pad_end_ + jump_read_ * (idx_length - pad_end_start_idx);
        auto pad_end_start_this_pixel =
            Util::AlignByN(channel_, bank_width) - pad_end_num_this_pixel;
        UNI_LOG_CHECK(pad_end_start_this_pixel > 0, SIM_OUT_OF_RANGE);
        for (int32_t idx_channel = pad_end_start_this_pixel;
             idx_channel < channel_; idx_channel++) {
          pad_mask[mask_idx + idx_channel] = false;
        }
      }
    }
  }

  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      auto ddr_offset = idx_length * jump_read_ + idx_channel;

      // pad mask
      if (pad_mask[ddr_offset] == false) {
        data[idx_channel] = 0;
        continue;
      }

      if (mode_avg_ != 0) {
        uint8_t* orivalue =
            reinterpret_cast<uint8_t*>(data.data() + idx_channel);
        uint8_t* subvalue =
            reinterpret_cast<uint8_t*>(avg_reg_.data() + idx_channel);
        uint32_t delta = (*orivalue & 0x000003ff) - (*subvalue);
        delta = delta >> (mode_avg_ - 1);
        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
      }
    }

    auto bank_addr = bank_addr_ + idx_length * jump_write_;
    auto ddr_offset = idx_length * jump_read_;
    auto ddr_pixel_num = (quant_lth_ | is_wgt_bank)
                             ? channel_
                             : (channel_ / 2 + (channel_ % 2 ? 1 : 0));
    auto data_out = separate(quant_lth_, buf + ddr_offset,
                             ddr_pixel_num);  // 8 bit -> 4 bit
    if (is_img_bank) {
      BankShell::write(quant_lth_, bank_id_, channel_, bank_addr,
                       data_out.data());
    } else if (is_wgt_bank) {
      BankShell::write(quant_lth_, bank_id_, channel_, bank_addr,
                       data_out.data());
      BankShell::write(quant_lth_, bank_id_ + 1, channel_, bank_addr,
                       data_out.data() + channel_);
    } else {
      auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
      bank->Write(bank_addr, channel_, data_out.data());
    }
  }
  debug_tick();
}

template <DPUVersion T>
void Load<T>::exec_mode_single_reg(const vector<bool>& pad_mask) {
  // get ddr handle
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  ddr_range_check(reg_id_);
  // get bank handle
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);

  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      auto ddr_offset = idx_length * jump_read_ + idx_channel;
      auto pad_mask_idx = idx_length * channel_ + idx_channel;
      if (pad_mask[pad_mask_idx] == false) {
        data[idx_channel] = 0;
        continue;
      }

      // fetch data from ddr and do minus mean
      data[idx_channel] = buf[ddr_offset];
      if (mode_avg_ != 0) {
        uint8_t orivalue = static_cast<uint8_t>(data[idx_channel]);
        uint8_t subvalue = static_cast<uint8_t>(avg_reg_[idx_channel]);
        int64_t delta = orivalue - subvalue;
        delta = delta / pow(2, (mode_avg_ - 1));
        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
      }
    }
    auto bank_addr = bank_addr_ + idx_length * jump_write_;
    bank->Write(bank_addr, channel_, data.data(), channel_offset_);
  }
  debug_tick_mode_single();
}

template <DPUVersion T>
void Load<T>::exec_mode_three_reg(const vector<bool>& pad_mask) {
  // get ddr handle
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  ddr_range_check(reg_id_);
  auto* buf_1 = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_1_, ddr_addr_1_));
  ddr_range_check(reg_id_1_);
  auto* buf_2 = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_2_, ddr_addr_2_));
  ddr_range_check(reg_id_2_);
  // get bank handle
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);

  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_, 0);
    vector<DPU_DATA_TYPE> data_1(channel_, 0);
    vector<DPU_DATA_TYPE> data_2(channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      auto ddr_offset = idx_length * jump_read_ + idx_channel;
      auto pad_mask_idx = idx_length * channel_ + idx_channel;
      if (pad_mask[pad_mask_idx] == false) {
        data[idx_channel] = 0;
        data_1[idx_channel] = 0;
        data_2[idx_channel] = 0;
        continue;
      }

      // fetch data from ddr and do minus mean
      data[idx_channel] = buf[ddr_offset];
      data_1[idx_channel] = buf_1[ddr_offset];
      data_2[idx_channel] = buf_2[ddr_offset];
      if (mode_avg_ != 0) {
        uint8_t orivalue = static_cast<uint8_t>(data[idx_channel]);
        uint8_t orivalue_1 = static_cast<uint8_t>(data_1[idx_channel]);
        uint8_t orivalue_2 = static_cast<uint8_t>(data_2[idx_channel]);

        uint8_t subvalue = static_cast<uint8_t>(avg_reg_[idx_channel]);
        int64_t delta = orivalue - subvalue;
        delta = delta / pow(2, (mode_avg_ - 1));
        int64_t delta_1 = orivalue_1 - subvalue;
        delta_1 = delta_1 / pow(2, (mode_avg_ - 1));
        int64_t delta_2 = orivalue_2 - subvalue;
        delta_2 = delta_2 / pow(2, (mode_avg_ - 1));

        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
        data_1[idx_channel] = static_cast<DPU_DATA_TYPE>(delta_1);
        data_2[idx_channel] = static_cast<DPU_DATA_TYPE>(delta_2);
      }
    }
    auto bank_addr = bank_addr_ + idx_length * jump_write_;
    vector<DPU_DATA_TYPE> data_concat(3 * channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      data_concat[idx_channel] = data[idx_channel];
      data_concat[idx_channel + channel_] = data_1[idx_channel];
      data_concat[idx_channel + channel_ * 2] = data_2[idx_channel];
    }
    // pad top
    uint64_t pad_top_start = pad_top_ * channel_;
    UNI_LOG_CHECK(pad_top_start < data_concat.size(), SIM_OUT_OF_RANGE)
        << "pad_top_ error, " << pad_top_ << endl;
    std::fill(data_concat.begin(), data_concat.begin() + pad_top_start, 0);
    // pad bottom
    uint64_t pad_bottom_start = pad_bottom_ * channel_;
    pad_bottom_start = pad_bottom_start >= data_concat.size()
                           ? data_concat.size()
                           : pad_bottom_start;
    UNI_LOG_CHECK(pad_bottom_start >= 0, SIM_PARAMETER_FAILED)
        << "pad_bottom_ error, " << pad_bottom_ << endl;
    std::fill(data_concat.begin() + pad_bottom_start, data_concat.end(), 0);
    //

    bank->Write(bank_addr, data_concat.size(), data_concat.data());
  }
  debug_tick_mode_three();
}

template <DPUVersion T>
void Load<T>::debug_tick_mode_single() {
  if (!debug_) return;

  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto* buf =
      reinterpret_cast<DPU_DATA_TYPE*>(DDR::Instance().GetAddr(reg_id_, 0));

  auto ddr_offset = ddr_addr_;
  for (int i = 0; i < length_; i++) {
    Util::SaveHexContSmallEndDDRAddr(save_name, &buf[ddr_offset],
                                     channel_ * sizeof(DPU_DATA_TYPE),
                                     hp_width_, ddr_offset, reg_id_, SM_APPEND);
    ddr_offset += jump_read_;
  }
}

template <DPUVersion T>
void Load<T>::debug_tick_mode_three() {
  if (!debug_) return;

  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto* buf_ = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  auto* buf_1 = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_1_, ddr_addr_1_));
  auto* buf_2 = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_2_, ddr_addr_2_));
  for (int i = 0; i < length_; i++) {
    // line 0
    auto ddr_offset = i * jump_read_;
    Util::SaveHexContSmallEndDDRAddr(save_name, &buf_[ddr_offset],
                                     channel_ * sizeof(DPU_DATA_TYPE),
                                     hp_width_, ddr_offset, reg_id_, SM_APPEND);
    Util::SaveHexContSmallEndDDRAddr(
        save_name, &buf_1[ddr_offset], channel_ * sizeof(DPU_DATA_TYPE),
        hp_width_, ddr_offset, reg_id_1_, SM_APPEND);
    Util::SaveHexContSmallEndDDRAddr(
        save_name, &buf_2[ddr_offset], channel_ * sizeof(DPU_DATA_TYPE),
        hp_width_, ddr_offset, reg_id_2_, SM_APPEND);
  }
}

template <DPUVersion T>
vector<DPU_DATA_TYPE> Load<T>::separate(bool is_8_bit, DPU_DATA_TYPE* data,
                                        int32_t num) {
  auto buf_size = is_8_bit ? num : 2 * num;
  // cout << "channel: " << num << " data_size: " << buf_size << endl;
  vector<DPU_DATA_TYPE> data_out(buf_size);

  for (auto idx_data = 0; idx_data < num; idx_data++) {
    if (is_8_bit)
      data_out.at(idx_data) = data[idx_data];
    else {
      auto tmp = data[idx_data];
      auto idx_lo = is_img_bank ? 2 * idx_data : idx_data;
      auto idx_hi = is_img_bank ? 2 * idx_data + 1 : num + idx_data;
      data_out.at(idx_lo) = static_cast<DPU_DATA_TYPE>(0x0F & tmp);
      data_out.at(idx_hi) = static_cast<DPU_DATA_TYPE>(tmp >> 4);
      if ((0x08 & data_out.at(idx_lo)) == 0x08)
        data_out.at(idx_lo) =
            static_cast<DPU_DATA_TYPE>(0xF0 | data_out.at(idx_lo));
      if (is_img_bank && (idx_data * 2 > channel_)) data_out.at(idx_hi) = 0;
      // cout << "data[" << idx_lo << "]: " << hex << +data_out.at(idx_lo)
      //     << " data[" << idx_hi << "] : " << +data_out.at(idx_hi) << endl;
    }
  }
  return data_out;
}

template <>
void Load<DPUVersion::DPU4F>::debug_tick() {
  if (!debug_) return;

  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto channel = ((!quant_lth_) && is_img_bank)
                     ? (channel_ / 2 + (channel_ % 2 ? 1 : 0))
                     : channel_;
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));

  for (int i = 0; i < length_; i++) {
    auto ddr_offset = i * jump_read_;
    Util::SaveHexContSmallEndDDRAddr(
        save_name, &buf[ddr_offset], channel * sizeof(DPU_DATA_TYPE), hp_width_,
        ddr_addr_ + ddr_offset, reg_id_, SM_APPEND);
  }
}

template <DPUVersion T>
void Load<T>::debug_tick() {
  if (!debug_) return;

  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  //  cout << "inst_id: " << instid_ << " reg_id: " << reg_id_
  //       << " length: " << length_ << " channel: " << channel_ << endl;
  for (int i = 0; i < length_; i++) {
    auto ddr_offset = i * jump_read_;
    Util::SaveHexContSmallEndDDRAddr(
        save_name, &buf[ddr_offset], channel_ * sizeof(DPU_DATA_TYPE),
        hp_width_, ddr_addr_ + ddr_offset, reg_id_, SM_APPEND);
  }
}

template <>
void Load<DPUVersion::DPUV4E>::debug_tick() {
  if (!debug_) return;

  string save_name_input = debug_path_ + "input_inst_" + to_string(instid_) +
                           "tick." + to_string(instid_classify_);
  string save_name_output = debug_path_ + "output_inst_" + to_string(instid_) +
                            "tick." + to_string(instid_classify_);
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  auto block_num = (bank_id_ == 0xFF) ? output_channel_num_ : 1;
  auto wgt_bank_group =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  auto start_id = (bank_id_ == 0xFF) ? Buffer<DPU_DATA_TYPE>::Instance()
                                           .GetBank(wgt_bank_group, 0)
                                           ->GetID()
                                     : bank_id_;
  auto bank_depth = Buffer<DPU_DATA_TYPE>::Instance().GetBank(start_id)->GetH();
  auto bank_width = Buffer<DPU_DATA_TYPE>::Instance().GetBank(start_id)->GetW();

  for (auto idx_block = 0; idx_block < block_num; idx_block++) {
    auto bank_id = start_id + idx_block;
    auto group = idx_block / 16;
    auto stride_id =
        (group < 4) ? (group * 16 + idx_block) : (group * 16 - 112 + idx_block);

    for (auto idx_length = 0; idx_length < length_; idx_length++) {
      auto ddr_offset = stride_id * jump_read_endl_ + idx_length * jump_read_;
      Util::SaveHexContSmallEndDDRAddr(
          save_name_input, &buf[ddr_offset], channel_ * sizeof(DPU_DATA_TYPE),
          hp_width_, ddr_addr_ + ddr_offset, reg_id_, SM_APPEND);

      auto bank_addr = bank_addr_ + idx_length * jump_write_;
      vector<DPU_DATA_TYPE> bank_buf(channel_);
      auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id);
      bank->Read(bank_addr, channel_, bank_buf.data());

      Util::SaveHexContSmallEndBankAddr(
          save_name_output, reinterpret_cast<const char*>(bank_buf.data()),
          channel_ * sizeof(DPU_DATA_TYPE), bank_width * sizeof(DPU_DATA_TYPE),
          bank_depth, bank_id, bank_addr, SM_APPEND);
    }
  }
}

template <DPUVersion T>
void Load<T>::debug_tick(const int32_t ddr_addr) {
  if (!debug_) return;

  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr));
  for (int i = 0; i < length_; i++) {
    auto ddr_offset = i * jump_read_;
    Util::SaveHexContSmallEndDDRAddr(
        save_name, &buf[ddr_offset], channel_ * sizeof(DPU_DATA_TYPE),
        hp_width_, ddr_addr + ddr_offset, reg_id_, SM_APPEND);
  }
}

template <DPUVersion T>
void Load<T>::gen_PadMask(vector<bool>& pad_mask, int bank_width) {
  size_t ll = length_ * channel_;
  UNI_LOG_CHECK(ll == pad_mask.size(), SIM_OUT_OF_RANGE);
  if (channel_ >= jump_read_ && channel_ <= MODE_AVG_CHANNEL_LIMIT) {
    auto pad_start_num_this_pixel = 0;
    auto pad_end_num_this_pixel = 0;
    auto pad_end_start_idx = length_ - pad_idx_;
    for (auto idx_length = 0; idx_length < length_; idx_length++) {
      auto mask_idx = idx_length * channel_;
      pad_start_num_this_pixel = pad_start_ - idx_length * jump_read_;
      pad_start_num_this_pixel =
          pad_start_num_this_pixel > 0 ? pad_start_num_this_pixel : 0;
      UNI_LOG_CHECK(pad_start_num_this_pixel < channel_, SIM_OUT_OF_RANGE);
      for (auto idx_channel = 0; idx_channel < pad_start_num_this_pixel;
           idx_channel++) {
        pad_mask[mask_idx + idx_channel] = false;
      }
      if (idx_length >= pad_end_start_idx) {
        pad_end_num_this_pixel =
            pad_end_ + jump_read_ * (idx_length - pad_end_start_idx);
        auto pad_end_start_this_pixel =
            Util::AlignByN(channel_, bank_width) - pad_end_num_this_pixel;
        UNI_LOG_CHECK(pad_end_start_this_pixel > 0, SIM_OUT_OF_RANGE);
        for (int32_t idx_channel = pad_end_start_this_pixel;
             idx_channel < channel_; idx_channel++) {
          pad_mask[mask_idx + idx_channel] = false;
        }
      }
    }
  }
}

template <DPUVersion T>
void Load<T>::load_and_minusmean(DPU_DATA_TYPE* buf,
                                 shared_ptr<Bank<DPU_DATA_TYPE>>& bank,
                                 const vector<bool>& pad_mask,
                                 int32_t bank_addr_offset) {
  for (int idx_length = 0; idx_length < length_; idx_length++) {
    vector<DPU_DATA_TYPE> data(channel_, 0);
    for (auto idx_channel = 0; idx_channel < channel_; idx_channel++) {
      if (const_en_) {
        data[idx_channel] = const_value_;
        continue;
      }

      auto pad_mask_idx = idx_length * channel_ + idx_channel;
      if (pad_mask[pad_mask_idx] == false) {
        data[idx_channel] = 0;
        continue;
      }

      // fetch data from ddr and do minus mean
      auto ddr_offset = idx_length * jump_read_ + idx_channel;
      DDR::Instance().set_addr_used(reg_id_, ddr_addr_ + ddr_offset);
      data[idx_channel] = buf[ddr_offset];
      if (mode_avg_ != 0) {
        uint8_t* orivalue =
            reinterpret_cast<uint8_t*>(data.data() + idx_channel);
        uint8_t* subvalue =
            reinterpret_cast<uint8_t*>(avg_reg_.data() + idx_channel);
        uint32_t delta = (*orivalue & 0x000003ff) - (*subvalue);
        delta = delta >> (mode_avg_ - 1);
        data[idx_channel] = static_cast<DPU_DATA_TYPE>(delta);
      }
    }
    auto bank_addr = bank_addr_ + idx_length * jump_write_ + bank_addr_offset;
    bank->Write(bank_addr, channel_, data.data());
  }
}

template <DPUVersion T>
void Load<T>::space2depth() {
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  auto bank_width = bank->GetW();

  auto channel = jump_write_ * bank_width;

  auto read_bank = vector<DPU_DATA_TYPE>(block_num_ * length_ * channel);
  auto* read_bank_ptr = reinterpret_cast<char*>(read_bank.data());
  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    auto bank_addr_per_block = bank_addr_ + idx_block * jump_write_endl_;
    auto ddr_addr_per_block = idx_block * length_ * channel;
    for (auto idx_length = 0; idx_length < length_; ++idx_length) {
      auto bank_addr = bank_addr_per_block + idx_length * jump_write_;
      auto ddr_addr = ddr_addr_per_block + idx_length * channel;
      bank->Read(bank_addr, channel, (read_bank_ptr + ddr_addr));
    }
  }

  auto sub_oc = channel / tile_stride_h_ / tile_stride_w_;
  auto jump_write = sub_oc / bank_width;
  auto jump_write_endl = jump_write * length_ * tile_stride_w_;
  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    auto idx_oh = idx_block / tile_stride_h_;
    for (auto idx_length = 0; idx_length < length_; ++idx_length) {
      auto idx_ow = idx_length / tile_stride_w_;

      auto idx_ocg = (idx_block % tile_stride_h_) * tile_stride_w_ +
                     (idx_length % tile_stride_w_);
      auto idx_oc = idx_ocg * sub_oc;

      auto ddr_addr =
          idx_block * length_ * channel + idx_length * channel + idx_oc;
      auto bank_addr = idx_oh * jump_write_endl + idx_ow * jump_write;

      bank->Write(bank_addr, sub_oc, read_bank.data() + ddr_addr);
    }
  }
}

template <DPUVersion T>
void Load<T>::depth2space() {
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  auto bank_width = bank->GetW();

  auto channel = jump_write_ * bank_width;

  auto read_bank = vector<DPU_DATA_TYPE>(block_num_ * length_ * channel);
  auto* read_bank_ptr = reinterpret_cast<char*>(read_bank.data());
  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    auto bank_addr_per_block = bank_addr_ + idx_block * jump_write_endl_;
    auto ddr_addr_per_block = idx_block * length_ * channel;
    for (auto idx_length = 0; idx_length < length_; ++idx_length) {
      auto bank_addr = bank_addr_per_block + idx_length * jump_write_;
      auto ddr_addr = ddr_addr_per_block + idx_length * channel;
      bank->Read(bank_addr, channel, (read_bank_ptr + ddr_addr));
    }
  }

  auto oh = block_num_ * tile_stride_h_;
  auto ow = length_ * tile_stride_w_;
  auto oc = channel / tile_stride_h_ / tile_stride_w_;
  auto jump_write = oc / bank_width;
  auto jump_write_endl = jump_write * ow;
  for (auto idx_oh = 0U; idx_oh < oh; ++idx_oh) {
    auto ddr_addr_per_h = (idx_oh / tile_stride_h_) * length_ * channel +
                          (idx_oh % tile_stride_h_) * tile_stride_w_ * oc;
    auto bank_addr_per_h = bank_addr_ + idx_oh * jump_write_endl;
    for (auto idx_ow = 0U; idx_ow < ow; ++idx_ow) {
      auto ddr_addr = ddr_addr_per_h + (idx_ow / tile_stride_w_) * channel +
                      (idx_ow % tile_stride_w_) * oc;
      auto bank_addr = bank_addr_per_h + idx_ow * jump_write;
      bank->Write(bank_addr, channel, read_bank.data() + ddr_addr);
    }
  }
}

template <DPUVersion T>
void Load<T>::upsample() {
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  auto bank_width = bank->GetW();
  auto channel = jump_write_ * bank_width;
  auto read_bank = vector<DPU_DATA_TYPE>(block_num_ * length_ * channel);
  auto* read_bank_ptr = reinterpret_cast<char*>(read_bank.data());

  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    auto bank_addr_per_block = bank_addr_ + idx_block * jump_write_endl_;
    auto ddr_addr_per_block = idx_block * length_ * channel;
    for (auto idx_length = 0; idx_length < length_; ++idx_length) {
      auto bank_addr = bank_addr_per_block + idx_length * jump_write_;
      auto ddr_addr = ddr_addr_per_block + idx_length * channel;
      bank->Read(bank_addr, channel, (read_bank_ptr + ddr_addr));
    }
  }

  auto oh = block_num_ * sample_scale_h_;
  auto ow = length_ * sample_scale_w_;
  auto jump_write = jump_write_;
  auto jump_write_endl = jump_write * ow;
  for (auto idx_oh = 0U; idx_oh < oh; ++idx_oh) {
    auto ddr_addr_per_h = (idx_oh / sample_scale_h_) * length_ * channel;
    auto bank_addr_per_h = bank_addr_ + idx_oh * jump_write_endl;
    for (auto idx_ow = 0U; idx_ow < ow; ++idx_ow) {
      auto ddr_addr = ddr_addr_per_h + (idx_ow / sample_scale_w_) * channel;
      auto bank_addr = bank_addr_per_h + idx_ow * jump_write;
      bank->Write(bank_addr, channel, read_bank_ptr + ddr_addr);
    }
  }
}

template <DPUVersion T>
void Load<T>::downsample() {
    auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  auto bank_width = bank->GetW();
  auto channel = jump_write_ * bank_width;
  auto read_bank = vector<DPU_DATA_TYPE>(block_num_ * length_ * channel);
  auto* read_bank_ptr = reinterpret_cast<char*>(read_bank.data());

  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    auto bank_addr_per_block = bank_addr_ + idx_block * jump_write_endl_;
    auto ddr_addr_per_block = idx_block * length_ * channel;
    for (auto idx_length = 0; idx_length < length_; ++idx_length) {
      auto bank_addr = bank_addr_per_block + idx_length * jump_write_;
      auto ddr_addr = ddr_addr_per_block + idx_length * channel;
      bank->Read(bank_addr, channel, (read_bank_ptr + ddr_addr));
    }
  }

  auto oh = block_num_ / sample_scale_h_;
  auto ow = length_ / sample_scale_w_;
  auto jump_write = jump_write_;
  auto jump_write_endl = jump_write * ow;
  for (auto idx_oh = 0U; idx_oh < oh; ++idx_oh) {
    auto ddr_addr_per_h = (idx_oh * sample_scale_h_) * length_ * channel;
    auto bank_addr_per_h = bank_addr_ + idx_oh * jump_write_endl;
    for (auto idx_ow = 0U; idx_ow < ow; ++idx_ow) {
      auto ddr_addr = ddr_addr_per_h + (idx_ow * sample_scale_w_) * channel;
      auto bank_addr = bank_addr_per_h + idx_ow * jump_write;
      bank->Write(bank_addr, channel, read_bank_ptr + ddr_addr);
    }
  }
}

template <DPUVersion T>
void Load<T>::target_check() {
  // bank id check
  auto white_list = ArchCfg::Instance().get_bank_access_white_list("load-out");
  UNI_LOG_CHECK((white_list.count(bank_id_) > 0), SIM_OUT_OF_RANGE)
      << "bank_id (= " << bank_id_ << ") does not match target!" << endl;
}

template <DPUVersion T>
void Load<T>::GenInstStr(int inst_fmt) {
  ac_ = Helper<T>::GetInstStr(inst_type_, inst_fmt, dpdon_, dpdby_, field_str_);
}

template <DPUVersion T>
void Load<T>::ddr_range_check(int32_t reg_id) {
  UNI_LOG_CHECK_LE(
      static_cast<uint64_t>(ddr_addr_ + (length_ - 1) * jump_read_ + channel_),
      DDR::Instance().GetSize(reg_id_), SIM_OUT_OF_RANGE)
      << "[DDR address segment fault] "
      << UNI_LOG_VALUES(ddr_addr_, length_, jump_read_, channel_, reg_id_);
}

template <>
void Load<DPUVersion::XV2DPU>::load_img() {
  UNI_LOG_CHECK(block_num_ == 1, SIM_OUT_OF_RANGE)
      << "Now bank_id = " << bank_id_
      << ". It is not a weights bank, so long load is unsupported. "
         "Block_num shoud be 1.";

  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;

  // set pad_mask for channel augment
  auto bank_width = bank->GetW();
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);
  gen_PadMask(pad_mask, bank_width);

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  ddr_range_check(reg_id_);

  load_and_minusmean(buf, bank, pad_mask);

  debug_tick();
}

template <>
void Load<DPUVersion::XV3DPU>::load_img() {
  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  UNI_LOG_CHECK(bank != nullptr, SIM_OUT_OF_RANGE)
      << "bank_id " << bank_id_ << " out of range!" << endl;

  // set pad_mask for channel augment
  auto bank_width = bank->GetW();
  int ll = length_ * channel_;
  vector<bool> pad_mask(ll, true);
  gen_PadMask(pad_mask, bank_width);

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  ddr_range_check(reg_id_);

  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    load_and_minusmean(buf + idx_block * jump_read_endl_, bank, pad_mask,
                       idx_block * jump_write_endl_);
  }

  switch (trans_mode_) {
    case 0:
      break;
    case 1:
      space2depth();
      break;
    case 2:
      depth2space();
      break;
    case 4:
      upsample();
      break;
    case 5:
      downsample();
      break;
    case 3:
    default:
      UNI_LOG_CHECK(0, SIM_OUT_OF_RANGE)
          << "invalid trans_mode " << trans_mode_;
      break;
  }

  for (auto idx_block = 0; idx_block < block_num_; ++idx_block) {
    debug_tick(ddr_addr_ + idx_block * jump_read_endl_);
  }
}

template <>
void Load<DPUVersion::XV2DPU>::load_weights_conv() {
  auto conv_ocp =
      ArchCfg::Instance().get_param().conv_engine().output_channel_parallel();
  auto conv_icp =
      ArchCfg::Instance().get_param().conv_engine().input_channel_parallel();
  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().weight_bank();
  auto bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(wgt_bank_group_name);
  auto bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(wgt_bank_group_name);
  auto wgt_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(wgt_bank_group_name);
  UNI_LOG_CHECK(conv_icp == 32, SIM_OUT_OF_RANGE)
      << "icp is 32. Now icp = " << conv_icp;
  UNI_LOG_CHECK(conv_ocp == 32, SIM_OUT_OF_RANGE)
      << "ocp is 32. Now ocp = " << conv_ocp;
  UNI_LOG_CHECK(bank_width == 16, SIM_OUT_OF_RANGE)
      << "bank_width is 16. Now bank_width = " << bank_width;
  UNI_LOG_CHECK(wgt_bank_num == 4, SIM_OUT_OF_RANGE)
      << "wgt_bank_num is 4. Now wgt_bank_num = " << wgt_bank_num;

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  int icg = div_ceil(channel_, conv_icp);
  int ocg = div_ceil(block_num_, conv_ocp);
  int ddr_block_size = conv_icp * 1 * 1 * conv_ocp;
  int fetch_cycles_per_block = ddr_block_size / bank_width / wgt_bank_num;
  UNI_LOG_CHECK_EQ(ddr_block_size % (wgt_bank_num * bank_width), 0,
                   SIM_ALIGN_ERROR)
      << UNI_LOG_VALUES(ddr_block_size, wgt_bank_num, bank_width);
  UNI_LOG_CHECK_EQ(fetch_cycles_per_block * length_, jump_write_,
                   SIM_ALIGN_ERROR)
      << UNI_LOG_VALUES(fetch_cycles_per_block, length_, jump_write_);
  UNI_LOG_CHECK_EQ(fetch_cycles_per_block * icg * length_, jump_write_endl_,
                   SIM_ALIGN_ERROR)
      << UNI_LOG_VALUES(fetch_cycles_per_block, icg, length_, jump_write_endl_);

  for (int idx_ocg = 0; idx_ocg < ocg; ++idx_ocg) {
    for (int idx_icg = 0; idx_icg < icg; ++idx_icg) {
      for (int idx_lenght = 0; idx_lenght < length_; ++idx_lenght) {
        int idx_block_num =
            idx_ocg * icg * length_ + idx_icg * length_ + idx_lenght;

        for (int idx_cycle = 0; idx_cycle < fetch_cycles_per_block;
             ++idx_cycle) {
          for (int idx_bank = 0; idx_bank < wgt_bank_num; ++idx_bank) {
            vector<DPU_DATA_TYPE> wgt_buffer(bank_width, 0);
            auto ddr_addr_offset = idx_block_num * ddr_block_size +
                                   idx_cycle * bank_width * wgt_bank_num +
                                   idx_bank * bank_width;
            memcpy(wgt_buffer.data(), buf + ddr_addr_offset, bank_width);
            auto bank_addr_offset =
                idx_ocg * jump_write_endl_ + idx_icg * jump_write_ +
                idx_lenght * fetch_cycles_per_block + idx_cycle;
            auto bank =
                Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);
            bank->Write((bank_addr_ + bank_addr_offset) % bank_depth,
                        bank_width, wgt_buffer.data());
            if (debug_) {
              Util::SaveHexContSmallEndBankAddr(
                  save_name, wgt_buffer.data(), bank_width, bank_width,
                  bank_depth, bank_id_ + idx_bank,
                  (bank_addr_ + bank_addr_offset) % bank_depth, SM_APPEND);
            }
          }
        }
      }
    }
  }
}

template <>
void Load<DPUVersion::XV3DPU>::load_weights_conv() {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto data_size = block_num_ * length_ * channel_;
  vector<DPU_DATA_TYPE> wgt_buffer(data_size, 0);
  memcpy(wgt_buffer.data(), buf, data_size);

  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  bank->Write(bank_addr_, data_size, wgt_buffer.data());

  if (debug_) {
    auto weights_bank_group_name =
        ArchCfg::Instance().get_param().conv_engine().weight_bank();
    auto bank_width =
        Buffer<DPU_DATA_TYPE>::Instance().GetBankW(weights_bank_group_name);
    auto bank_depth =
        Buffer<DPU_DATA_TYPE>::Instance().GetBankH(weights_bank_group_name);
    Util::SaveHexContBigEndBankAddr(save_name, wgt_buffer.data(), data_size,
                                    bank_width, bank_depth, bank_id_,
                                    bank_addr_, SM_APPEND);
  }
}

template <>
void Load<DPUVersion::XV2DPU>::load_bias_conv() {
  auto conv_ocp =
      ArchCfg::Instance().get_param().conv_engine().output_channel_parallel();
  auto bias_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().bias_bank();
  auto bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(bias_bank_group_name);
  auto bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(bias_bank_group_name);
  auto bias_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(bias_bank_group_name);
  UNI_LOG_CHECK(block_num_ == 1, SIM_OUT_OF_RANGE);
  UNI_LOG_CHECK(conv_ocp == 32, SIM_OUT_OF_RANGE)
      << "ocp is 32. Now ocp = " << conv_ocp;
  UNI_LOG_CHECK(bank_width == 16, SIM_OUT_OF_RANGE)
      << "bank_width is 16. Now bank_width = " << bank_width;
  UNI_LOG_CHECK(bias_bank_num == 4, SIM_OUT_OF_RANGE)
      << "bias_bank_num is 4. Now bias_bank_num = " << bias_bank_num;
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  int output_channel = channel_;
  int ocg = div_ceil(output_channel, conv_ocp);
  int ddr_block_size = 256;
  int ddr_block_num = ocg;
  int fetch_cycles_per_block = ddr_block_size / bank_width / bias_bank_num;
  UNI_LOG_CHECK_EQ(ddr_block_size % (bias_bank_num * bank_width), 0,
                   SIM_ALIGN_ERROR)
      << UNI_LOG_VALUES(ddr_block_size, bias_bank_num, bank_width);

  for (int idx_block_num = 0; idx_block_num < ddr_block_num; ++idx_block_num) {
    for (int idx_cycle = 0; idx_cycle < fetch_cycles_per_block; ++idx_cycle) {
      for (int idx_bank = 0; idx_bank < bias_bank_num; ++idx_bank) {
        vector<DPU_DATA_TYPE> bias_buffer(bank_width, 0);
        auto ddr_addr_offset = idx_block_num * ddr_block_size +
                               idx_cycle * bank_width * bias_bank_num +
                               idx_bank * bank_width;
        memcpy(bias_buffer.data(), buf + ddr_addr_offset, bank_width);
        auto bank_addr_offset = idx_block_num * jump_write_ + idx_cycle;
        auto bank =
            Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);
        bank->Write((bank_addr_ + bank_addr_offset) % bank_depth, bank_width,
                    bias_buffer.data());
        if (debug_) {
          Util::SaveHexContSmallEndBankAddr(
              save_name, bias_buffer.data(), bank_width, bank_width, bank_depth,
              bank_id_ + idx_bank, bank_addr_ + bank_addr_offset, SM_APPEND);
        }
      }
    }
  }
}

template <>
void Load<DPUVersion::XV3DPU>::load_bias_conv() {
  auto conv_ocp =
      ArchCfg::Instance().get_param().conv_engine().output_channel_parallel();
  auto bias_bank_group_name =
      ArchCfg::Instance().get_param().conv_engine().bias_bank();
  auto bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(bias_bank_group_name);
  auto bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(bias_bank_group_name);
  auto bias_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(bias_bank_group_name);
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  int output_channel = channel_;
  int ocg = div_ceil(output_channel, conv_ocp);
  int ddr_block_size = 256;
  int ddr_block_num = ocg;
  int fetch_cycles_per_block = ddr_block_size / bank_width / bias_bank_num;
  UNI_LOG_CHECK_EQ(ddr_block_size % (bias_bank_num * bank_width), 0,
                   SIM_ALIGN_ERROR)
      << UNI_LOG_VALUES(ddr_block_size, bias_bank_num, bank_width);

  for (int idx_block_num = 0; idx_block_num < ddr_block_num; ++idx_block_num) {
    for (int idx_cycle = 0; idx_cycle < fetch_cycles_per_block; ++idx_cycle) {
      for (int idx_bank = 0; idx_bank < bias_bank_num; ++idx_bank) {
        vector<DPU_DATA_TYPE> bias_buffer(bank_width, 0);
        auto ddr_addr_offset = idx_block_num * ddr_block_size +
                               idx_cycle * bank_width * bias_bank_num +
                               idx_bank * bank_width;
        memcpy(bias_buffer.data(), buf + ddr_addr_offset, bank_width);
        auto bank_addr_offset = idx_block_num * jump_write_ + idx_cycle;
        auto bank =
            Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);
        bank->Write((bank_addr_ + bank_addr_offset) % bank_depth, bank_width,
                    bias_buffer.data());
        if (debug_) {
          Util::SaveHexContSmallEndBankAddr(
              save_name, bias_buffer.data(), bank_width, bank_width, bank_depth,
              bank_id_ + idx_bank, bank_addr_ + bank_addr_offset, SM_APPEND);
        }
      }
    }
  }
}

template <>
void Load<DPUVersion::XV2DPU>::load_weights_misc() {
  int conv_ocp =
      ArchCfg::Instance().get_param().alu_engine().channel_parallel();
  auto wgt_bank_group_name =
      ArchCfg::Instance().get_param().alu_engine().weight_bank();
  int bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(wgt_bank_group_name);
  int bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(wgt_bank_group_name);
  auto wgt_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(wgt_bank_group_name);
  UNI_LOG_CHECK_EQ(block_num_, 1, SIM_OUT_OF_RANGE);
  UNI_LOG_CHECK(conv_ocp == 32, SIM_OUT_OF_RANGE)
      << "ocp is 32. Now ocp = " << conv_ocp;
  UNI_LOG_CHECK(bank_width == 16, SIM_OUT_OF_RANGE)
      << "bank_width is 16. Now bank_width = " << bank_width;
  UNI_LOG_CHECK(wgt_bank_num == 4, SIM_OUT_OF_RANGE)
      << "wgt_bank_num is 4. Now wgt_bank_num = " << wgt_bank_num;
  UNI_LOG_CHECK_EQ(length_, jump_write_, SIM_ALIGN_ERROR);

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";
  int output_channel = channel_;
  int ocg = div_ceil(output_channel, conv_ocp);
  UNI_LOG_CHECK(conv_ocp == 2 * bank_width, SIM_OUT_OF_RANGE);

  for (int idx_ocg = 0; idx_ocg < ocg; ++idx_ocg) {
    for (int idx_length = 0; idx_length < length_; ++idx_length) {
      for (int idx_bank = 0; idx_bank < wgt_bank_num; ++idx_bank) {
        std::vector<char> wgt_buffer(bank_width, 0);
        auto bank =
            Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);

        if (idx_bank < 2) {
          auto ddr_addr_offset = idx_ocg * length_ * wgt_bank_num * bank_width +
                                 idx_length * wgt_bank_num * bank_width +
                                 idx_bank * bank_width;
          memcpy(wgt_buffer.data(), buf + ddr_addr_offset, bank_width);
        }
        auto bank_addr_offset = idx_ocg * jump_write_ + idx_length;
        bank->Write((bank_addr_ + bank_addr_offset) % bank_depth, bank_width,
                    wgt_buffer.data());
        if (debug_) {
          Util::SaveHexContSmallEndBankAddr(
              save_name, wgt_buffer.data(), bank_width, bank_width, bank_depth,
              bank_id_ + idx_bank, bank_addr_ + bank_addr_offset, SM_APPEND);
        }
      }
    }
  }
}

template <>
void Load<DPUVersion::XV3DPU>::load_weights_misc() {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto data_size = block_num_ * length_ * channel_;
  vector<DPU_DATA_TYPE> wgt_buffer(data_size, 0);
  memcpy(wgt_buffer.data(), buf, data_size);

  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  bank->Write(bank_addr_, data_size, wgt_buffer.data());

  if (debug_) {
    Util::SaveHexContSmallEndBankAddr(save_name, wgt_buffer.data(), data_size,
                                      bank->GetW(), bank->GetH(), bank_id_,
                                      bank_addr_, SM_APPEND);
  }
}

template <>
void Load<DPUVersion::XV2DPU>::load_bias_misc() {
  auto conv_ocp =
      ArchCfg::Instance().get_param().alu_engine().channel_parallel();
  auto bias_bank_group_name =
      ArchCfg::Instance().get_param().alu_engine().bias_bank();
  auto bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(bias_bank_group_name);
  auto bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(bias_bank_group_name);
  auto bias_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(bias_bank_group_name);
  UNI_LOG_CHECK(block_num_ == 1, SIM_OUT_OF_RANGE);
  UNI_LOG_CHECK(conv_ocp == 32, SIM_OUT_OF_RANGE)
      << "ocp is 32. Now ocp = " << conv_ocp;
  UNI_LOG_CHECK(bank_width == 16, SIM_OUT_OF_RANGE)
      << "bank_width is 16. Now bank_width = " << bank_width;
  UNI_LOG_CHECK(bias_bank_num == 4, SIM_OUT_OF_RANGE)
      << "bias_bank_num is 4. Now bias_bank_num = " << bias_bank_num;

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  int output_channel = channel_;
  int ocg = div_ceil(output_channel, conv_ocp);
  int ddr_block_size = 64;
  int ddr_block_num = ocg;

  for (int idx_block_num = 0; idx_block_num < ddr_block_num; ++idx_block_num) {
    for (int idx_bank = 0; idx_bank < bias_bank_num; ++idx_bank) {
      vector<DPU_DATA_TYPE> bias_buffer(bank_width, 0);
      auto ddr_addr_offset =
          idx_block_num * ddr_block_size + idx_bank * bank_width;
      auto bank_addr_offset = idx_block_num * jump_write_;

      memcpy(bias_buffer.data(), buf + ddr_addr_offset, bank_width);
      auto bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);
      bank->Write((bank_addr_ + bank_addr_offset) % bank_depth, bank_width,
                  bias_buffer.data());
      if (debug_) {
        Util::SaveHexContSmallEndBankAddr(
            save_name, bias_buffer.data(), bank_width, bank_width, bank_depth,
            bank_id_ + idx_bank, bank_addr_ + bank_addr_offset, SM_APPEND);
      }
    }
  }
}

template <>
void Load<DPUVersion::XV3DPU>::load_bias_misc() {
  auto conv_ocp =
      ArchCfg::Instance().get_param().alu_engine().channel_parallel();
  auto bias_bank_group_name =
      ArchCfg::Instance().get_param().alu_engine().bias_bank();
  auto bank_width =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankW(bias_bank_group_name);
  auto bank_depth =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankH(bias_bank_group_name);
  auto bias_bank_num =
      Buffer<DPU_DATA_TYPE>::Instance().GetBankNum(bias_bank_group_name);
  UNI_LOG_CHECK(block_num_ == 1, SIM_OUT_OF_RANGE);
  UNI_LOG_CHECK(conv_ocp == 32, SIM_OUT_OF_RANGE)
      << "ocp is 32. Now ocp = " << conv_ocp;
  UNI_LOG_CHECK(bank_width == 16, SIM_OUT_OF_RANGE)
      << "bank_width is 16. Now bank_width = " << bank_width;
  UNI_LOG_CHECK(bias_bank_num == 4, SIM_OUT_OF_RANGE)
      << "bias_bank_num is 4. Now bias_bank_num = " << bias_bank_num;

  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  int output_channel = channel_;
  int ocg = div_ceil(output_channel, conv_ocp);
  int ddr_block_size = 64;
  int ddr_block_num = ocg;

  for (int idx_block_num = 0; idx_block_num < ddr_block_num; ++idx_block_num) {
    for (int idx_bank = 0; idx_bank < bias_bank_num; ++idx_bank) {
      vector<DPU_DATA_TYPE> bias_buffer(bank_width, 0);
      auto ddr_addr_offset =
          idx_block_num * ddr_block_size + idx_bank * bank_width;
      auto bank_addr_offset = idx_block_num * jump_write_;

      memcpy(bias_buffer.data(), buf + ddr_addr_offset, bank_width);
      auto bank =
          Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_ + idx_bank);
      bank->Write((bank_addr_ + bank_addr_offset) % bank_depth, bank_width,
                  bias_buffer.data());
      if (debug_) {
        Util::SaveHexContSmallEndBankAddr(
            save_name, bias_buffer.data(), bank_width, bank_width, bank_depth,
            bank_id_ + idx_bank, bank_addr_ + bank_addr_offset, SM_APPEND);
      }
    }
  }
}

template <>
void Load<DPUVersion::XV3DPU>::load_layer_param() {
  auto* buf = reinterpret_cast<DPU_DATA_TYPE*>(
      DDR::Instance().GetAddr(reg_id_, ddr_addr_));
  string save_name = debug_path_ + "inst_" + to_string(instid_) + "_" +
                     to_string(instid_classify_) + ".tick";

  auto data_size = block_num_ * length_ * channel_;
  vector<DPU_DATA_TYPE> layer_param_buffer(data_size, 0);
  memcpy(layer_param_buffer.data(), buf, data_size);

  auto bank = Buffer<DPU_DATA_TYPE>::Instance().GetBank(bank_id_);
  bank->Write(bank_addr_, data_size, layer_param_buffer.data());

  if (debug_) {
    Util::SaveHexContSmallEndBankAddr(save_name, layer_param_buffer.data(),
                                      data_size, bank->GetW(), bank->GetH(),
                                      bank_id_, bank_addr_, SM_APPEND);
  }
}
