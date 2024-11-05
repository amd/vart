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

#ifndef __INST_TABLE_HPP__
#define __INST_TABLE_HPP__


#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using std::vector;
using std::string;
using std::map;
using std::unordered_map;

enum class DPUVersion{
	DPU4F,
	DPUV2,
	DPUV3E,
	DPUV3ME,
	DPUV4E,
	XV2DPU,
	XV3DPU,
	XVDPU,
};

// instruction category enum
enum class Category{
    INST_CATEGORY_MIN = 0,
    INST_CATEGORY_LOAD = INST_CATEGORY_MIN,
    INST_CATEGORY_SAVE,
    INST_CATEGORY_CONV,
    INST_CATEGORY_MISC,
    INST_CATEGORY_MAX,
};

namespace DPU4F {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONV, // 3
        INST_TYPE_POOLINIT, // 4
        INST_TYPE_POOL, // 5
        INST_TYPE_DWINIT, // 6
        INST_TYPE_DPTWISE, // 7
        INST_TYPE_ELEWINIT, // 8
        INST_TYPE_ELEW, // 9
        INST_TYPE_THD, // 10
        INST_TYPE_END, // 11
        INST_TYPE_DUMPBANK, // 12
        INST_TYPE_DUMPDDR, // 13
        INST_TYPE_DUMPDDRSLICE, // 14
        INST_TYPE_MAX, // 15
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_HP_ID, // 3
        LOAD_FIELD_BANK_ID, // 4
        LOAD_FIELD_BANK_ADDR, // 5
        LOAD_FIELD_QUANT_LTH, // 6
        LOAD_FIELD_PAD_START, // 7
        LOAD_FIELD_PAD_END, // 8
        LOAD_FIELD_PAD_IDX, // 9
        LOAD_FIELD_JUMP_READ, // 10
        LOAD_FIELD_JUMP_WRITE, // 11
        LOAD_FIELD_LENGTH, // 12
        LOAD_FIELD_MODE_AVG, // 13
        LOAD_FIELD_CHANNEL, // 14
        LOAD_FIELD_REG_ID, // 15
        LOAD_FIELD_DDR_ADDR, // 16
        LOAD_FIELD_MAX, // 17
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_HP_ID, // 3
        SAVE_FIELD_BANK_ID, // 4
        SAVE_FIELD_BANK_ADDR, // 5
        SAVE_FIELD_QUANT_LTH, // 6
        SAVE_FIELD_JUMP_WRITE, // 7
        SAVE_FIELD_JUMP_READ, // 8
        SAVE_FIELD_LENGTH, // 9
        SAVE_FIELD_CHANNEL, // 10
        SAVE_FIELD_REG_ID, // 11
        SAVE_FIELD_DDR_ADDR, // 12
        SAVE_FIELD_MAX, // 13
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_ACT_TYPE, // 3
        CONVINIT_FIELD_SHIFT_CUT, // 4
        CONVINIT_FIELD_JUMP_READ, // 5
        CONVINIT_FIELD_STRIDE_OUT, // 6
        CONVINIT_FIELD_CALT_MODE, // 7
        CONVINIT_FIELD_QUANT_LTH, // 8
        CONVINIT_FIELD_SHIFT_BIAS, // 9
        CONVINIT_FIELD_JUMP_READ_ENDL, // 10
        CONVINIT_FIELD_STRIDE_H, // 11
        CONVINIT_FIELD_KERNEL_H, // 12
        CONVINIT_FIELD_VALID_PIXEL_PARALLEL, // 13
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 14
        CONVINIT_FIELD_JUMP_WRITE, // 15
        CONVINIT_FIELD_STRIDE_W, // 16
        CONVINIT_FIELD_KERNEL_W, // 17
        CONVINIT_FIELD_STRIDE_OFFSET_OUT, // 18
        CONVINIT_FIELD_JUMP_WRITE_ENDL, // 19
        CONVINIT_FIELD_MAX, // 20
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_PAD_LEFT, // 3
        CONV_FIELD_PAD_TOP, // 4
        CONV_FIELD_BANK_ADDR_IN, // 5
        CONV_FIELD_CHANNEL_GROUP, // 6
        CONV_FIELD_CHANNEL_OFFSET, // 7
        CONV_FIELD_PAD_RIGHT, // 8
        CONV_FIELD_PAD_BOTTOM, // 9
        CONV_FIELD_BANK_ADDR_OUT, // 10
        CONV_FIELD_LENGTH, // 11
        CONV_FIELD_BANK_ID_IN, // 12
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 13
        CONV_FIELD_BANK_ADDR_IN_1, // 14
        CONV_FIELD_BANK_ID_OUT, // 15
        CONV_FIELD_BANK_ADDR_BIAS, // 16
        CONV_FIELD_BANK_ADDR_IN_3, // 17
        CONV_FIELD_BANK_ADDR_IN_2, // 18
        CONV_FIELD_MAX, // 19
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_SHIFT_CUT, // 3
        POOLINIT_FIELD_KERNEL_W, // 4
        POOLINIT_FIELD_KERNEL_H, // 5
        POOLINIT_FIELD_JUMP_READ, // 6
        POOLINIT_FIELD_JUMP_WRITE, // 7
        POOLINIT_FIELD_STRIDE_OUT, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 11
        POOLINIT_FIELD_STRIDE_W, // 12
        POOLINIT_FIELD_STRIDE_H, // 13
        POOLINIT_FIELD_POOL_TYPE, // 14
        POOLINIT_FIELD_MAX, // 15
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_CHANNEL_GROUP, // 3
        POOL_FIELD_BANK_ADDR_IN, // 4
        POOL_FIELD_JUMP_WRITE_ENDL, // 5
        POOL_FIELD_PAD_LEFT, // 6
        POOL_FIELD_PAD_TOP, // 7
        POOL_FIELD_PAD_RIGHT, // 8
        POOL_FIELD_PAD_BOTTOM, // 9
        POOL_FIELD_BANK_ID_IN, // 10
        POOL_FIELD_JUMP_READ_ENDL, // 11
        POOL_FIELD_LENGTH, // 12
        POOL_FIELD_BANK_ID_OUT, // 13
        POOL_FIELD_BANK_ADDR_IN_1, // 14
        POOL_FIELD_BANK_ADDR_OUT, // 15
        POOL_FIELD_BANK_ADDR_IN_3, // 16
        POOL_FIELD_BANK_ADDR_IN_2, // 17
        POOL_FIELD_MAX, // 18
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_JUMP_READ, // 3
        DWINIT_FIELD_JUMP_READ_ENDL, // 4
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 5
        DWINIT_FIELD_VALID_PIXEL_PARALLEL, // 6
        DWINIT_FIELD_STRIDE_W, // 7
        DWINIT_FIELD_STRIDE_H, // 8
        DWINIT_FIELD_KERNEL_W, // 9
        DWINIT_FIELD_KERNEL_H, // 10
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 11
        DWINIT_FIELD_JUMP_WRITE, // 12
        DWINIT_FIELD_STRIDE_OUT, // 13
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 14
        DWINIT_FIELD_MAX, // 15
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_CHANNEL_GROUP, // 3
        DPTWISE_FIELD_BANK_ADDR_IN, // 4
        DPTWISE_FIELD_PAD_LEFT, // 5
        DPTWISE_FIELD_PAD_RIGHT, // 6
        DPTWISE_FIELD_PAD_TOP, // 7
        DPTWISE_FIELD_PAD_BOTTOM, // 8
        DPTWISE_FIELD_CHANNEL_OFFSET, // 9
        DPTWISE_FIELD_BANK_ADDR_OUT, // 10
        DPTWISE_FIELD_ACT_TYPE, // 11
        DPTWISE_FIELD_LENGTH, // 12
        DPTWISE_FIELD_BANK_ID_IN, // 13
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 14
        DPTWISE_FIELD_BANK_ADDR_IN_1, // 15
        DPTWISE_FIELD_BANK_ID_OUT, // 16
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 17
        DPTWISE_FIELD_BANK_ADDR_IN_3, // 18
        DPTWISE_FIELD_BANK_ADDR_IN_2, // 19
        DPTWISE_FIELD_MAX, // 20
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_BANK_ID_IN, // 3
        ELEWINIT_FIELD_SHIFT_READ, // 4
        ELEWINIT_FIELD_JUMP_READ, // 5
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 6
        ELEWINIT_FIELD_ID, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_MAX, // 9
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_SHIFT_WRITE, // 4
        ELEW_FIELD_JUMP_WRITE, // 5
        ELEW_FIELD_NUM, // 6
        ELEW_FIELD_CHANNEL_GROUP, // 7
        ELEW_FIELD_ACT_TYPE, // 8
        ELEW_FIELD_LENGTH, // 9
        ELEW_FIELD_BANK_ADDR_OUT, // 10
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 11
        ELEW_FIELD_JUMP_WRITE_ENDL, // 12
        ELEW_FIELD_MAX, // 13
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // THD field enum
    enum ThdFieldEnum {
        THD_FIELD_OPCODE, // 0
        THD_FIELD_DPDON, // 1
        THD_FIELD_DPDBY, // 2
        THD_FIELD_CHANNEL_GROUP, // 3
        THD_FIELD_BANK_ADDR_IN, // 4
        THD_FIELD_SIGNED_MODE, // 5
        THD_FIELD_VALID_PIXEL_PARALLEL, // 6
        THD_FIELD_LENGTH, // 7
        THD_FIELD_BANK_ADDR_OUT, // 8
        THD_FIELD_STRIDE_OUT, // 9
        THD_FIELD_BANK_ID_IN, // 10
        THD_FIELD_JUMP_READ, // 11
        THD_FIELD_JUMP_READ_ENDL, // 12
        THD_FIELD_STRIDE_OFFSET_OUT, // 13
        THD_FIELD_BANK_ID_OUT, // 14
        THD_FIELD_JUMP_WRITE, // 15
        THD_FIELD_JUMP_WRITE_ENDL, // 16
        THD_FIELD_PARAM_ADDR, // 17
        THD_FIELD_MAX, // 18
    };
    const static vector<string> ThdFieldName;
    const static vector<int> ThdFieldDataType;
    const static vector<int> ThdFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ThdFieldPos;
    const static vector<map<uint32_t, uint32_t>> ThdFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace DPUV_2 {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONV, // 3
        INST_TYPE_POOLINIT, // 4
        INST_TYPE_POOL, // 5
        INST_TYPE_ALUINIT, // 6
        INST_TYPE_ALU, // 7
        INST_TYPE_DWINIT, // 8
        INST_TYPE_DPTWISE, // 9
        INST_TYPE_ELEWINIT, // 10
        INST_TYPE_ELEW, // 11
        INST_TYPE_END, // 12
        INST_TYPE_DUMPBANK, // 13
        INST_TYPE_DUMPDDR, // 14
        INST_TYPE_DUMPDDRSLICE, // 15
        INST_TYPE_MAX, // 16
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_BANK_ID, // 3
        LOAD_FIELD_BANK_ADDR, // 4
        LOAD_FIELD_JUMP_READ, // 5
        LOAD_FIELD_JUMP_WRITE, // 6
        LOAD_FIELD_PAD_IDX, // 7
        LOAD_FIELD_CHANNEL, // 8
        LOAD_FIELD_LENGTH, // 9
        LOAD_FIELD_PAD_START, // 10
        LOAD_FIELD_PAD_END, // 11
        LOAD_FIELD_MODE_AVG, // 12
        LOAD_FIELD_BROADCAST, // 13
        LOAD_FIELD_CONST_VALUE, // 14
        LOAD_FIELD_REG_ID, // 15
        LOAD_FIELD_DDR_ADDR, // 16
        LOAD_FIELD_MAX, // 17
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_BANK_ID, // 3
        SAVE_FIELD_BANK_ADDR, // 4
        SAVE_FIELD_HP_ID, // 5
        SAVE_FIELD_ARGMAX, // 6
        SAVE_FIELD_JUMP_WRITE, // 7
        SAVE_FIELD_JUMP_READ, // 8
        SAVE_FIELD_CHANNEL, // 9
        SAVE_FIELD_LENGTH, // 10
        SAVE_FIELD_CONST_EN, // 11
        SAVE_FIELD_CONST_VALUE, // 12
        SAVE_FIELD_REG_ID, // 13
        SAVE_FIELD_DDR_ADDR, // 14
        SAVE_FIELD_MAX, // 15
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_ACT_TYPE, // 3
        CONVINIT_FIELD_SHIFT_CUT, // 4
        CONVINIT_FIELD_JUMP_READ, // 5
        CONVINIT_FIELD_STRIDE_OUT, // 6
        CONVINIT_FIELD_SHIFT_BIAS, // 7
        CONVINIT_FIELD_JUMP_READ_ENDL, // 8
        CONVINIT_FIELD_STRIDE_H, // 9
        CONVINIT_FIELD_KERNEL_H, // 10
        CONVINIT_FIELD_VALID_PIXEL_PARALLEL, // 11
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 12
        CONVINIT_FIELD_JUMP_WRITE, // 13
        CONVINIT_FIELD_STRIDE_W, // 14
        CONVINIT_FIELD_KERNEL_W, // 15
        CONVINIT_FIELD_STRIDE_OFFSET_OUT, // 16
        CONVINIT_FIELD_JUMP_WRITE_ENDL, // 17
        CONVINIT_FIELD_MAX, // 18
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_PAD_LEFT, // 3
        CONV_FIELD_PAD_TOP, // 4
        CONV_FIELD_BANK_ADDR_IN, // 5
        CONV_FIELD_CHANNEL_GROUP, // 6
        CONV_FIELD_PAD_RIGHT, // 7
        CONV_FIELD_PAD_BOTTOM, // 8
        CONV_FIELD_BANK_ADDR_OUT, // 9
        CONV_FIELD_LENGTH, // 10
        CONV_FIELD_BANK_ID_IN, // 11
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 12
        CONV_FIELD_BANK_ADDR_IN_1, // 13
        CONV_FIELD_BANK_ID_OUT, // 14
        CONV_FIELD_BANK_ADDR_BIAS, // 15
        CONV_FIELD_CHANNEL_OFFSET, // 16
        CONV_FIELD_BANK_ADDR_IN_3, // 17
        CONV_FIELD_BANK_ADDR_IN_2, // 18
        CONV_FIELD_MAX, // 19
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_SHIFT_CUT, // 3
        POOLINIT_FIELD_KERNEL_W, // 4
        POOLINIT_FIELD_KERNEL_H, // 5
        POOLINIT_FIELD_JUMP_READ, // 6
        POOLINIT_FIELD_JUMP_WRITE, // 7
        POOLINIT_FIELD_STRIDE_OUT, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 11
        POOLINIT_FIELD_STRIDE_W, // 12
        POOLINIT_FIELD_STRIDE_H, // 13
        POOLINIT_FIELD_POOL_TYPE, // 14
        POOLINIT_FIELD_MAX, // 15
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_CHANNEL_GROUP, // 3
        POOL_FIELD_BANK_ADDR_IN, // 4
        POOL_FIELD_JUMP_WRITE_ENDL, // 5
        POOL_FIELD_PAD_LEFT, // 6
        POOL_FIELD_PAD_TOP, // 7
        POOL_FIELD_PAD_RIGHT, // 8
        POOL_FIELD_PAD_BOTTOM, // 9
        POOL_FIELD_BANK_ID_IN, // 10
        POOL_FIELD_JUMP_READ_ENDL, // 11
        POOL_FIELD_LENGTH, // 12
        POOL_FIELD_BANK_ID_OUT, // 13
        POOL_FIELD_DOWNSAMPLE_KERNEL_W, // 14
        POOL_FIELD_BANK_ADDR_IN_1, // 15
        POOL_FIELD_BANK_ADDR_OUT, // 16
        POOL_FIELD_DOWNSAMPLE_KERNEL_H, // 17
        POOL_FIELD_BANK_ADDR_IN_3, // 18
        POOL_FIELD_BANK_ADDR_IN_2, // 19
        POOL_FIELD_MAX, // 20
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // ALUINIT field enum
    enum AluInitFieldEnum {
        ALUINIT_FIELD_OPCODE, // 0
        ALUINIT_FIELD_DPDON, // 1
        ALUINIT_FIELD_DPDBY, // 2
        ALUINIT_FIELD_EXEC_MODE, // 3
        ALUINIT_FIELD_KERNEL_H, // 4
        ALUINIT_FIELD_KERNEL_W, // 5
        ALUINIT_FIELD_STRIDE_H, // 6
        ALUINIT_FIELD_STRIDE_W, // 7
        ALUINIT_FIELD_STRIDE_OFFSET_IN, // 8
        ALUINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        ALUINIT_FIELD_STRIDE_OUT, // 10
        ALUINIT_FIELD_CHANNEL_GROUP, // 11
        ALUINIT_FIELD_CHANNEL_OFFSET, // 12
        ALUINIT_FIELD_LENGTH, // 13
        ALUINIT_FIELD_SHIFT_PRELU_P, // 14
        ALUINIT_FIELD_ACT_TYPE, // 15
        ALUINIT_FIELD_SHIFT_BIAS, // 16
        ALUINIT_FIELD_SHIFT_CUT, // 17
        ALUINIT_FIELD_SHIFT_PRELU_N, // 18
        ALUINIT_FIELD_MULTI_FACTOR, // 19
        ALUINIT_FIELD_JUMP_READ, // 20
        ALUINIT_FIELD_JUMP_READ_ENDL, // 21
        ALUINIT_FIELD_JUMP_READ_WEIGHTS, // 22
        ALUINIT_FIELD_JUMP_WRITE, // 23
        ALUINIT_FIELD_JUMP_WRITE_ENDL, // 24
        ALUINIT_FIELD_MAX, // 25
    };
    const static vector<string> AluInitFieldName;
    const static vector<int> AluInitFieldDataType;
    const static vector<int> AluInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldLen;


    // ALU field enum
    enum AluFieldEnum {
        ALU_FIELD_OPCODE, // 0
        ALU_FIELD_DPDON, // 1
        ALU_FIELD_DPDBY, // 2
        ALU_FIELD_BANK_ID_OUT, // 3
        ALU_FIELD_BANK_ADDR_OUT, // 4
        ALU_FIELD_PAD_BOTTOM, // 5
        ALU_FIELD_VALID_PIXEL_PARALLEL, // 6
        ALU_FIELD_BANK_ID_IN, // 7
        ALU_FIELD_BANK_ADDR_IN, // 8
        ALU_FIELD_PAD_TOP, // 9
        ALU_FIELD_PAD_LEFT, // 10
        ALU_FIELD_PAD_RIGHT, // 11
        ALU_FIELD_BANK_ADDR_IN_1, // 12
        ALU_FIELD_KERNEL_D, // 13
        ALU_FIELD_BANK_ADDR_WEIGHTS, // 14
        ALU_FIELD_BANK_ADDR_IN_2, // 15
        ALU_FIELD_BANK_ADDR_BIAS, // 16
        ALU_FIELD_BANK_ADDR_IN_3, // 17
        ALU_FIELD_MAX, // 18
    };
    const static vector<string> AluFieldName;
    const static vector<int> AluFieldDataType;
    const static vector<int> AluFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_JUMP_READ, // 3
        DWINIT_FIELD_JUMP_READ_ENDL, // 4
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 5
        DWINIT_FIELD_VALID_PIXEL_PARALLEL, // 6
        DWINIT_FIELD_STRIDE_W, // 7
        DWINIT_FIELD_STRIDE_H, // 8
        DWINIT_FIELD_KERNEL_W, // 9
        DWINIT_FIELD_KERNEL_H, // 10
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 11
        DWINIT_FIELD_JUMP_WRITE, // 12
        DWINIT_FIELD_STRIDE_OUT, // 13
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 14
        DWINIT_FIELD_SHIFT_BIAS, // 15
        DWINIT_FIELD_SHIFT_CUT, // 16
        DWINIT_FIELD_MAX, // 17
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_CHANNEL_GROUP, // 3
        DPTWISE_FIELD_BANK_ADDR_IN, // 4
        DPTWISE_FIELD_PAD_LEFT, // 5
        DPTWISE_FIELD_PAD_RIGHT, // 6
        DPTWISE_FIELD_PAD_TOP, // 7
        DPTWISE_FIELD_PAD_BOTTOM, // 8
        DPTWISE_FIELD_BANK_ADDR_OUT, // 9
        DPTWISE_FIELD_ACT_TYPE, // 10
        DPTWISE_FIELD_LENGTH, // 11
        DPTWISE_FIELD_BANK_ID_IN, // 12
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 13
        DPTWISE_FIELD_BANK_ADDR_IN_1, // 14
        DPTWISE_FIELD_BANK_ID_OUT, // 15
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 16
        DPTWISE_FIELD_CHANNEL_OFFSET, // 17
        DPTWISE_FIELD_BANK_ADDR_IN_3, // 18
        DPTWISE_FIELD_BANK_ADDR_IN_2, // 19
        DPTWISE_FIELD_MAX, // 20
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_BANK_ID_IN, // 3
        ELEWINIT_FIELD_SHARE_PP, // 4
        ELEWINIT_FIELD_JUMP_BANK, // 5
        ELEWINIT_FIELD_ID, // 6
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_SHIFT_READ, // 9
        ELEWINIT_FIELD_JUMP_READ, // 10
        ELEWINIT_FIELD_MAX, // 11
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_ELEW_TYPE, // 4
        ELEW_FIELD_ACT_TYPE, // 5
        ELEW_FIELD_LENGTH, // 6
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 7
        ELEW_FIELD_NUM, // 8
        ELEW_FIELD_JUMP_WRITE_ENDL, // 9
        ELEW_FIELD_BANK_ADDR_OUT, // 10
        ELEW_FIELD_CHANNEL_GROUP, // 11
        ELEW_FIELD_SHIFT_WRITE, // 12
        ELEW_FIELD_JUMP_WRITE, // 13
        ELEW_FIELD_MAX, // 14
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace DPUV_3E {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONV, // 3
        INST_TYPE_POOLINIT, // 4
        INST_TYPE_POOL, // 5
        INST_TYPE_DWINIT, // 6
        INST_TYPE_DPTWISE, // 7
        INST_TYPE_ELEWINIT, // 8
        INST_TYPE_ELEW, // 9
        INST_TYPE_END, // 10
        INST_TYPE_DUMPBANK, // 11
        INST_TYPE_DUMPDDR, // 12
        INST_TYPE_DUMPDDRSLICE, // 13
        INST_TYPE_MAX, // 14
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_BANK_ID, // 3
        LOAD_FIELD_BANK_ADDR, // 4
        LOAD_FIELD_PAD_START, // 5
        LOAD_FIELD_PAD_END, // 6
        LOAD_FIELD_PAD_IDX, // 7
        LOAD_FIELD_JUMP_READ, // 8
        LOAD_FIELD_JUMP_WRITE, // 9
        LOAD_FIELD_LENGTH, // 10
        LOAD_FIELD_MODE_AVG, // 11
        LOAD_FIELD_CHANNEL, // 12
        LOAD_FIELD_REG_ID, // 13
        LOAD_FIELD_DDR_ADDR, // 14
        LOAD_FIELD_DDR_MODE, // 15
        LOAD_FIELD_OUTPUT_CHANNEL_NUM, // 16
        LOAD_FIELD_JUMP_READ_ENDL, // 17
        LOAD_FIELD_REG_ID_1, // 18
        LOAD_FIELD_DDR_ADDR_1, // 19
        LOAD_FIELD_MAX, // 20
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_BANK_ID, // 3
        SAVE_FIELD_BANK_ADDR, // 4
        SAVE_FIELD_JUMP_WRITE, // 5
        SAVE_FIELD_JUMP_READ, // 6
        SAVE_FIELD_LENGTH, // 7
        SAVE_FIELD_CHANNEL, // 8
        SAVE_FIELD_REG_ID, // 9
        SAVE_FIELD_DDR_ADDR, // 10
        SAVE_FIELD_MAX, // 11
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_ACT_TYPE, // 3
        CONVINIT_FIELD_SHIFT_CUT, // 4
        CONVINIT_FIELD_JUMP_READ, // 5
        CONVINIT_FIELD_STRIDE_OUT, // 6
        CONVINIT_FIELD_SHIFT_BIAS, // 7
        CONVINIT_FIELD_JUMP_READ_ENDL, // 8
        CONVINIT_FIELD_STRIDE_H, // 9
        CONVINIT_FIELD_KERNEL_H, // 10
        CONVINIT_FIELD_VALID_PIXEL_PARALLEL, // 11
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 12
        CONVINIT_FIELD_JUMP_WRITE, // 13
        CONVINIT_FIELD_STRIDE_W, // 14
        CONVINIT_FIELD_KERNEL_W, // 15
        CONVINIT_FIELD_STRIDE_OFFSET_OUT, // 16
        CONVINIT_FIELD_JUMP_WRITE_ENDL, // 17
        CONVINIT_FIELD_MAX, // 18
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_BANK_ID_IN, // 3
        CONV_FIELD_BANK_ADDR_IN, // 4
        CONV_FIELD_PAD_TOP, // 5
        CONV_FIELD_CHANNEL_GROUP, // 6
        CONV_FIELD_BANK_ID_OUT, // 7
        CONV_FIELD_BANK_ADDR_OUT, // 8
        CONV_FIELD_PAD_BOTTOM, // 9
        CONV_FIELD_CHANNEL_OFFSET, // 10
        CONV_FIELD_LENGTH, // 11
        CONV_FIELD_BANK_ADDR_IN_1, // 12
        CONV_FIELD_PAD_LEFT, // 13
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 14
        CONV_FIELD_BANK_ADDR_IN_2, // 15
        CONV_FIELD_PAD_RIGHT, // 16
        CONV_FIELD_BANK_ADDR_BIAS, // 17
        CONV_FIELD_BANK_ADDR_IN_3, // 18
        CONV_FIELD_MAX, // 19
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_SHIFT_CUT, // 3
        POOLINIT_FIELD_KERNEL_W, // 4
        POOLINIT_FIELD_KERNEL_H, // 5
        POOLINIT_FIELD_JUMP_READ, // 6
        POOLINIT_FIELD_JUMP_WRITE, // 7
        POOLINIT_FIELD_STRIDE_OUT, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 11
        POOLINIT_FIELD_STRIDE_W, // 12
        POOLINIT_FIELD_STRIDE_H, // 13
        POOLINIT_FIELD_POOL_TYPE, // 14
        POOLINIT_FIELD_MAX, // 15
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_BANK_ID_IN, // 3
        POOL_FIELD_BANK_ADDR_IN, // 4
        POOL_FIELD_JUMP_READ_ENDL, // 5
        POOL_FIELD_BANK_ID_OUT, // 6
        POOL_FIELD_PAD_TOP, // 7
        POOL_FIELD_PAD_BOTTOM, // 8
        POOL_FIELD_PAD_LEFT, // 9
        POOL_FIELD_PAD_RIGHT, // 10
        POOL_FIELD_JUMP_WRITE_ENDL, // 11
        POOL_FIELD_CHANNEL_GROUP, // 12
        POOL_FIELD_LENGTH, // 13
        POOL_FIELD_BANK_ADDR_IN_1, // 14
        POOL_FIELD_BANK_ADDR_IN_2, // 15
        POOL_FIELD_BANK_ADDR_IN_3, // 16
        POOL_FIELD_BANK_ADDR_OUT, // 17
        POOL_FIELD_MAX, // 18
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_STRIDE_OUT, // 3
        DWINIT_FIELD_ACT_TYPE, // 4
        DWINIT_FIELD_JUMP_READ_ENDL, // 5
        DWINIT_FIELD_KERNEL_W, // 6
        DWINIT_FIELD_KERNEL_H, // 7
        DWINIT_FIELD_STRIDE_W, // 8
        DWINIT_FIELD_STRIDE_H, // 9
        DWINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 11
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 12
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 13
        DWINIT_FIELD_SHIFT_BIAS, // 14
        DWINIT_FIELD_SHIFT_CUT, // 15
        DWINIT_FIELD_JUMP_READ, // 16
        DWINIT_FIELD_JUMP_WRITE, // 17
        DWINIT_FIELD_MAX, // 18
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_BANK_ID_IN, // 3
        DPTWISE_FIELD_BANK_ADDR_IN, // 4
        DPTWISE_FIELD_PAD_TOP, // 5
        DPTWISE_FIELD_CHANNEL_GROUP, // 6
        DPTWISE_FIELD_BANK_ID_OUT, // 7
        DPTWISE_FIELD_BANK_ADDR_OUT, // 8
        DPTWISE_FIELD_PAD_BOTTOM, // 9
        DPTWISE_FIELD_CHANNEL_OFFSET, // 10
        DPTWISE_FIELD_LENGTH, // 11
        DPTWISE_FIELD_BANK_ADDR_IN_1, // 12
        DPTWISE_FIELD_PAD_LEFT, // 13
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 14
        DPTWISE_FIELD_BANK_ADDR_IN_2, // 15
        DPTWISE_FIELD_PAD_RIGHT, // 16
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 17
        DPTWISE_FIELD_BANK_ADDR_IN_3, // 18
        DPTWISE_FIELD_MAX, // 19
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_BANK_ID_IN, // 3
        ELEWINIT_FIELD_SHIFT_READ, // 4
        ELEWINIT_FIELD_JUMP_READ, // 5
        ELEWINIT_FIELD_ID, // 6
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_MAX, // 9
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_SHIFT_WRITE, // 4
        ELEW_FIELD_JUMP_WRITE, // 5
        ELEW_FIELD_CHANNEL_GROUP, // 6
        ELEW_FIELD_LENGTH, // 7
        ELEW_FIELD_ACT_TYPE, // 8
        ELEW_FIELD_BANK_ADDR_OUT, // 9
        ELEW_FIELD_NUM, // 10
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 11
        ELEW_FIELD_JUMP_WRITE_ENDL, // 12
        ELEW_FIELD_MAX, // 13
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace DPUV_3ME {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CBLOAD, // 2
        INST_TYPE_CONVINIT, // 3
        INST_TYPE_CONV, // 4
        INST_TYPE_POOLINIT, // 5
        INST_TYPE_POOL, // 6
        INST_TYPE_DWINIT, // 7
        INST_TYPE_DPTWISE, // 8
        INST_TYPE_ELEWINIT, // 9
        INST_TYPE_ELEW, // 10
        INST_TYPE_END, // 11
        INST_TYPE_DUMPBANK, // 12
        INST_TYPE_DUMPDDR, // 13
        INST_TYPE_DUMPDDRSLICE, // 14
        INST_TYPE_MAX, // 15
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_MODE, // 3
        LOAD_FIELD_BANK_ID, // 4
        LOAD_FIELD_LENGTH, // 5
        LOAD_FIELD_JUMP_WRITE, // 6
        LOAD_FIELD_CHANNEL, // 7
        LOAD_FIELD_JUMP_READ, // 8
        LOAD_FIELD_CHANNEL_OFFSET, // 9
        LOAD_FIELD_PAD_BOTTOM, // 10
        LOAD_FIELD_PAD_TOP, // 11
        LOAD_FIELD_MODE_AVG, // 12
        LOAD_FIELD_PAD_START, // 13
        LOAD_FIELD_PAD_END, // 14
        LOAD_FIELD_PAD_IDX, // 15
        LOAD_FIELD_BANK_ADDR, // 16
        LOAD_FIELD_REG_ID, // 17
        LOAD_FIELD_DDR_ADDR, // 18
        LOAD_FIELD_REG_ID_1, // 19
        LOAD_FIELD_DDR_ADDR_1, // 20
        LOAD_FIELD_REG_ID_2, // 21
        LOAD_FIELD_DDR_ADDR_2, // 22
        LOAD_FIELD_MAX, // 23
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_HP_ID, // 3
        SAVE_FIELD_BANK_ID, // 4
        SAVE_FIELD_LENGTH, // 5
        SAVE_FIELD_JUMP_READ, // 6
        SAVE_FIELD_CHANNEL, // 7
        SAVE_FIELD_JUMP_WRITE, // 8
        SAVE_FIELD_BANK_ADDR, // 9
        SAVE_FIELD_REG_ID, // 10
        SAVE_FIELD_DDR_ADDR, // 11
        SAVE_FIELD_MAX, // 12
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CBLOAD field enum
    enum CBLoadFieldEnum {
        CBLOAD_FIELD_OPCODE, // 0
        CBLOAD_FIELD_DPDON, // 1
        CBLOAD_FIELD_DPDBY, // 2
        CBLOAD_FIELD_CHANNEL_GROUP, // 3
        CBLOAD_FIELD_LENGTH, // 4
        CBLOAD_FIELD_CB_SEL, // 5
        CBLOAD_FIELD_GROUP_ID_IN, // 6
        CBLOAD_FIELD_BANK_ADDR_IN, // 7
        CBLOAD_FIELD_BANK_ADDR_OUT, // 8
        CBLOAD_FIELD_MAX, // 9
    };
    const static vector<string> CBLoadFieldName;
    const static vector<int> CBLoadFieldDataType;
    const static vector<int> CBLoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> CBLoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> CBLoadFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_ACT_TYPE, // 3
        CONVINIT_FIELD_SHIFT_CUT, // 4
        CONVINIT_FIELD_JUMP_READ, // 5
        CONVINIT_FIELD_STRIDE_H, // 6
        CONVINIT_FIELD_KERNEL_H, // 7
        CONVINIT_FIELD_STRIDE_OFFSET_OUT_HBM, // 8
        CONVINIT_FIELD_VALID_PIXEL_PARALLEL, // 9
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 10
        CONVINIT_FIELD_JUMP_WRITE_CB, // 11
        CONVINIT_FIELD_STRIDE_W, // 12
        CONVINIT_FIELD_KERNEL_W, // 13
        CONVINIT_FIELD_STRIDE_OFFSET_OUT_CB, // 14
        CONVINIT_FIELD_JUMP_READ_ENDL, // 15
        CONVINIT_FIELD_STRIDE_OUT_CB, // 16
        CONVINIT_FIELD_SHIFT_RELUSIX, // 17
        CONVINIT_FIELD_SHIFT_BIAS, // 18
        CONVINIT_FIELD_JUMP_WRITE_ENDL_CB, // 19
        CONVINIT_FIELD_STRIDE_OUT_HBM, // 20
        CONVINIT_FIELD_JUMP_WRITE_HBM, // 21
        CONVINIT_FIELD_JUMP_WRITE_ENDL_HBM, // 22
        CONVINIT_FIELD_MAX, // 23
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_CHANNEL_GROUP, // 3
        CONV_FIELD_LENGTH, // 4
        CONV_FIELD_OUTPUT_CHANNEL_OFFSET, // 5
        CONV_FIELD_CHANNEL_OFFSET, // 6
        CONV_FIELD_BANK_ID_IN, // 7
        CONV_FIELD_BANK_ID_OUT_CB, // 8
        CONV_FIELD_PAD_LEFT, // 9
        CONV_FIELD_PAD_TOP, // 10
        CONV_FIELD_BANK_ADDR_IN, // 11
        CONV_FIELD_PAD_RIGHT, // 12
        CONV_FIELD_PAD_BOTTOM, // 13
        CONV_FIELD_BANK_ADDR_OUT_CB, // 14
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 15
        CONV_FIELD_BANK_ADDR_BIAS, // 16
        CONV_FIELD_DEST_MODE, // 17
        CONV_FIELD_BANK_ID_OUT_HBM, // 18
        CONV_FIELD_BANK_ADDR_OUT_HBM, // 19
        CONV_FIELD_MAX, // 20
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_SHIFT_CUT, // 3
        POOLINIT_FIELD_KERNEL_W, // 4
        POOLINIT_FIELD_KERNEL_H, // 5
        POOLINIT_FIELD_JUMP_READ, // 6
        POOLINIT_FIELD_JUMP_WRITE, // 7
        POOLINIT_FIELD_STRIDE_OUT, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 11
        POOLINIT_FIELD_STRIDE_W, // 12
        POOLINIT_FIELD_STRIDE_H, // 13
        POOLINIT_FIELD_POOL_TYPE, // 14
        POOLINIT_FIELD_MAX, // 15
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_CHANNEL_GROUP, // 3
        POOL_FIELD_PAD_LEFT, // 4
        POOL_FIELD_PAD_TOP, // 5
        POOL_FIELD_PAD_RIGHT, // 6
        POOL_FIELD_PAD_BOTTOM, // 7
        POOL_FIELD_BANK_ID_IN, // 8
        POOL_FIELD_BANK_ID_OUT, // 9
        POOL_FIELD_JUMP_READ_ENDL, // 10
        POOL_FIELD_LENGTH, // 11
        POOL_FIELD_JUMP_WRITE_ENDL, // 12
        POOL_FIELD_BANK_ADDR_IN, // 13
        POOL_FIELD_BANK_ADDR_OUT, // 14
        POOL_FIELD_MAX, // 15
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_ACT_TYPE, // 3
        DWINIT_FIELD_SHIFT_CUT, // 4
        DWINIT_FIELD_JUMP_READ, // 5
        DWINIT_FIELD_STRIDE_H, // 6
        DWINIT_FIELD_KERNEL_H, // 7
        DWINIT_FIELD_VALID_PIXEL_PARALLEL, // 8
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 9
        DWINIT_FIELD_JUMP_WRITE, // 10
        DWINIT_FIELD_STRIDE_W, // 11
        DWINIT_FIELD_KERNEL_W, // 12
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 13
        DWINIT_FIELD_JUMP_READ_ENDL, // 14
        DWINIT_FIELD_STRIDE_OUT, // 15
        DWINIT_FIELD_SHIFT_RELUSIX, // 16
        DWINIT_FIELD_SHIFT_BIAS, // 17
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 18
        DWINIT_FIELD_MAX, // 19
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_CHANNEL_GROUP, // 3
        DPTWISE_FIELD_LENGTH, // 4
        DPTWISE_FIELD_CHANNEL_OFFSET, // 5
        DPTWISE_FIELD_BANK_ID_IN, // 6
        DPTWISE_FIELD_BANK_ID_OUT, // 7
        DPTWISE_FIELD_PAD_LEFT, // 8
        DPTWISE_FIELD_PAD_TOP, // 9
        DPTWISE_FIELD_BANK_ADDR_IN, // 10
        DPTWISE_FIELD_PAD_RIGHT, // 11
        DPTWISE_FIELD_PAD_BOTTOM, // 12
        DPTWISE_FIELD_BANK_ADDR_OUT, // 13
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 14
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 15
        DPTWISE_FIELD_MAX, // 16
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_BANK_ID_IN, // 3
        ELEWINIT_FIELD_SHIFT_READ, // 4
        ELEWINIT_FIELD_JUMP_READ, // 5
        ELEWINIT_FIELD_ID, // 6
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_MAX, // 9
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_SHIFT_WRITE, // 4
        ELEW_FIELD_JUMP_WRITE, // 5
        ELEW_FIELD_NUM, // 6
        ELEW_FIELD_CHANNEL_GROUP, // 7
        ELEW_FIELD_ACT_TYPE, // 8
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 9
        ELEW_FIELD_LENGTH, // 10
        ELEW_FIELD_JUMP_WRITE_ENDL, // 11
        ELEW_FIELD_BANK_ADDR_OUT, // 12
        ELEW_FIELD_MAX, // 13
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace DPUV_4E {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONV, // 3
        INST_TYPE_POOLINIT, // 4
        INST_TYPE_POOL, // 5
        INST_TYPE_DWINIT, // 6
        INST_TYPE_DPTWISE, // 7
        INST_TYPE_ELEWINIT, // 8
        INST_TYPE_ELEW, // 9
        INST_TYPE_END, // 10
        INST_TYPE_DUMPBANK, // 11
        INST_TYPE_DUMPDDR, // 12
        INST_TYPE_DUMPDDRSLICE, // 13
        INST_TYPE_MAX, // 14
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_OUTPUT_CHANNEL_NUM, // 3
        LOAD_FIELD_BANK_ADDR, // 4
        LOAD_FIELD_LOAD_MODE, // 5
        LOAD_FIELD_PAD_IDX, // 6
        LOAD_FIELD_DDR_MODE, // 7
        LOAD_FIELD_BANK_ID, // 8
        LOAD_FIELD_JUMP_READ, // 9
        LOAD_FIELD_JUMP_WRITE, // 10
        LOAD_FIELD_LENGTH, // 11
        LOAD_FIELD_MODE_AVG, // 12
        LOAD_FIELD_CHANNEL, // 13
        LOAD_FIELD_REG_ID, // 14
        LOAD_FIELD_DDR_ADDR, // 15
        LOAD_FIELD_PAD_START, // 16
        LOAD_FIELD_PAD_END, // 17
        LOAD_FIELD_JUMP_READ_ENDL, // 18
        LOAD_FIELD_MAX, // 19
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_BANK_ADDR, // 3
        SAVE_FIELD_BANK_ID, // 4
        SAVE_FIELD_JUMP_WRITE, // 5
        SAVE_FIELD_JUMP_READ, // 6
        SAVE_FIELD_LENGTH, // 7
        SAVE_FIELD_CHANNEL, // 8
        SAVE_FIELD_REG_ID, // 9
        SAVE_FIELD_DDR_ADDR, // 10
        SAVE_FIELD_MAX, // 11
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_STRIDE_OUT, // 3
        CONVINIT_FIELD_STRIDE_H, // 4
        CONVINIT_FIELD_STRIDE_W, // 5
        CONVINIT_FIELD_KERNEL_H, // 6
        CONVINIT_FIELD_KERNEL_W, // 7
        CONVINIT_FIELD_ACT_TYPE, // 8
        CONVINIT_FIELD_SHIFT_HSWISH, // 9
        CONVINIT_FIELD_CHANNEL_GROUP, // 10
        CONVINIT_FIELD_CHANNEL_OFFSET, // 11
        CONVINIT_FIELD_LENGTH, // 12
        CONVINIT_FIELD_SHIFT_BIAS, // 13
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 14
        CONVINIT_FIELD_JUMP_READ, // 15
        CONVINIT_FIELD_JUMP_READ_ENDL, // 16
        CONVINIT_FIELD_SHIFT_CUT, // 17
        CONVINIT_FIELD_STRIDE_OFFSET_OUT, // 18
        CONVINIT_FIELD_JUMP_WRITE, // 19
        CONVINIT_FIELD_JUMP_WRITE_ENDL, // 20
        CONVINIT_FIELD_SHIFT_HSIGMOID, // 21
        CONVINIT_FIELD_HSIGMOID_IN, // 22
        CONVINIT_FIELD_BATCH_NUM, // 23
        CONVINIT_FIELD_STRIDE_BATCH, // 24
        CONVINIT_FIELD_MAX, // 25
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_BANK_ADDR_BIAS, // 3
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 4
        CONV_FIELD_PAD_LEFT, // 5
        CONV_FIELD_PAD_RIGHT, // 6
        CONV_FIELD_PAD_TOP, // 7
        CONV_FIELD_PAD_BOTTOM, // 8
        CONV_FIELD_VALID_PIXEL_PARALLEL, // 9
        CONV_FIELD_BANK_ADDR_OUT, // 10
        CONV_FIELD_BANK_ID_IN, // 11
        CONV_FIELD_BANK_ADDR_IN_1, // 12
        CONV_FIELD_BANK_ADDR_IN, // 13
        CONV_FIELD_BANK_ID_OUT, // 14
        CONV_FIELD_BANK_ADDR_IN_3, // 15
        CONV_FIELD_BANK_ADDR_IN_2, // 16
        CONV_FIELD_MAX, // 17
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_POOL_TYPE, // 3
        POOLINIT_FIELD_KERNEL_H, // 4
        POOLINIT_FIELD_KERNEL_W, // 5
        POOLINIT_FIELD_STRIDE_H, // 6
        POOLINIT_FIELD_STRIDE_W, // 7
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_STRIDE_OUT, // 10
        POOLINIT_FIELD_JUMP_READ, // 11
        POOLINIT_FIELD_JUMP_READ_ENDL, // 12
        POOLINIT_FIELD_MULTI_FACTOR, // 13
        POOLINIT_FIELD_JUMP_WRITE, // 14
        POOLINIT_FIELD_JUMP_WRITE_ENDL, // 15
        POOLINIT_FIELD_MAX, // 16
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_CHANNEL_GROUP, // 3
        POOL_FIELD_SHIFT_CUT, // 4
        POOL_FIELD_BANK_ID_OUT, // 5
        POOL_FIELD_PAD_LEFT, // 6
        POOL_FIELD_PAD_RIGHT, // 7
        POOL_FIELD_PAD_TOP, // 8
        POOL_FIELD_PAD_BOTTOM, // 9
        POOL_FIELD_BANK_ID_IN, // 10
        POOL_FIELD_VALID_PIXEL_PARALLEL, // 11
        POOL_FIELD_LENGTH, // 12
        POOL_FIELD_BANK_ADDR_OUT, // 13
        POOL_FIELD_BANK_ADDR_IN_1, // 14
        POOL_FIELD_BANK_ADDR_IN, // 15
        POOL_FIELD_BANK_ADDR_IN_3, // 16
        POOL_FIELD_BANK_ADDR_IN_2, // 17
        POOL_FIELD_MAX, // 18
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_STRIDE_OUT, // 3
        DWINIT_FIELD_STRIDE_H, // 4
        DWINIT_FIELD_STRIDE_W, // 5
        DWINIT_FIELD_KERNEL_H, // 6
        DWINIT_FIELD_KERNEL_W, // 7
        DWINIT_FIELD_ACT_TYPE, // 8
        DWINIT_FIELD_CHANNEL_GROUP, // 9
        DWINIT_FIELD_CHANNEL_OFFSET, // 10
        DWINIT_FIELD_LENGTH, // 11
        DWINIT_FIELD_SHIFT_BIAS, // 12
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 13
        DWINIT_FIELD_JUMP_READ, // 14
        DWINIT_FIELD_JUMP_READ_ENDL, // 15
        DWINIT_FIELD_SHIFT_CUT, // 16
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 17
        DWINIT_FIELD_JUMP_WRITE, // 18
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 19
        DWINIT_FIELD_MAX, // 20
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 3
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 4
        DPTWISE_FIELD_PAD_LEFT, // 5
        DPTWISE_FIELD_PAD_RIGHT, // 6
        DPTWISE_FIELD_PAD_TOP, // 7
        DPTWISE_FIELD_PAD_BOTTOM, // 8
        DPTWISE_FIELD_VALID_PIXEL_PARALLEL, // 9
        DPTWISE_FIELD_BANK_ADDR_OUT, // 10
        DPTWISE_FIELD_BANK_ID_IN, // 11
        DPTWISE_FIELD_BANK_ADDR_IN_1, // 12
        DPTWISE_FIELD_BANK_ADDR_IN, // 13
        DPTWISE_FIELD_BANK_ID_OUT, // 14
        DPTWISE_FIELD_BANK_ADDR_IN_3, // 15
        DPTWISE_FIELD_BANK_ADDR_IN_2, // 16
        DPTWISE_FIELD_MAX, // 17
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_SHIFT_READ, // 3
        ELEWINIT_FIELD_BANK_ID_IN, // 4
        ELEWINIT_FIELD_JUMP_READ, // 5
        ELEWINIT_FIELD_ID, // 6
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_MAX, // 9
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_JUMP_WRITE, // 4
        ELEW_FIELD_STRIDE_OUT, // 5
        ELEW_FIELD_NUM, // 6
        ELEW_FIELD_JUMP_WRITE_ENDL, // 7
        ELEW_FIELD_BANK_ADDR_OUT, // 8
        ELEW_FIELD_ACT_TYPE, // 9
        ELEW_FIELD_ELEW_TYPE, // 10
        ELEW_FIELD_CHANNEL_GROUP, // 11
        ELEW_FIELD_STRIDE_OFFSET_OUT, // 12
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 13
        ELEW_FIELD_LENGTH, // 14
        ELEW_FIELD_SHIFT_WRITE, // 15
        ELEW_FIELD_HSIGMOID_IN, // 16
        ELEW_FIELD_SHIFT_HSIGMOID, // 17
        ELEW_FIELD_MAX, // 18
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace XV_2DPU {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONVADDR, // 3
        INST_TYPE_CONV, // 4
        INST_TYPE_ALUINIT, // 5
        INST_TYPE_ALUADDR, // 6
        INST_TYPE_ALU, // 7
        INST_TYPE_END, // 8
        INST_TYPE_DUMPBANK, // 9
        INST_TYPE_DUMPDDR, // 10
        INST_TYPE_DUMPDDRSLICE, // 11
        INST_TYPE_MAX, // 12
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_MT_DST, // 3
        LOAD_FIELD_MT_ADDR, // 4
        LOAD_FIELD_BLOCK_NUM, // 5
        LOAD_FIELD_BROADCAST, // 6
        LOAD_FIELD_PAD_START, // 7
        LOAD_FIELD_PAD_END, // 8
        LOAD_FIELD_PAD_IDX, // 9
        LOAD_FIELD_REG_ID, // 10
        LOAD_FIELD_CONST_VALUE, // 11
        LOAD_FIELD_JUMP_READ, // 12
        LOAD_FIELD_MODE_AVG, // 13
        LOAD_FIELD_LENGTH, // 14
        LOAD_FIELD_CHANNEL, // 15
        LOAD_FIELD_DDR_ADDR, // 16
        LOAD_FIELD_JUMP_WRITE, // 17
        LOAD_FIELD_JUMP_WRITE_ENDL, // 18
        LOAD_FIELD_MAX, // 19
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_ARGMAX, // 3
        SAVE_FIELD_CONST_EN, // 4
        SAVE_FIELD_MT_ADDR, // 5
        SAVE_FIELD_REG_ID, // 6
        SAVE_FIELD_CONST_VALUE, // 7
        SAVE_FIELD_JUMP_WRITE, // 8
        SAVE_FIELD_JUMP_READ, // 9
        SAVE_FIELD_LENGTH, // 10
        SAVE_FIELD_CHANNEL, // 11
        SAVE_FIELD_DDR_ADDR, // 12
        SAVE_FIELD_MAX, // 13
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_EXEC_MODE, // 3
        CONVINIT_FIELD_KERNEL_H, // 4
        CONVINIT_FIELD_KERNEL_W, // 5
        CONVINIT_FIELD_STRIDE_H, // 6
        CONVINIT_FIELD_STRIDE_W, // 7
        CONVINIT_FIELD_TOTAL_IN, // 8
        CONVINIT_FIELD_TILE_ICG, // 9
        CONVINIT_FIELD_TILE_OCG, // 10
        CONVINIT_FIELD_TILE_OHG, // 11
        CONVINIT_FIELD_TILE_OWG, // 12
        CONVINIT_FIELD_IC_ITER, // 13
        CONVINIT_FIELD_OW_ITER, // 14
        CONVINIT_FIELD_OW_OFFSET, // 15
        CONVINIT_FIELD_SHIFT_HSWISH, // 16
        CONVINIT_FIELD_SHIFT_HSIGMOID, // 17
        CONVINIT_FIELD_HSIGMOID_IN, // 18
        CONVINIT_FIELD_KW_ITER, // 19
        CONVINIT_FIELD_KH_ITER, // 20
        CONVINIT_FIELD_ACT_TYPE, // 21
        CONVINIT_FIELD_AIE_MODE, // 22
        CONVINIT_FIELD_CONV_NUM, // 23
        CONVINIT_FIELD_SHIFT_BIAS, // 24
        CONVINIT_FIELD_SHIFT_CUT, // 25
        CONVINIT_FIELD_ONE_HEIGHT, // 26
        CONVINIT_FIELD_ONE_WIDTH, // 27
        CONVINIT_FIELD_ONE_CUBE, // 28
        CONVINIT_FIELD_TILE_EN, // 29
        CONVINIT_FIELD_TOTAL_TILE, // 30
        CONVINIT_FIELD_ONE_SIZE, // 31
        CONVINIT_FIELD_SHIFT_ELEW0, // 32
        CONVINIT_FIELD_SHIFT_ELEW1, // 33
        CONVINIT_FIELD_ELEW_CUT, // 34
        CONVINIT_FIELD_ELEW_ACT_TYPE, // 35
        CONVINIT_FIELD_MAX, // 36
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONVADDR field enum
    enum ConvAddrFieldEnum {
        CONVADDR_FIELD_OPCODE, // 0
        CONVADDR_FIELD_DPDON, // 1
        CONVADDR_FIELD_DPDBY, // 2
        CONVADDR_FIELD_TYPE, // 3
        CONVADDR_FIELD_LAST, // 4
        CONVADDR_FIELD_COMPRESSION, // 5
        CONVADDR_FIELD_INVALID, // 6
        CONVADDR_FIELD_H_NUM, // 7
        CONVADDR_FIELD_MT_ADDR, // 8
        CONVADDR_FIELD_JUMP, // 9
        CONVADDR_FIELD_JUMP_ENDL, // 10
        CONVADDR_FIELD_MAX, // 11
    };
    const static vector<string> ConvAddrFieldName;
    const static vector<int> ConvAddrFieldDataType;
    const static vector<int> ConvAddrFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvAddrFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvAddrFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_ICG_OFFSET, // 3
        CONV_FIELD_OCG_OFFSET, // 4
        CONV_FIELD_PAD_RIGHT, // 5
        CONV_FIELD_PAD_LEFT, // 6
        CONV_FIELD_PAD_BOTTOM, // 7
        CONV_FIELD_PAD_TOP, // 8
        CONV_FIELD_REUSE, // 9
        CONV_FIELD_MACC_CONT, // 10
        CONV_FIELD_MAX, // 11
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // ALUINIT field enum
    enum AluInitFieldEnum {
        ALUINIT_FIELD_OPCODE, // 0
        ALUINIT_FIELD_DPDON, // 1
        ALUINIT_FIELD_DPDBY, // 2
        ALUINIT_FIELD_EXEC_MODE, // 3
        ALUINIT_FIELD_KERNEL_H, // 4
        ALUINIT_FIELD_KERNEL_W, // 5
        ALUINIT_FIELD_STRIDE_H, // 6
        ALUINIT_FIELD_STRIDE_W, // 7
        ALUINIT_FIELD_B_MODE, // 8
        ALUINIT_FIELD_NUM, // 9
        ALUINIT_FIELD_SHARE_KERNEL, // 10
        ALUINIT_FIELD_SHARE_CHANNEL_GROUP, // 11
        ALUINIT_FIELD_TILE_OWG, // 12
        ALUINIT_FIELD_TILE_OHG, // 13
        ALUINIT_FIELD_PAD_TYPE, // 14
        ALUINIT_FIELD_TILE_CG, // 15
        ALUINIT_FIELD_OW_ITER, // 16
        ALUINIT_FIELD_OW_OFFSET, // 17
        ALUINIT_FIELD_SHIFT_HSWISH, // 18
        ALUINIT_FIELD_SHIFT_HSIGMOID, // 19
        ALUINIT_FIELD_HSIGMOID_IN, // 20
        ALUINIT_FIELD_ACT_TYPE, // 21
        ALUINIT_FIELD_SHIFT_BIAS, // 22
        ALUINIT_FIELD_SHIFT_CUT, // 23
        ALUINIT_FIELD_ALU_NUM, // 24
        ALUINIT_FIELD_WEIGHTS_LINES, // 25
        ALUINIT_FIELD_SHIFT_READ_3, // 26
        ALUINIT_FIELD_SHIFT_READ_2, // 27
        ALUINIT_FIELD_SHIFT_READ_1, // 28
        ALUINIT_FIELD_SHIFT_READ_0, // 29
        ALUINIT_FIELD_TOTAL_TILE, // 30
        ALUINIT_FIELD_ONE_HEIGHT, // 31
        ALUINIT_FIELD_ONE_WIDTH, // 32
        ALUINIT_FIELD_KH_ITER, // 33
        ALUINIT_FIELD_INCAO3, // 34
        ALUINIT_FIELD_KW_ITER, // 35
        ALUINIT_FIELD_INCAO2, // 36
        ALUINIT_FIELD_MAX, // 37
    };
    const static vector<string> AluInitFieldName;
    const static vector<int> AluInitFieldDataType;
    const static vector<int> AluInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldLen;


    // ALUADDR field enum
    enum AluAddrFieldEnum {
        ALUADDR_FIELD_OPCODE, // 0
        ALUADDR_FIELD_DPDON, // 1
        ALUADDR_FIELD_DPDBY, // 2
        ALUADDR_FIELD_TYPE, // 3
        ALUADDR_FIELD_LAST, // 4
        ALUADDR_FIELD_COMPRESSION, // 5
        ALUADDR_FIELD_INVALID, // 6
        ALUADDR_FIELD_H_NUM, // 7
        ALUADDR_FIELD_MACC_DIM, // 8
        ALUADDR_FIELD_MT_ADDR, // 9
        ALUADDR_FIELD_ID, // 10
        ALUADDR_FIELD_JUMP, // 11
        ALUADDR_FIELD_JUMP_ENDL, // 12
        ALUADDR_FIELD_MAX, // 13
    };
    const static vector<string> AluAddrFieldName;
    const static vector<int> AluAddrFieldDataType;
    const static vector<int> AluAddrFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluAddrFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluAddrFieldLen;


    // ALU field enum
    enum AluFieldEnum {
        ALU_FIELD_OPCODE, // 0
        ALU_FIELD_DPDON, // 1
        ALU_FIELD_DPDBY, // 2
        ALU_FIELD_REUSE, // 3
        ALU_FIELD_MACC_CONT, // 4
        ALU_FIELD_PAD_LEFT, // 5
        ALU_FIELD_PAD_TOP, // 6
        ALU_FIELD_PAD_RIGHT_MT, // 7
        ALU_FIELD_PAD_BOTTOM_MT, // 8
        ALU_FIELD_PAD_BOTTOM_AT, // 9
        ALU_FIELD_PAD_RIGHT_AT, // 10
        ALU_FIELD_MAX, // 11
    };
    const static vector<string> AluFieldName;
    const static vector<int> AluFieldDataType;
    const static vector<int> AluFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace XV_3DPU {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONVADDR, // 3
        INST_TYPE_CONV, // 4
        INST_TYPE_ALUINIT, // 5
        INST_TYPE_ALUADDR, // 6
        INST_TYPE_ALU, // 7
        INST_TYPE_END, // 8
        INST_TYPE_DWCONVLP_IPUGEN, // 9
        INST_TYPE_DUMPBANK, // 10
        INST_TYPE_DUMPDDR, // 11
        INST_TYPE_DUMPDDRSLICE, // 12
        INST_TYPE_MAX, // 13
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_MT_DST, // 3
        LOAD_FIELD_MT_ADDR, // 4
        LOAD_FIELD_BLOCK_NUM, // 5
        LOAD_FIELD_BROADCAST, // 6
        LOAD_FIELD_PAD_START, // 7
        LOAD_FIELD_PAD_END, // 8
        LOAD_FIELD_PAD_IDX, // 9
        LOAD_FIELD_REG_ID, // 10
        LOAD_FIELD_CONST_VALUE, // 11
        LOAD_FIELD_JUMP_READ, // 12
        LOAD_FIELD_MODE_AVG, // 13
        LOAD_FIELD_IF_PING_PONG_BUFFER, // 14
        LOAD_FIELD_TILE_STRIDE_H, // 15
        LOAD_FIELD_TILE_STRIDE_W, // 16
        LOAD_FIELD_LENGTH, // 17
        LOAD_FIELD_CHANNEL, // 18
        LOAD_FIELD_TRANS_MODE, // 19
        LOAD_FIELD_DDR_ADDR, // 20
        LOAD_FIELD_JUMP_WRITE, // 21
        LOAD_FIELD_JUMP_WRITE_ENDL, // 22
        LOAD_FIELD_SAMPLE_SCALE_H, // 23
        LOAD_FIELD_SAMPLE_SCALE_W, // 24
        LOAD_FIELD_JUMP_READ_ENDL, // 25
        LOAD_FIELD_MAX, // 26
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_ARGMAX, // 3
        SAVE_FIELD_CONST_EN, // 4
        SAVE_FIELD_MT_ADDR, // 5
        SAVE_FIELD_REG_ID, // 6
        SAVE_FIELD_CONST_VALUE, // 7
        SAVE_FIELD_JUMP_WRITE, // 8
        SAVE_FIELD_IF_PING_PONG_BUFFER, // 9
        SAVE_FIELD_LENGTH, // 10
        SAVE_FIELD_CHANNEL, // 11
        SAVE_FIELD_DDR_ADDR, // 12
        SAVE_FIELD_JUMP_READ, // 13
        SAVE_FIELD_BLOCK_NUM, // 14
        SAVE_FIELD_JUMP_READ_ENDL, // 15
        SAVE_FIELD_JUMP_WRITE_ENDL, // 16
        SAVE_FIELD_MAX, // 17
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_EXEC_MODE, // 3
        CONVINIT_FIELD_KERNEL_H, // 4
        CONVINIT_FIELD_KERNEL_W, // 5
        CONVINIT_FIELD_STRIDE_H, // 6
        CONVINIT_FIELD_STRIDE_W, // 7
        CONVINIT_FIELD_TILE_ICG, // 8
        CONVINIT_FIELD_TILE_OCG, // 9
        CONVINIT_FIELD_TILE_OHG, // 10
        CONVINIT_FIELD_TILE_OWG, // 11
        CONVINIT_FIELD_IC_ITER, // 12
        CONVINIT_FIELD_OC_ITER, // 13
        CONVINIT_FIELD_OH_ITER, // 14
        CONVINIT_FIELD_OW_ITER, // 15
        CONVINIT_FIELD_SHIFT_BIAS, // 16
        CONVINIT_FIELD_SHIFT_CUT, // 17
        CONVINIT_FIELD_ACT_TYPE, // 18
        CONVINIT_FIELD_AIE_MODE, // 19
        CONVINIT_FIELD_SHIFT_HSWISH, // 20
        CONVINIT_FIELD_SHIFT_HSIGMOID, // 21
        CONVINIT_FIELD_HSIGMOID_IN, // 22
        CONVINIT_FIELD_OWP, // 23
        CONVINIT_FIELD_ELEW_CUT, // 24
        CONVINIT_FIELD_ELEW_ACT_TYPE, // 25
        CONVINIT_FIELD_CONV_NUM, // 26
        CONVINIT_FIELD_SHIFT_ELEW0, // 27
        CONVINIT_FIELD_SHIFT_ELEW1, // 28
        CONVINIT_FIELD_TILE_EN, // 29
        CONVINIT_FIELD_LP_MT_ADDR, // 30
        CONVINIT_FIELD_OHP, // 31
        CONVINIT_FIELD_MAX, // 32
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONVADDR field enum
    enum ConvAddrFieldEnum {
        CONVADDR_FIELD_OPCODE, // 0
        CONVADDR_FIELD_DPDON, // 1
        CONVADDR_FIELD_DPDBY, // 2
        CONVADDR_FIELD_TYPE, // 3
        CONVADDR_FIELD_LAST, // 4
        CONVADDR_FIELD_COMPRESSION, // 5
        CONVADDR_FIELD_INVALID, // 6
        CONVADDR_FIELD_H_NUM, // 7
        CONVADDR_FIELD_IF_PING_PONG_BUFFER, // 8
        CONVADDR_FIELD_MT_ADDR, // 9
        CONVADDR_FIELD_JUMP, // 10
        CONVADDR_FIELD_JUMP_ENDL, // 11
        CONVADDR_FIELD_MAX, // 12
    };
    const static vector<string> ConvAddrFieldName;
    const static vector<int> ConvAddrFieldDataType;
    const static vector<int> ConvAddrFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvAddrFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvAddrFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_ICG_OFFSET, // 3
        CONV_FIELD_OCG_OFFSET, // 4
        CONV_FIELD_PAD_RIGHT, // 5
        CONV_FIELD_PAD_LEFT, // 6
        CONV_FIELD_PAD_BOTTOM, // 7
        CONV_FIELD_PAD_TOP, // 8
        CONV_FIELD_REUSE, // 9
        CONV_FIELD_MACC_CONT, // 10
        CONV_FIELD_MAX, // 11
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // ALUINIT field enum
    enum AluInitFieldEnum {
        ALUINIT_FIELD_OPCODE, // 0
        ALUINIT_FIELD_DPDON, // 1
        ALUINIT_FIELD_DPDBY, // 2
        ALUINIT_FIELD_EXEC_MODE, // 3
        ALUINIT_FIELD_EXEC_SUB_MODE, // 4
        ALUINIT_FIELD_STRIDE_H, // 5
        ALUINIT_FIELD_STRIDE_W, // 6
        ALUINIT_FIELD_PAD_TYPE, // 7
        ALUINIT_FIELD_INPUT_NUM, // 8
        ALUINIT_FIELD_ALU_NUM, // 9
        ALUINIT_FIELD_OC_ITER, // 10
        ALUINIT_FIELD_TILE_OCG, // 11
        ALUINIT_FIELD_OH_ITER, // 12
        ALUINIT_FIELD_TILE_OHG, // 13
        ALUINIT_FIELD_OW_ITER, // 14
        ALUINIT_FIELD_TILE_OWG, // 15
        ALUINIT_FIELD_B_MODE, // 16
        ALUINIT_FIELD_SHIFT_HSWISH, // 17
        ALUINIT_FIELD_SHIFT_HSIGMOID, // 18
        ALUINIT_FIELD_HSIGMOID_IN, // 19
        ALUINIT_FIELD_ACT_TYPE, // 20
        ALUINIT_FIELD_SHIFT_BIAS, // 21
        ALUINIT_FIELD_SHIFT_CUT, // 22
        ALUINIT_FIELD_SHIFT_READ_3, // 23
        ALUINIT_FIELD_SHIFT_READ_2, // 24
        ALUINIT_FIELD_SHIFT_READ_1, // 25
        ALUINIT_FIELD_SHIFT_READ_0, // 26
        ALUINIT_FIELD_AIE_MODE, // 27
        ALUINIT_FIELD_CP, // 28
        ALUINIT_FIELD_KERNEL_H, // 29
        ALUINIT_FIELD_KERNEL_W, // 30
        ALUINIT_FIELD_LP_MT_ADDR, // 31
        ALUINIT_FIELD_OHP, // 32
        ALUINIT_FIELD_OWP, // 33
        ALUINIT_FIELD_EPSILON, // 34
        ALUINIT_FIELD_MAX, // 35
    };
    const static vector<string> AluInitFieldName;
    const static vector<int> AluInitFieldDataType;
    const static vector<int> AluInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldLen;


    // ALUADDR field enum
    enum AluAddrFieldEnum {
        ALUADDR_FIELD_OPCODE, // 0
        ALUADDR_FIELD_DPDON, // 1
        ALUADDR_FIELD_DPDBY, // 2
        ALUADDR_FIELD_TYPE, // 3
        ALUADDR_FIELD_LAST, // 4
        ALUADDR_FIELD_COMPRESSION, // 5
        ALUADDR_FIELD_INVALID, // 6
        ALUADDR_FIELD_H_NUM, // 7
        ALUADDR_FIELD_IF_PING_PONG_BUFFER, // 8
        ALUADDR_FIELD_MACC_DIM, // 9
        ALUADDR_FIELD_FIX_POINT, // 10
        ALUADDR_FIELD_MT_ADDR, // 11
        ALUADDR_FIELD_ID, // 12
        ALUADDR_FIELD_JUMP, // 13
        ALUADDR_FIELD_JUMP_ENDL, // 14
        ALUADDR_FIELD_MAX, // 15
    };
    const static vector<string> AluAddrFieldName;
    const static vector<int> AluAddrFieldDataType;
    const static vector<int> AluAddrFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluAddrFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluAddrFieldLen;


    // ALU field enum
    enum AluFieldEnum {
        ALU_FIELD_OPCODE, // 0
        ALU_FIELD_DPDON, // 1
        ALU_FIELD_DPDBY, // 2
        ALU_FIELD_REUSE, // 3
        ALU_FIELD_MACC_CONT, // 4
        ALU_FIELD_PAD_LEFT, // 5
        ALU_FIELD_PAD_TOP, // 6
        ALU_FIELD_PAD_RIGHT_MT, // 7
        ALU_FIELD_PAD_BOTTOM_MT, // 8
        ALU_FIELD_PAD_BOTTOM_AT, // 9
        ALU_FIELD_PAD_RIGHT_AT, // 10
        ALU_FIELD_MAX, // 11
    };
    const static vector<string> AluFieldName;
    const static vector<int> AluFieldDataType;
    const static vector<int> AluFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DWCONVLP_IPUGEN field enum
    enum DWCONVLP_IPUGenFieldEnum {
        DWCONVLP_IPUGEN_FIELD_OPCODE, // 0
        DWCONVLP_IPUGEN_FIELD_DPDON, // 1
        DWCONVLP_IPUGEN_FIELD_DPDBY, // 2
        DWCONVLP_IPUGEN_FIELD_AIE_OW, // 3
        DWCONVLP_IPUGEN_FIELD_CHANNEL_MODE, // 4
        DWCONVLP_IPUGEN_FIELD_ITER_MODE, // 5
        DWCONVLP_IPUGEN_FIELD_CORE_SEL, // 6
        DWCONVLP_IPUGEN_FIELD_KERNEL_TYPE_DWCONV, // 7
        DWCONVLP_IPUGEN_FIELD_AIE_OH, // 8
        DWCONVLP_IPUGEN_FIELD_DDR2MT_IC_LOOP, // 9
        DWCONVLP_IPUGEN_FIELD_DDR2MT_W_LOOP, // 10
        DWCONVLP_IPUGEN_FIELD_DDR2MT_H_LOOP, // 11
        DWCONVLP_IPUGEN_FIELD_ITER_INNER, // 12
        DWCONVLP_IPUGEN_FIELD_MT2AIE_C, // 13
        DWCONVLP_IPUGEN_FIELD_MT2AIE_H, // 14
        DWCONVLP_IPUGEN_FIELD_MT2AIE_W, // 15
        DWCONVLP_IPUGEN_FIELD_TILE_ICG_, // 16
        DWCONVLP_IPUGEN_FIELD_STEP_PTR_CO, // 17
        DWCONVLP_IPUGEN_FIELD_STEP_PTR_CI, // 18
        DWCONVLP_IPUGEN_FIELD_KERNEL_TYPE2_DWCONV, // 19
        DWCONVLP_IPUGEN_FIELD_STRIDE_H, // 20
        DWCONVLP_IPUGEN_FIELD_STRIDE_W, // 21
        DWCONVLP_IPUGEN_FIELD_KERNEL_H, // 22
        DWCONVLP_IPUGEN_FIELD_KERNEL_W, // 23
        DWCONVLP_IPUGEN_FIELD_LOCAL_OFFSET, // 24
        DWCONVLP_IPUGEN_FIELD_DWC_KER_WORDS, // 25
        DWCONVLP_IPUGEN_FIELD_ACT_TYPE, // 26
        DWCONVLP_IPUGEN_FIELD_SHIFT_CUT, // 27
        DWCONVLP_IPUGEN_FIELD_SHIFT_BIAS, // 28
        DWCONVLP_IPUGEN_FIELD_IFM_SIZE, // 29
        DWCONVLP_IPUGEN_FIELD_IFM_OFFSET, // 30
        DWCONVLP_IPUGEN_FIELD_DWCONV_WGT_OFFSET, // 31
        DWCONVLP_IPUGEN_FIELD_WGT_SIZE_BD, // 32
        DWCONVLP_IPUGEN_FIELD_CONV_WGT_OFFSET, // 33
        DWCONVLP_IPUGEN_FIELD_OFM_SIZE_BD, // 34
        DWCONVLP_IPUGEN_FIELD_OFM_OFFSET, // 35
        DWCONVLP_IPUGEN_FIELD_T_OFM_ENABLE, // 36
        DWCONVLP_IPUGEN_FIELD_T_IFM_ENABLE, // 37
        DWCONVLP_IPUGEN_FIELD_T_MODE, // 38
        DWCONVLP_IPUGEN_FIELD_T_STEP2, // 39
        DWCONVLP_IPUGEN_FIELD_T_STEP1, // 40
        DWCONVLP_IPUGEN_FIELD_SHIFT_HSWISH, // 41
        DWCONVLP_IPUGEN_FIELD_HSIGMOID_IN, // 42
        DWCONVLP_IPUGEN_FIELD_SHIFT_HSIGMOID, // 43
        DWCONVLP_IPUGEN_FIELD_T_WARP2, // 44
        DWCONVLP_IPUGEN_FIELD_T_WARP1, // 45
        DWCONVLP_IPUGEN_FIELD_MAX, // 46
    };
    const static vector<string> DWCONVLP_IPUGenFieldName;
    const static vector<int> DWCONVLP_IPUGenFieldDataType;
    const static vector<int> DWCONVLP_IPUGenFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWCONVLP_IPUGenFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWCONVLP_IPUGenFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

namespace XV_DPU {
struct InstTable {
public:
    // instruction version
    const static string INST_VERSION;


    // instruction type enum
    enum InstTypeEnum {
        INST_TYPE_MIN = 0,
        INST_TYPE_LOAD = INST_TYPE_MIN, // 0
        INST_TYPE_SAVE, // 1
        INST_TYPE_CONVINIT, // 2
        INST_TYPE_CONV, // 3
        INST_TYPE_POOLINIT, // 4
        INST_TYPE_POOL, // 5
        INST_TYPE_ALUINIT, // 6
        INST_TYPE_ALU, // 7
        INST_TYPE_DWINIT, // 8
        INST_TYPE_DPTWISE, // 9
        INST_TYPE_ELEWINIT, // 10
        INST_TYPE_ELEW, // 11
        INST_TYPE_END, // 12
        INST_TYPE_DUMPBANK, // 13
        INST_TYPE_DUMPDDR, // 14
        INST_TYPE_DUMPDDRSLICE, // 15
        INST_TYPE_MAX, // 16
    };


    // LOAD field enum
    enum LoadFieldEnum {
        LOAD_FIELD_OPCODE, // 0
        LOAD_FIELD_DPDON, // 1
        LOAD_FIELD_DPDBY, // 2
        LOAD_FIELD_BANK_ID, // 3
        LOAD_FIELD_BANK_ADDR, // 4
        LOAD_FIELD_JUMP_READ, // 5
        LOAD_FIELD_JUMP_WRITE, // 6
        LOAD_FIELD_JUMP_WRITE_ENDL, // 7
        LOAD_FIELD_BLOCK_NUM, // 8
        LOAD_FIELD_PAD_IDX, // 9
        LOAD_FIELD_CHANNEL, // 10
        LOAD_FIELD_LENGTH, // 11
        LOAD_FIELD_PAD_START, // 12
        LOAD_FIELD_PAD_END, // 13
        LOAD_FIELD_MODE_AVG, // 14
        LOAD_FIELD_BROADCAST, // 15
        LOAD_FIELD_CONST_VALUE, // 16
        LOAD_FIELD_REG_ID, // 17
        LOAD_FIELD_DDR_ADDR, // 18
        LOAD_FIELD_MAX, // 19
    };
    const static vector<string> LoadFieldName;
    const static vector<int> LoadFieldDataType;
    const static vector<int> LoadFieldMinus;
    const static vector<map<uint32_t, uint32_t>> LoadFieldPos;
    const static vector<map<uint32_t, uint32_t>> LoadFieldLen;


    // SAVE field enum
    enum SaveFieldEnum {
        SAVE_FIELD_OPCODE, // 0
        SAVE_FIELD_DPDON, // 1
        SAVE_FIELD_DPDBY, // 2
        SAVE_FIELD_BANK_ID, // 3
        SAVE_FIELD_BANK_ADDR, // 4
        SAVE_FIELD_ARGMAX, // 5
        SAVE_FIELD_JUMP_WRITE, // 6
        SAVE_FIELD_JUMP_READ, // 7
        SAVE_FIELD_CHANNEL, // 8
        SAVE_FIELD_LENGTH, // 9
        SAVE_FIELD_CONST_EN, // 10
        SAVE_FIELD_CONST_VALUE, // 11
        SAVE_FIELD_REG_ID, // 12
        SAVE_FIELD_DDR_ADDR, // 13
        SAVE_FIELD_MAX, // 14
    };
    const static vector<string> SaveFieldName;
    const static vector<int> SaveFieldDataType;
    const static vector<int> SaveFieldMinus;
    const static vector<map<uint32_t, uint32_t>> SaveFieldPos;
    const static vector<map<uint32_t, uint32_t>> SaveFieldLen;


    // CONVINIT field enum
    enum ConvInitFieldEnum {
        CONVINIT_FIELD_OPCODE, // 0
        CONVINIT_FIELD_DPDON, // 1
        CONVINIT_FIELD_DPDBY, // 2
        CONVINIT_FIELD_TILE_EN, // 3
        CONVINIT_FIELD_SHIFT_CUT, // 4
        CONVINIT_FIELD_JUMP_READ_ENDL, // 5
        CONVINIT_FIELD_STRIDE_H, // 6
        CONVINIT_FIELD_STRIDE_W, // 7
        CONVINIT_FIELD_KERNEL_H, // 8
        CONVINIT_FIELD_KERNEL_W, // 9
        CONVINIT_FIELD_SHIFT_BIAS, // 10
        CONVINIT_FIELD_JUMP_READ, // 11
        CONVINIT_FIELD_CONV_NUM, // 12
        CONVINIT_FIELD_ACT_TYPE, // 13
        CONVINIT_FIELD_STRIDE_OFFSET_IN, // 14
        CONVINIT_FIELD_STRIDE_OFFSET_OUT, // 15
        CONVINIT_FIELD_JUMP_WRITE_ENDL, // 16
        CONVINIT_FIELD_STRIDE_OUT, // 17
        CONVINIT_FIELD_TILE_ICG, // 18
        CONVINIT_FIELD_TILE_OCG, // 19
        CONVINIT_FIELD_TILE_OWG, // 20
        CONVINIT_FIELD_JUMP_WRITE, // 21
        CONVINIT_FIELD_IC_ITER, // 22
        CONVINIT_FIELD_CHANNEL_OFFSET, // 23
        CONVINIT_FIELD_OW_ITER, // 24
        CONVINIT_FIELD_OW_OFFSET, // 25
        CONVINIT_FIELD_JUMP_READ_WEIGHTS, // 26
        CONVINIT_FIELD_SHIFT_HSWISH, // 27
        CONVINIT_FIELD_SHIFT_HSIGMOID, // 28
        CONVINIT_FIELD_HSIGMOID_IN, // 29
        CONVINIT_FIELD_JUMP_READ_WEIGHTS_ENDL, // 30
        CONVINIT_FIELD_MAX, // 31
    };
    const static vector<string> ConvInitFieldName;
    const static vector<int> ConvInitFieldDataType;
    const static vector<int> ConvInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvInitFieldLen;


    // CONV field enum
    enum ConvFieldEnum {
        CONV_FIELD_OPCODE, // 0
        CONV_FIELD_DPDON, // 1
        CONV_FIELD_DPDBY, // 2
        CONV_FIELD_BANK_ID_OUT, // 3
        CONV_FIELD_BANK_ADDR_OUT, // 4
        CONV_FIELD_ICG_OFFSET, // 5
        CONV_FIELD_BANK_ID_IN, // 6
        CONV_FIELD_BANK_ADDR_IN, // 7
        CONV_FIELD_PAD_LEFT, // 8
        CONV_FIELD_PAD_RIGHT, // 9
        CONV_FIELD_PAD_TOP, // 10
        CONV_FIELD_PAD_BOTTOM, // 11
        CONV_FIELD_BANK_ADDR_IN_1, // 12
        CONV_FIELD_VALID_PIXEL_PARALLEL, // 13
        CONV_FIELD_BANK_ADDR_WEIGHTS, // 14
        CONV_FIELD_BANK_ADDR_IN_2, // 15
        CONV_FIELD_OCG_OFFSET, // 16
        CONV_FIELD_BANK_ADDR_BIAS, // 17
        CONV_FIELD_BANK_ADDR_IN_3, // 18
        CONV_FIELD_MAX, // 19
    };
    const static vector<string> ConvFieldName;
    const static vector<int> ConvFieldDataType;
    const static vector<int> ConvFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ConvFieldPos;
    const static vector<map<uint32_t, uint32_t>> ConvFieldLen;


    // POOLINIT field enum
    enum PoolInitFieldEnum {
        POOLINIT_FIELD_OPCODE, // 0
        POOLINIT_FIELD_DPDON, // 1
        POOLINIT_FIELD_DPDBY, // 2
        POOLINIT_FIELD_SHIFT_CUT, // 3
        POOLINIT_FIELD_KERNEL_W, // 4
        POOLINIT_FIELD_KERNEL_H, // 5
        POOLINIT_FIELD_JUMP_READ, // 6
        POOLINIT_FIELD_JUMP_WRITE, // 7
        POOLINIT_FIELD_STRIDE_OUT, // 8
        POOLINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        POOLINIT_FIELD_VALID_PIXEL_PARALLEL, // 10
        POOLINIT_FIELD_STRIDE_OFFSET_IN, // 11
        POOLINIT_FIELD_STRIDE_W, // 12
        POOLINIT_FIELD_STRIDE_H, // 13
        POOLINIT_FIELD_POOL_TYPE, // 14
        POOLINIT_FIELD_MAX, // 15
    };
    const static vector<string> PoolInitFieldName;
    const static vector<int> PoolInitFieldDataType;
    const static vector<int> PoolInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolInitFieldLen;


    // POOL field enum
    enum PoolFieldEnum {
        POOL_FIELD_OPCODE, // 0
        POOL_FIELD_DPDON, // 1
        POOL_FIELD_DPDBY, // 2
        POOL_FIELD_PAD_LEFT, // 3
        POOL_FIELD_PAD_TOP, // 4
        POOL_FIELD_BANK_ADDR_IN, // 5
        POOL_FIELD_LENGTH, // 6
        POOL_FIELD_PAD_RIGHT, // 7
        POOL_FIELD_PAD_BOTTOM, // 8
        POOL_FIELD_CHANNEL_GROUP, // 9
        POOL_FIELD_BANK_ID_IN, // 10
        POOL_FIELD_JUMP_READ_ENDL, // 11
        POOL_FIELD_JUMP_WRITE_ENDL, // 12
        POOL_FIELD_BANK_ID_OUT, // 13
        POOL_FIELD_BANK_ADDR_IN_1, // 14
        POOL_FIELD_BANK_ADDR_OUT, // 15
        POOL_FIELD_BANK_ADDR_IN_3, // 16
        POOL_FIELD_BANK_ADDR_IN_2, // 17
        POOL_FIELD_MAX, // 18
    };
    const static vector<string> PoolFieldName;
    const static vector<int> PoolFieldDataType;
    const static vector<int> PoolFieldMinus;
    const static vector<map<uint32_t, uint32_t>> PoolFieldPos;
    const static vector<map<uint32_t, uint32_t>> PoolFieldLen;


    // ALUINIT field enum
    enum AluInitFieldEnum {
        ALUINIT_FIELD_OPCODE, // 0
        ALUINIT_FIELD_DPDON, // 1
        ALUINIT_FIELD_DPDBY, // 2
        ALUINIT_FIELD_EXEC_MODE, // 3
        ALUINIT_FIELD_KERNEL_H, // 4
        ALUINIT_FIELD_KERNEL_W, // 5
        ALUINIT_FIELD_STRIDE_H, // 6
        ALUINIT_FIELD_STRIDE_W, // 7
        ALUINIT_FIELD_STRIDE_OFFSET_IN, // 8
        ALUINIT_FIELD_STRIDE_OFFSET_OUT, // 9
        ALUINIT_FIELD_STRIDE_OUT, // 10
        ALUINIT_FIELD_CHANNEL_GROUP, // 11
        ALUINIT_FIELD_CHANNEL_OFFSET, // 12
        ALUINIT_FIELD_LENGTH, // 13
        ALUINIT_FIELD_SHIFT_PRELU_P, // 14
        ALUINIT_FIELD_ACT_TYPE, // 15
        ALUINIT_FIELD_SHIFT_BIAS, // 16
        ALUINIT_FIELD_SHIFT_CUT, // 17
        ALUINIT_FIELD_SHIFT_PRELU_N, // 18
        ALUINIT_FIELD_MULTI_FACTOR, // 19
        ALUINIT_FIELD_JUMP_READ, // 20
        ALUINIT_FIELD_JUMP_READ_ENDL, // 21
        ALUINIT_FIELD_JUMP_READ_WEIGHTS, // 22
        ALUINIT_FIELD_JUMP_WRITE, // 23
        ALUINIT_FIELD_JUMP_WRITE_ENDL, // 24
        ALUINIT_FIELD_MAX, // 25
    };
    const static vector<string> AluInitFieldName;
    const static vector<int> AluInitFieldDataType;
    const static vector<int> AluInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluInitFieldLen;


    // ALU field enum
    enum AluFieldEnum {
        ALU_FIELD_OPCODE, // 0
        ALU_FIELD_DPDON, // 1
        ALU_FIELD_DPDBY, // 2
        ALU_FIELD_BANK_ID_OUT, // 3
        ALU_FIELD_BANK_ADDR_OUT, // 4
        ALU_FIELD_PAD_BOTTOM, // 5
        ALU_FIELD_VALID_PIXEL_PARALLEL, // 6
        ALU_FIELD_BANK_ID_IN, // 7
        ALU_FIELD_BANK_ADDR_IN, // 8
        ALU_FIELD_PAD_TOP, // 9
        ALU_FIELD_PAD_LEFT, // 10
        ALU_FIELD_PAD_RIGHT, // 11
        ALU_FIELD_BANK_ADDR_IN_1, // 12
        ALU_FIELD_KERNEL_D, // 13
        ALU_FIELD_BANK_ADDR_WEIGHTS, // 14
        ALU_FIELD_BANK_ADDR_IN_2, // 15
        ALU_FIELD_BANK_ADDR_BIAS, // 16
        ALU_FIELD_BANK_ADDR_IN_3, // 17
        ALU_FIELD_MAX, // 18
    };
    const static vector<string> AluFieldName;
    const static vector<int> AluFieldDataType;
    const static vector<int> AluFieldMinus;
    const static vector<map<uint32_t, uint32_t>> AluFieldPos;
    const static vector<map<uint32_t, uint32_t>> AluFieldLen;


    // DWINIT field enum
    enum DWInitFieldEnum {
        DWINIT_FIELD_OPCODE, // 0
        DWINIT_FIELD_DPDON, // 1
        DWINIT_FIELD_DPDBY, // 2
        DWINIT_FIELD_CHANNEL_GROUP, // 3
        DWINIT_FIELD_JUMP_READ_ENDL, // 4
        DWINIT_FIELD_STRIDE_H, // 5
        DWINIT_FIELD_STRIDE_W, // 6
        DWINIT_FIELD_KERNEL_H, // 7
        DWINIT_FIELD_KERNEL_W, // 8
        DWINIT_FIELD_STRIDE_OFFSET_IN, // 9
        DWINIT_FIELD_STRIDE_OFFSET_OUT, // 10
        DWINIT_FIELD_JUMP_READ, // 11
        DWINIT_FIELD_STRIDE_OUT, // 12
        DWINIT_FIELD_ACT_TYPE, // 13
        DWINIT_FIELD_CHANNEL_OFFSET, // 14
        DWINIT_FIELD_JUMP_WRITE_ENDL, // 15
        DWINIT_FIELD_SHIFT_CUT, // 16
        DWINIT_FIELD_SHIFT_BIAS, // 17
        DWINIT_FIELD_LENGTH, // 18
        DWINIT_FIELD_JUMP_WRITE, // 19
        DWINIT_FIELD_MAX, // 20
    };
    const static vector<string> DWInitFieldName;
    const static vector<int> DWInitFieldDataType;
    const static vector<int> DWInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> DWInitFieldLen;


    // DPTWISE field enum
    enum DptWiseFieldEnum {
        DPTWISE_FIELD_OPCODE, // 0
        DPTWISE_FIELD_DPDON, // 1
        DPTWISE_FIELD_DPDBY, // 2
        DPTWISE_FIELD_BANK_ID_OUT, // 3
        DPTWISE_FIELD_BANK_ADDR_OUT, // 4
        DPTWISE_FIELD_BANK_ID_IN, // 5
        DPTWISE_FIELD_BANK_ADDR_IN, // 6
        DPTWISE_FIELD_PAD_TOP, // 7
        DPTWISE_FIELD_PAD_BOTTOM, // 8
        DPTWISE_FIELD_PAD_LEFT, // 9
        DPTWISE_FIELD_PAD_RIGHT, // 10
        DPTWISE_FIELD_BANK_ADDR_IN_1, // 11
        DPTWISE_FIELD_VALID_PIXEL_PARALLEL, // 12
        DPTWISE_FIELD_BANK_ADDR_WEIGHTS, // 13
        DPTWISE_FIELD_BANK_ADDR_IN_2, // 14
        DPTWISE_FIELD_BANK_ADDR_BIAS, // 15
        DPTWISE_FIELD_BANK_ADDR_IN_3, // 16
        DPTWISE_FIELD_MAX, // 17
    };
    const static vector<string> DptWiseFieldName;
    const static vector<int> DptWiseFieldDataType;
    const static vector<int> DptWiseFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldPos;
    const static vector<map<uint32_t, uint32_t>> DptWiseFieldLen;


    // ELEWINIT field enum
    enum ElewInitFieldEnum {
        ELEWINIT_FIELD_OPCODE, // 0
        ELEWINIT_FIELD_DPDON, // 1
        ELEWINIT_FIELD_DPDBY, // 2
        ELEWINIT_FIELD_BANK_ID_IN, // 3
        ELEWINIT_FIELD_SHARE_PP, // 4
        ELEWINIT_FIELD_JUMP_BANK, // 5
        ELEWINIT_FIELD_ID, // 6
        ELEWINIT_FIELD_JUMP_READ_ENDL, // 7
        ELEWINIT_FIELD_BANK_ADDR_IN, // 8
        ELEWINIT_FIELD_SHIFT_READ, // 9
        ELEWINIT_FIELD_JUMP_READ, // 10
        ELEWINIT_FIELD_MAX, // 11
    };
    const static vector<string> ElewInitFieldName;
    const static vector<int> ElewInitFieldDataType;
    const static vector<int> ElewInitFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewInitFieldLen;


    // ELEW field enum
    enum ElewFieldEnum {
        ELEW_FIELD_OPCODE, // 0
        ELEW_FIELD_DPDON, // 1
        ELEW_FIELD_DPDBY, // 2
        ELEW_FIELD_BANK_ID_OUT, // 3
        ELEW_FIELD_ELEW_TYPE, // 4
        ELEW_FIELD_ACT_TYPE, // 5
        ELEW_FIELD_LENGTH, // 6
        ELEW_FIELD_VALID_PIXEL_PARALLEL, // 7
        ELEW_FIELD_NUM, // 8
        ELEW_FIELD_JUMP_WRITE_ENDL, // 9
        ELEW_FIELD_BANK_ADDR_OUT, // 10
        ELEW_FIELD_CHANNEL_GROUP, // 11
        ELEW_FIELD_SHIFT_WRITE, // 12
        ELEW_FIELD_JUMP_WRITE, // 13
        ELEW_FIELD_MAX, // 14
    };
    const static vector<string> ElewFieldName;
    const static vector<int> ElewFieldDataType;
    const static vector<int> ElewFieldMinus;
    const static vector<map<uint32_t, uint32_t>> ElewFieldPos;
    const static vector<map<uint32_t, uint32_t>> ElewFieldLen;


    // END field enum
    enum EndFieldEnum {
        END_FIELD_OPCODE, // 0
        END_FIELD_DPDON, // 1
        END_FIELD_DPDBY, // 2
        END_FIELD_MAX, // 3
    };
    const static vector<string> EndFieldName;
    const static vector<int> EndFieldDataType;
    const static vector<int> EndFieldMinus;
    const static vector<map<uint32_t, uint32_t>> EndFieldPos;
    const static vector<map<uint32_t, uint32_t>> EndFieldLen;


    // DUMPBANK field enum
    enum DumpBankFieldEnum {
        DUMPBANK_FIELD_OPCODE, // 0
        DUMPBANK_FIELD_DPDON, // 1
        DUMPBANK_FIELD_DPDBY, // 2
        DUMPBANK_FIELD_SAVE_NAME, // 3
        DUMPBANK_FIELD_SAVE_FMT, // 4
        DUMPBANK_FIELD_BANK_START, // 5
        DUMPBANK_FIELD_BANK_NUM, // 6
        DUMPBANK_FIELD_MAX, // 7
    };
    const static vector<string> DumpBankFieldName;
    const static vector<int> DumpBankFieldDataType;
    const static vector<int> DumpBankFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpBankFieldLen;


    // DUMPDDR field enum
    enum DumpDDRFieldEnum {
        DUMPDDR_FIELD_OPCODE, // 0
        DUMPDDR_FIELD_DPDON, // 1
        DUMPDDR_FIELD_DPDBY, // 2
        DUMPDDR_FIELD_SAVE_NAME, // 3
        DUMPDDR_FIELD_SAVE_FMT, // 4
        DUMPDDR_FIELD_REG_ID, // 5
        DUMPDDR_FIELD_DDR_START, // 6
        DUMPDDR_FIELD_DDR_SIZE, // 7
        DUMPDDR_FIELD_MAX, // 8
    };
    const static vector<string> DumpDDRFieldName;
    const static vector<int> DumpDDRFieldDataType;
    const static vector<int> DumpDDRFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRFieldLen;


    // DUMPDDRSLICE field enum
    enum DumpDDRSliceFieldEnum {
        DUMPDDRSLICE_FIELD_OPCODE, // 0
        DUMPDDRSLICE_FIELD_DPDON, // 1
        DUMPDDRSLICE_FIELD_DPDBY, // 2
        DUMPDDRSLICE_FIELD_SAVE_NAME, // 3
        DUMPDDRSLICE_FIELD_SAVE_FMT, // 4
        DUMPDDRSLICE_FIELD_REG_ID, // 5
        DUMPDDRSLICE_FIELD_DDR_START, // 6
        DUMPDDRSLICE_FIELD_HEIGHT, // 7
        DUMPDDRSLICE_FIELD_HEIGHT_STRIDE, // 8
        DUMPDDRSLICE_FIELD_WIDTH, // 9
        DUMPDDRSLICE_FIELD_WIDTH_STRIDE, // 10
        DUMPDDRSLICE_FIELD_CHANNEL, // 11
        DUMPDDRSLICE_FIELD_CHANNEL_STRIDE, // 12
        DUMPDDRSLICE_FIELD_MAX, // 13
    };
    const static vector<string> DumpDDRSliceFieldName;
    const static vector<int> DumpDDRSliceFieldDataType;
    const static vector<int> DumpDDRSliceFieldMinus;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldPos;
    const static vector<map<uint32_t, uint32_t>> DumpDDRSliceFieldLen;


    // instruction field data type enum
    enum InstFieldDataTypeEnum {
        INST_FIELD_DATA_TYPE_MIN = 0,
        INST_FIELD_DATA_TYPE_STRING = INST_FIELD_DATA_TYPE_MIN,
        INST_FIELD_DATA_TYPE_UINT,
        INST_FIELD_DATA_TYPE_MAX,
    };


    // global vector var
    const static vector<string> CategoryName;
    const static vector<string> InstName;
    const static vector<string> InstNameLower;
    const static vector<Category> InstCategory;
    const static vector<uint32_t> WordNum;
    const static vector<uint32_t> FieldNum;
    const static vector<uint32_t> OPCode;


    // global map var
    const static unordered_map<uint32_t, uint32_t> OPCode2InstType;
    const static unordered_map<string, uint32_t> InstName2InstType;
    const static unordered_map<uint32_t, const vector<string> &> InstFieldName;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldDataType;
    const static unordered_map<uint32_t, const vector<int> &> InstFieldMinus;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldPos;
    const static unordered_map<uint32_t, const vector<map<uint32_t, uint32_t>> &> InstFieldLen;
};
}

template<DPUVersion T> class TableInterface;

template<> class TableInterface<DPUVersion::DPU4F> {
public:
using inst_table = DPU4F::InstTable;
};

template<> class TableInterface<DPUVersion::DPUV2> {
public:
using inst_table = DPUV_2::InstTable;
};

template<> class TableInterface<DPUVersion::DPUV3E> {
public:
using inst_table = DPUV_3E::InstTable;
};

template<> class TableInterface<DPUVersion::DPUV3ME> {
public:
using inst_table = DPUV_3ME::InstTable;
};

template<> class TableInterface<DPUVersion::DPUV4E> {
public:
using inst_table = DPUV_4E::InstTable;
};

template<> class TableInterface<DPUVersion::XV2DPU> {
public:
using inst_table = XV_2DPU::InstTable;
};

template<> class TableInterface<DPUVersion::XV3DPU> {
public:
using inst_table = XV_3DPU::InstTable;
};

template<> class TableInterface<DPUVersion::XVDPU> {
public:
using inst_table = XV_DPU::InstTable;
};

#endif /*__INST_TABLE_HPP__*/
