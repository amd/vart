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
#include "InstFactory.hpp"
#include "SimCfg.hpp"

shared_ptr<InstBase> InstFactory::CreateInst(int inst_id,
                                             vector<string>& inst_str,
                                             vector<uint32_t>& inst_val) {
  shared_ptr<InstBase> pinst;
  auto isa_ver = SimCfg::Instance().get_isa_version();
  if (isa_ver == int(DPUVersion::DPUV2)) {
    using TABLE = TableInterface<DPUVersion::DPUV2>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::DPUV2>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUINIT) {
      pinst = std::make_shared<AluInit<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALU) {
      pinst = std::make_shared<Alu<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::DPUV2>>(inst_type, inst_id,
                                                       inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::DPUV3E)) {
    using TABLE = TableInterface<DPUVersion::DPUV3E>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::DPUV3E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::DPUV3E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::DPUV3E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::DPUV3E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::DPUV3E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::DPUV3E>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::DPUV3ME)) {
    using TABLE = TableInterface<DPUVersion::DPUV3ME>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CBLOAD) {
      pinst = std::make_shared<CBLoad>(inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::DPUV3ME>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::DPUV3ME>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::XVDPU)) {
    using TABLE = TableInterface<DPUVersion::XVDPU>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::XVDPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUINIT) {
      pinst = std::make_shared<AluInit<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALU) {
      pinst = std::make_shared<Alu<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                       inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::XVDPU>>(inst_type, inst_id,
                                                       inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::XV2DPU)) {
    using TABLE = TableInterface<DPUVersion::XV2DPU>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::XV2DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::XV2DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::XV2DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVADDR) {
      pinst = std::make_shared<ConvAddr<DPUVersion::XV2DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUINIT) {
      pinst = std::make_shared<AluInit<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUADDR) {
      pinst = std::make_shared<AluAddr<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALU) {
      pinst = std::make_shared<Alu<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::XV2DPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::XV3DPU)) {
    using TABLE = TableInterface<DPUVersion::XV3DPU>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::XV3DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::XV3DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::XV3DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVADDR) {
      pinst = std::make_shared<ConvAddr<DPUVersion::XV3DPU>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUINIT) {
      pinst = std::make_shared<AluInit<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALUADDR) {
      pinst = std::make_shared<AluAddr<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ALU) {
      pinst = std::make_shared<Alu<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::XV3DPU>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::DPU4F)) {
    using TABLE = TableInterface<DPUVersion::DPU4F>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::DPU4F>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                          inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_THD) {
      pinst = std::make_shared<Thd>(inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::DPU4F>>(inst_type, inst_id,
                                                       inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else if (isa_ver == int(DPUVersion::DPUV4E)) {
    using TABLE = TableInterface<DPUVersion::DPUV4E>::inst_table;
    auto inst_type = TABLE::OPCode2InstType.at(inst_val[0]);
    if (inst_type == TABLE::INST_TYPE_DUMPBANK) {
      pinst = std::make_shared<DumpBank<DPUVersion::DPUV4E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDR) {
      pinst = std::make_shared<DumpDDR<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DUMPDDRSLICE) {
      pinst = std::make_shared<DumpDDRSlice<DPUVersion::DPUV4E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_LOAD) {
      pinst = std::make_shared<Load<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_SAVE) {
      pinst = std::make_shared<Save<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONVINIT) {
      pinst = std::make_shared<ConvInit<DPUVersion::DPUV4E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_CONV) {
      pinst = std::make_shared<Conv<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOLINIT) {
      pinst = std::make_shared<PoolInit<DPUVersion::DPUV4E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_POOL) {
      pinst = std::make_shared<Pool<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DWINIT) {
      pinst = std::make_shared<DWInit<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                           inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_DPTWISE) {
      pinst = std::make_shared<DptWise<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                            inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEWINIT) {
      pinst = std::make_shared<ElewInit<DPUVersion::DPUV4E>>(
          inst_type, inst_id, inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_ELEW) {
      pinst = std::make_shared<Elew<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                         inst_str, inst_val);
    } else if (inst_type == TABLE::INST_TYPE_END) {
      pinst = std::make_shared<End<DPUVersion::DPUV4E>>(inst_type, inst_id,
                                                        inst_str, inst_val);
    } else {
      UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
          << "not supported inst type: " << inst_type << endl;
    }
  } else {
    UNI_LOG_FATAL(SIM_PARAMETER_FAILED)
        << "not supported isa version, default is v1.4.0!" << endl;
  }

  return pinst;
}
