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
#include "sim_runner.hpp"
#include "ArchCfg.hpp"
#include "SimCfg.hpp"
#include "buffer/DDR.hpp"
#include "inst/pub/Layer.hpp"
#include "inst/pub/ReadInst.hpp"
#include "inst/xv2dpu/simUtil.hpp"
#include "vart/mm/host_flat_tensor_buffer.hpp"

#include <cstring>
#include <vitis/ai/target_factory.hpp>
#include <xir/graph/graph.hpp>
#include <xir/util/tool_function.hpp>

namespace vart {
namespace sim {

SimRunner::SimRunner(const xir::Subgraph* subgraph)
    : Runner(), subgraph_(subgraph), layer_id_(0), batch_idx_(0) {
  UNI_LOG_CHECK(subgraph_->get_attr<std::string>("device") == "DPU",
                SIM_OUT_OF_RANGE);

  UNI_LOG_CHECK(subgraph_->has_attr("dpu_name"), SIM_ATTR_ERROR)
      << subgraph_->to_string() << "'s \"dpu_name\" attribute is missing.";
  UNI_LOG_CHECK(subgraph_->has_attr("dpu_fingerprint"), SIM_ATTR_ERROR)
      << subgraph_->to_string() << "'s \"dpu_fingerprint\" is missing.";
  auto dpu_fingerprint = subgraph_->get_attr<std::uint64_t>("dpu_fingerprint");
  ArchCfg::Instance().set_param(
      vitis::ai::target_factory()->create(dpu_fingerprint));
  auto isa_version = ArchCfg::Instance().get_param().type();
  ArchCfg::Instance().init_white_lists();

  SimCfg::Instance().set_fmap_bank_group();
  SimCfg::Instance().set_isa_version(isa_version);
  if (SimCfg::Instance().get_debug_enable()) {
    Util::ChkFolder(SimCfg::Instance().get_debug_path(), false);
  }

  // initial weight/bias in ddr
  SimCfg::Instance().set_ddr_dump_end_fast(0x00);
  Buffer<DPU_DATA_TYPE>::Instance();
  DDR::Instance().Initial(subgraph_);

  // get input/output nodes, and create vitis tensor from xir tensor
  for (auto input_tensor : subgraph_->get_input_tensors()) {
    inputs_.push_back(input_tensor);
    tensor_map.emplace(input_tensor->get_name(), input_tensor);
  }
  for (auto output_tensor : subgraph_->get_output_tensors()) {
    outputs_.push_back(output_tensor);
    tensor_map.emplace(output_tensor->get_name(), output_tensor);
  }
}

SimRunner::SimRunner() : layer_id_(0), batch_idx_(0) {
  if (SimCfg::Instance().get_debug_enable()) {
    Util::ChkFolder(SimCfg::Instance().get_debug_path(), true);
  }
  // set ISA version
  auto isa_version = ArchCfg::Instance().get_param().type();
  ArchCfg::Instance().init_white_lists();
  SimCfg::Instance().set_fmap_bank_group();
  SimCfg::Instance().set_isa_version(isa_version);
  DDR::Instance().Initial();
  Buffer<DPU_DATA_TYPE>::Instance();

  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_init()) {
    std::string ddr_file = SimCfg::Instance().get_debug_path() + "/ddr_init";
    SimCfg::Instance().set_ddr_dump_end_fast(
        SimCfg::Instance().get_ddr_dump_end_fast() & 0x10);
    DDR::Instance().SaveDDR(ddr_file, SimCfg::Instance().get_ddr_dump_format());
  }
}

void SimRunner::run_ac(const string& inst_file) {
  auto ac_code = get_ac(inst_file);
  std::string layer_dbg_path = SimCfg::Instance().get_debug_path();
  Layer layer(0, layer_id_++, layer_dbg_path, ac_code);
  layer.Run();
  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_end()) {
    std::string ddr_file = SimCfg::Instance().get_debug_path() + "/ddr_end";
    int saveDDRFast = SimCfg::Instance().get_ddr_dump_end_fast();
    saveDDRFast |= (saveDDRFast >> 4);
    SimCfg::Instance().set_ddr_dump_end_fast(saveDDRFast);
    if (SimCfgProxy::Params().CO_SIM_ON) {
      std::string ddr_file_cosim =
          SimCfgProxy::Params().DUMP_DIR + "/ddr_last_save.hex";
      DDR::Instance().SaveDDR_cosim(ddr_file_cosim,
                                    SimCfg::Instance().get_ddr_dump_format());
    } else {
      DDR::Instance().SaveDDR(ddr_file,
                              SimCfg::Instance().get_ddr_dump_format());
    }
  }
}

vector<string> SimRunner::get_ac(const string& inst_file) {
  ifstream ifs(inst_file);
  if (ifs.fail()) {
    UNI_LOG_FATAL(SIM_FILE_OPEN_FAILED)
        << "Open AC file error: " << inst_file << endl;
  }

  vector<string> ac_code;

  // read all kinds of insts, put them in a vector
  while (!ifs.eof()) {
    string line;

    // get one line from file
    getline(ifs, line);
    line = Util::Trim(line);
    if (line.size() == 0) {  // empty line
      continue;
    } else if (line[0] == '#') {  // annotation line
      continue;
    }

    ac_code.emplace_back(line);
  }
  return ac_code;
}

bool SimRunner::if_has_ac_code(const xir::Subgraph* subg) {
  return (subg->has_attr("ac_code") || subg->has_attr("ac_code_verify_slnode"));
}

std::vector<std::string> SimRunner::get_ac_code(const xir::Subgraph* subg) {
  if (if_has_ac_code(subg) == false) {
    return std::vector<std::string>{};
  }

  auto ret =
      subg->has_attr("ac_code_verify_slnode")
          ? subg->get_attr<std::vector<std::string>>("ac_code_verify_slnode")
          : std::vector<std::string>{};
  ret = (ret.size() == 0) && (subg->has_attr("ac_code"))
            ? subg->get_attr<std::vector<std::string>>("ac_code")
            : ret;
  return ret;
}

bool SimRunner::if_has_ac_code_preload(const xir::Subgraph* subg) {
  return (subg->has_attr("ac_code_preload") ||
          subg->has_attr("ac_code_verify_slnode_preload"));
}

std::vector<std::string> SimRunner::get_ac_code_preload(
    const xir::Subgraph* subg) {
  if (if_has_ac_code(subg) == false) {
    return std::vector<std::string>{};
  }
  auto ret = (subg->has_attr("ac_code_preload_verify_slnode"))
                 ? subg->get_attr<std::vector<std::string>>(
                       "ac_code_preload_verify_slnode")
                 : std::vector<std::string>{};
  ret = (ret.size() == 0) && (subg->has_attr("ac_code_preload"))
            ? subg->get_attr<std::vector<std::string>>("ac_code_preload")
            : ret;
  return ret;
}

bool SimRunner::if_has_mc_code(const xir::Subgraph* subg) {
  return (subg->has_attr("mc_code") || subg->has_attr("mc_code_verify_slnode"));
}

std::vector<char> SimRunner::get_mc_code(const xir::Subgraph* subg) {
  if (if_has_mc_code(subg) == false) {
    return std::vector<char>{};
  }
  auto ret = (subg->has_attr("mc_code_verify_slnode"))
                 ? subg->get_attr<std::vector<char>>("mc_code_verify_slnode")
                 : std::vector<char>{};
  ret = ret.size() == 0 && (subg->has_attr("mc_code"))
            ? subg->get_attr<std::vector<char>>("mc_code")
            : ret;
  return ret;
}

void SimRunner::run_subgraph(const xir::Subgraph* subg) {
  if (!subg->get_children_num()) {
    if (if_has_ac_code(subg)) {
      run_superlayer(subg);
    }
  } else {
    UNI_LOG_CHECK(subg->has_attr("children_topological_sort"), SIM_ATTR_ERROR)
        << "This subgraph has no children_topological_sort attr!";
    auto children_topological_sort =
        subg->get_attr<std::vector<std::string>>("children_topological_sort");
    auto children = subg->get_children();
    for (auto& subgraph_name : children_topological_sort) {
      for (auto it = children.begin(); it != children.end(); it++) {
        if ((*it)->get_name() == subgraph_name) {
          run_subgraph(*it);
          break;
        }
      }
    }
  }
}

static const xir::Subgraph* get_device_subgraph(const xir::Subgraph* subgraph) {
  if (subgraph->has_attr("device")) {
    return subgraph;
  }
  if (subgraph->is_root()) {
    return nullptr;
  } else {
    // search the parent
    return get_device_subgraph(subgraph->get_parent());
  }
}

static std::unique_ptr<vart::mm::HostFlatTensorBuffer> create_internal_tb(
    const xir::Subgraph* subgraph, const xir::Tensor* tensor) {
  auto device_subgraph = get_device_subgraph(subgraph);
  UNI_LOG_CHECK(device_subgraph != nullptr, SIM_FIND_DEVICE_SUBGRAPH_FAILED)
      << "Can't find " << subgraph->to_string() << "'s device info.";
  auto tensor_producer = tensor->get_producer();
  const xir::Tensor* internal_tensor{nullptr};
  if (device_subgraph->has_op(tensor_producer)) {
    // if this tensor is in inside the current device
    if (tensor->has_attr("ddr_addr")) {
      // if it has DDR space
      internal_tensor = tensor;
    } else {
      // if it does't have DDR space
      if (tensor_producer->get_type() == "download") {
        // if it's an download, get it's inputs tensor
        internal_tensor = tensor_producer->get_input_tensor("input", 0);
      }
    }
  } else {
    // if the tensor is outside the current device, try to find it's fanout
    // upload op in the current device
    for (auto& fanout_op : tensor_producer->get_fanout_ops()) {
      if ((device_subgraph->has_op(fanout_op)) &&
          (fanout_op->get_type() == "upload")) {
        internal_tensor = fanout_op->get_output_tensor();
      }
    }
  }
  UNI_LOG_CHECK(nullptr != internal_tensor, SIM_TENSOR_NOT_IN_DDR)
      << "Can't find " << tensor->to_string() << " in "
      << device_subgraph->to_string() << "'s DDR space.";
  return std::make_unique<vart::mm::HostFlatTensorBuffer>(internal_tensor);
}

static std::tuple<int32_t, int32_t> cal_reg_id_ddr_addr(const xir::Tensor *tensor)
{
  auto reg_id = tensor->get_attr<std::int32_t>("reg_id");
  auto ddr_addr = tensor->get_attr<std::int32_t>("ddr_addr");
  auto engine_id = SimCfg::Instance().get_tiling_engine_id();
  auto combine_num_to_str = [] (int32_t v1, int32_t v2) -> 
      std::string { return std::to_string(v1) + "_" + std::to_string(v2); };
  auto key_str = combine_num_to_str(reg_id, engine_id);
  auto ddr_offset_map = SimCfg::Instance().get_ddr_addr_offset_map();
  auto fake_rid_to_real_rid_map = SimCfg::Instance().get_fake_rid_to_real_rid_map();
  ddr_addr = ddr_offset_map.count(key_str) > 0 ? (ddr_addr + ddr_offset_map.at(key_str)) : ddr_addr;
  reg_id = fake_rid_to_real_rid_map.count(key_str) > 0 ? fake_rid_to_real_rid_map.at(key_str) : reg_id;
  
  return std::make_tuple(reg_id, ddr_addr);
}

static void load_tensor_to_DDR(const xir::Subgraph* subgraph,
                               mm::HostFlatTensorBuffer* buffer,
                               int32_t idx_batch) {
  auto tensor = buffer->get_tensor();
  auto bit_width = tensor->get_data_type().bit_width;
  UNI_LOG_CHECK(tensor->get_attr<std::int32_t>("location") == 1,
                SIM_TENSOR_LOCATION_ERROR);
  auto reg_id = tensor->get_attr<std::int32_t>("reg_id");

  auto ori_stride = vart::mm::get_strides(tensor, true);
  auto cur_stride = vart::mm::get_strides(tensor, false);
  std::transform(ori_stride.begin(), ori_stride.end(), ori_stride.begin(),
                 [bit_width](auto val) { return val / bit_width; });
  std::transform(cur_stride.begin(), cur_stride.end(), cur_stride.begin(),
                 [bit_width](auto val) { return val / bit_width; });
  auto ddr_addr = tensor->get_attr<std::int32_t>("ddr_addr") +
                  idx_batch * cur_stride.front();
  auto ptr_src = reinterpret_cast<char*>(buffer->data().first) +
                 idx_batch * ori_stride.front();
  auto ptr_dst = DDR::Instance().GetAddr(reg_id, ddr_addr);
  UNI_LOG_INFO << "Load tensor " << tensor->get_name()
               << "(size=" << ori_stride.front() << "B)"
               << " to DDR(id=" << reg_id << ", addr=" << ddr_addr << ")";

  auto data_size = ori_stride.front();
  for (auto i = 0; i < data_size; ++i) {
    auto pos = i;
    std::int64_t addr = 0;
    for (auto j = 0U; j < ori_stride.size(); ++j) {
      auto coord = pos / ori_stride[j];
      pos %= ori_stride[j];
      addr += coord * cur_stride[j];
    }
    ptr_dst[addr] = ptr_src[i];
  }
}

static void save_tensor_from_DDR(const xir::Subgraph* subgraph,
                                 mm::HostFlatTensorBuffer* buffer,
                                 int32_t idx_batch) {
  auto tensor = buffer->get_tensor();
  auto bit_width = tensor->get_data_type().bit_width;
  auto tensor_producer = tensor->get_producer();
  UNI_LOG_CHECK(get_device_subgraph(subgraph)->has_op(tensor_producer),
                SIM_TENSOR_NOT_IN_DDR)
      << "Can't find " << tensor->to_string() << " in " << subgraph->to_string()
      << ".";
  UNI_LOG_CHECK(tensor->get_attr<std::int32_t>("location") == 1,
                SIM_TENSOR_LOCATION_ERROR);
  int32_t reg_id;
  int32_t ddr_addr;
  std::tie(reg_id, ddr_addr) = cal_reg_id_ddr_addr(tensor);
  ddr_addr = tensor->get_attr<std::int32_t>("ddr_addr");

  auto ori_stride = vart::mm::get_strides(tensor, true);
  auto cur_stride = vart::mm::get_strides(tensor, false);
  std::transform(ori_stride.begin(), ori_stride.end(), ori_stride.begin(),
                 [bit_width](auto val) { return val / bit_width; });
  std::transform(cur_stride.begin(), cur_stride.end(), cur_stride.begin(),
                 [bit_width](auto val) { return val / bit_width; });
  auto batch_size = static_cast<uint32_t>(ori_stride.front());

  auto ptr_dst =
      reinterpret_cast<char*>(buffer->data().first) + idx_batch * batch_size;
  auto ptr_src = DDR::Instance().GetAddr(reg_id, ddr_addr);
  UNI_LOG_INFO << "Save tensor " << tensor->get_name() << "(size=" << batch_size
               << "B)"
               << " from DDR(id=" << reg_id << ", addr=" << ddr_addr << ")";
  auto data_size = ori_stride.front();
  for (auto dst_addr = 0; dst_addr < data_size; ++dst_addr) {
    auto pos = dst_addr;
    std::int64_t src_addr = 0;
    for (auto j = 0U; j < ori_stride.size(); ++j) {
      auto coord = pos / ori_stride[j];
      pos %= ori_stride[j];
      src_addr += coord * cur_stride[j];
    }
    ptr_dst[dst_addr] = ptr_src[src_addr];
  }
}

static void cal_correct_rid_offset(const xir::Subgraph* subgraph_) {
  auto reg_engine_offset_info =
      subgraph_->get_attr<std::vector<uint32_t>>("reg_to_engine");
  SimCfg::Instance().clear_ddr_addr_offset_map();  // clear all prepare data
  SimCfg::Instance().set_reg_id_engine_id_offset_value(reg_engine_offset_info);

  auto rid_eid_offset_info = SimCfg::Instance().get_rid_eid_offset();
  auto combine_num_to_str = [] (int32_t v1, int32_t v2) -> 
      std::string { return std::to_string(v1) + "_" + std::to_string(v2); };
  for (auto &table_info : rid_eid_offset_info) {
      auto ddr_offset_map = SimCfg::Instance().get_ddr_addr_offset_map();
      auto fake_rid_to_real_rid_map = SimCfg::Instance().get_fake_rid_to_real_rid_map();
      auto key_str = combine_num_to_str(table_info[0], table_info[1]);
      int32_t key_val = table_info[4];
      int32_t real_rid = table_info[0];
      auto dependency_str = combine_num_to_str(table_info[2], table_info[3]);
      if (ddr_offset_map.count(dependency_str)) {
        key_val += ddr_offset_map.at(dependency_str);
        real_rid = fake_rid_to_real_rid_map.at(dependency_str);
      }
      // solve the 0_0 dependency situation
      if (dependency_str == "0_0") {
        real_rid = 0;
      }
      SimCfg::Instance().set_ddr_addr_offset_map(key_str, key_val);
      SimCfg::Instance().set_fake_rid_to_real_rid_map(key_str, real_rid);
      UNI_LOG_INFO << "key_str " << key_str << " depend_str:" << dependency_str <<
            " offset: " << key_val << " real rid: " << real_rid;
  }
}

static std::string shrink_path_names(const std::string& path_name) {
  std::string ret;
  if (path_name.size() <= 255) {
    ret = path_name;
  } else {
    ret = path_name.substr(0, 120) + "..." +
          path_name.substr(path_name.size() - 120);
  }
  return ret;
}

std::pair<uint32_t, int> SimRunner::execute_async(
    const std::vector<TensorBuffer*>& input,
    const std::vector<TensorBuffer*>& output) {
  std::string reg_file = SimCfg::Instance().get_debug_path() + "/case.cfg";
  DDR::Instance().SaveReg(reg_file);

  std::vector<int> batches;
  for (auto& tb : input) {
    auto vtensor = tb->get_tensor();
    batches.push_back(vtensor->get_shape().at(0));
  }
  UNI_LOG_CHECK(std::adjacent_find(batches.begin(), batches.end(),
                                   std::not_equal_to<>()) == batches.end(),
                SIM_TENSOR_DIM_ERROR)
      << "the batch size for all input tensors is not identical";

  for (int i = 0; i < batches[0]; i++) {
    // copy input data from TensorBuffer to DDR
    for (auto tb : input) {
      auto batch_num = tb->get_tensor()->get_shape().front();
      auto ibuf = create_internal_tb(this->subgraph_, tb->get_tensor());
      vart::mm::tensorbuffer_copy(
          static_cast<vart::mm::HostFlatTensorBuffer*>(tb), ibuf.get());
      for (auto batch_idx = 0; batch_idx < batch_num; ++batch_idx) {
        load_tensor_to_DDR(this->subgraph_, ibuf.get(), batch_idx);
      }
    }
    run();
    // copy output data from DDR to TensorBuffer
    for (auto tb : output) {
      auto batch_num = tb->get_tensor()->get_shape().front();
      auto ibuf = create_internal_tb(this->subgraph_, tb->get_tensor());
      for (auto batch_idx = 0; batch_idx < batch_num; ++batch_idx) {
        save_tensor_from_DDR(this->subgraph_, ibuf.get(), batch_idx);
      }
      vart::mm::tensorbuffer_copy(
          ibuf.get(), static_cast<vart::mm::HostFlatTensorBuffer*>(tb));
    }
    batch_idx_++;
  }
  return std::pair<uint32_t, int>{0, 0};
}

void SimRunner::run() {
  UNI_LOG_INFO << "run batch " << batch_idx_ << "...";
  SimCfg::Instance().set_batch_index(batch_idx_);

  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_net() &&
      SimCfg::Instance().get_ddr_dump_init()) {
    std::string ddr_file = SimCfg::Instance().get_debug_path() + "/" +
                           "batch_" + std::to_string(batch_idx_) + "_ddr_init";
    DDR::Instance().SaveDDR(ddr_file, SimCfg::Instance().get_ddr_dump_format());
  }

  auto children_topological_sort = subgraph_->children_topological_sort();
  if (!children_topological_sort.front()->is_leaf()) {
    cal_correct_rid_offset(subgraph_);
    for (auto &tiling_subg : children_topological_sort) {
      if (if_has_ac_code(tiling_subg)) {
        auto engine_id = tiling_subg->has_attr("engine_id")
                             ? tiling_subg->get_attr<uint32_t>("engine_id")
                             : 0U;
        SimCfg::Instance().set_tiling_engine_id((int)engine_id);
        if (SimCfg::Instance().get_run_mode() == "release") {
          run_superlayer(tiling_subg);
        } else if (tiling_subg->has_attr("children_topological_sort")) {
          run_subgraph(tiling_subg);
        }
      }
    }
  } else {
    if (SimCfg::Instance().get_run_mode() == "release") {
      run_superlayer(subgraph_);
    } else {
      run_subgraph(subgraph_);
    }
  }

  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_net() &&
      SimCfg::Instance().get_ddr_dump_end()) {
    std::string ddr_file = SimCfg::Instance().get_debug_path() + "/" +
                           "batch_" + std::to_string(batch_idx_) + "_ddr_end";
    DDR::Instance().SaveDDR(ddr_file, SimCfg::Instance().get_ddr_dump_format());
  }

  layer_id_ = 0;
}

void SimRunner::run_superlayer(const xir::Subgraph* super) {
  auto super_name = xir::remove_xfix(super->get_name());
  super_name = shrink_path_names(super_name);
  for_each(super_name.begin(), super_name.end(), [](char& c) {
    if (c == '/') c = '_';
  });
  UNI_LOG_INFO << super_name;
  std::string layer_dbg_path = SimCfg::Instance().get_debug_path() + super_name;
  Util::ChkFolder(layer_dbg_path, false);

  auto ac_raw = get_ac_code(super);
  auto has_mc_code = if_has_mc_code(super);
  if (has_mc_code) {
    auto mc_in_xmodel = get_mc_code(super);
    ReadInst readhelper(ac_raw);
    auto mc_from_sim = readhelper.GetMC();
    UNI_LOG_CHECK(mc_from_sim.size() * 4 == mc_in_xmodel.size(),
                  SIM_PARAMETER_FAILED)
        << super_name
        << " 's mc_code size in xmodel not equal to sim-runner generated"
        << " in xmodel: " << mc_in_xmodel.size()
        << ", sim-runner generated: " << mc_from_sim.size();
    char one_word[4];
    for (auto idx_w = 0u; idx_w < mc_from_sim.size(); idx_w++) {
      memcpy(one_word, mc_in_xmodel.data() + idx_w * 4, 4);
      uint32_t xmodel_mc_val = *(reinterpret_cast<uint32_t*>(one_word));
      UNI_LOG_CHECK(xmodel_mc_val == mc_from_sim[idx_w], SIM_PARAMETER_FAILED)
          << super_name << ", "
          << " line " << idx_w
          << ": mc_code in xmodel diff with sim-runner generate one."
          << " xmodel is 0x" << hex << xmodel_mc_val
          << " while sim-runner is 0x" << hex << mc_from_sim[idx_w];
    }
  }

  std::vector<std::string> acp_raw;
  if (if_has_ac_code_preload(super)) {
    acp_raw = get_ac_code_preload(super);
  }
  // erase last END instr, concat ac_code_preload and ac_code
  std::vector<std::string> ac_code;
  for (auto idx_acp = 0U; idx_acp < acp_raw.size(); idx_acp++) {
    if (idx_acp == acp_raw.size() - 1) continue;
    std::string acp_line = acp_raw[idx_acp];
    if (idx_acp == acp_raw.size() - 2) {
      const int pos_dpdby = 10;
      acp_line.replace(pos_dpdby, 4, "0001");
    }
    ac_code.push_back(acp_line);
  }

  for (auto& ele : ac_raw) ac_code.push_back(ele);

  //  auto layer_param =
  //  super->get_attr<std::vector<std::string>>("superlayer");

  // dump assemble code and layer parameter
  if (SimCfg::Instance().get_debug_enable() && (batch_idx_ == 0)) {
    std::string ac_file = layer_dbg_path + "/" + "instr_ac.txt";
    fstream fac(ac_file, std::ios::out | std::ios::trunc);
    for (auto& ele : ac_code) fac << ele << std::endl;
    fac.close();

    // std::string lp_file = layer_dbg_path + "/" + "layer_param.txt";
    // fstream flp(lp_file, std::ios::out | std::ios::trunc);
    // for (auto& ele : layer_param) flp << ele << std::endl;
    // flp.close();
  }
  bool is_layer_debug = false;
  if (SimCfg::Instance().get_debug_enable() &&
      (SimCfg::Instance().is_layer_name_in_list(super_name) ||
       SimCfg::Instance().is_layer_name_in_list(super->get_name())))
    is_layer_debug = true;

  // dump input nodes' tensor
  if (SimCfg::Instance().get_debug_enable() &&
      (SimCfg::Instance().get_debug_layer() || is_layer_debug)) {
    for (auto tensor : super->get_input_tensors()) {
      std::string dump_file =
          tensor_name_to_full(tensor->get_name(), layer_dbg_path, batch_idx_);
      auto buffer = create_internal_tb(this->subgraph_, tensor);
      save_tensor_from_DDR(this->subgraph_, buffer.get(), batch_idx_);
      
      auto pos = dump_file.rfind("/");
      if (dump_file.size() - pos >= 256) {
        dump_file = dump_file.substr(0, 125) + "..." +
          dump_file.substr(dump_file.size() - 125);
      }

      mm::dump_to_file(buffer.get(), dump_file);
    }
  }

  // dump ddr for each layer
  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_layer() &&
      SimCfg::Instance().get_ddr_dump_init()) {
    std::string ddr_file = layer_dbg_path + "/" + "batch_" +
                           std::to_string(batch_idx_) + "_ddr_init";

    using RegEntry = std::tuple<int32_t, int32_t, int32_t>;
    std::vector<RegEntry> tin;
    for (auto& in_tensor : super->get_input_tensors()) {
      auto tensor_ = in_tensor;
      if (!tensor_->has_attr("ddr_addr")) {
        for (auto& fanout_op : tensor_->get_producer()->get_fanout_ops()) {
          if (fanout_op->get_type() == "upload") {
            tensor_ = fanout_op->get_output_tensor();
            break;
          }
        }
      }
      UNI_LOG_CHECK(tensor_->has_attr("ddr_addr"), SIM_INVALID_VALUE)
          << "invalid ddr_addr";

      int32_t reg_id;
      int32_t ddr_addr;
      std::tie(reg_id, ddr_addr) = cal_reg_id_ddr_addr(tensor_);

      std::vector<std::int32_t> ddr_strides =
          tensor_->get_attr<std::vector<std::int32_t>>("stride");
      int32_t elem_num_real =
          tensor_->get_element_num() / tensor_->get_shape().at(0);
      int32_t elem_num_stride = ddr_strides.at(0);
      if (elem_num_stride != elem_num_real) {
        UNI_LOG_WARNING << "the elements in ddr is noncontinuous: "
                        << elem_num_stride << " != " << elem_num_real
                        << " for input tensor " << tensor_->get_name()
                        << " of layer " << super_name << std::endl;
      }
      int32_t elem_num = elem_num_stride;
      tin.push_back(
          std::make_tuple(reg_id, ddr_addr, elem_num * sizeof(DPU_DATA_TYPE)));
    }
    // alignment for result comparison
    for (auto& out_tensor : super->get_output_tensors()) {
      auto tensor = out_tensor;
      if (!tensor->has_attr("ddr_addr")) {
        auto tensor_producer = tensor->get_producer();
        if (tensor_producer->get_type() == "download") {
          // handle the download op
          tensor =
              tensor_producer->get_input_op("input", 0)->get_output_tensor();
        } else {
          // handle the upload op
          for (auto& fanout_op : tensor_producer->get_fanout_ops()) {
            if (fanout_op->get_type() == "upload") {
              tensor = fanout_op->get_output_tensor();
              break;
            } else {
              // do nothing
            }
          }  // for fanout
        }
      }  // handle some no ddr addr situation
      UNI_LOG_CHECK(tensor->has_attr("ddr_addr"), SIM_INVALID_VALUE)
          << "invalid ddr_addr";

      int32_t reg_id;
      int32_t ddr_addr;
      std::tie(reg_id, ddr_addr) = cal_reg_id_ddr_addr(tensor);

      std::vector<std::int32_t> ddr_strides =
          tensor->get_attr<std::vector<std::int32_t>>("stride");
      int32_t elem_num_real =
          tensor->get_element_num() / tensor->get_shape().at(0);
      int32_t elem_num_stride = ddr_strides.at(0);
      if (elem_num_stride != elem_num_real) {
        UNI_LOG_WARNING << "the elements in ddr is noncontinuous: "
                        << elem_num_stride << " != " << elem_num_real
                        << " for output tensor " << tensor->get_name()
                        << " of layer " << super_name << std::endl;
      }
      int32_t elem_num = elem_num_stride;

      auto hp_width = SimCfg::Instance().get_hp_width();
      auto head_offset = ddr_addr / hp_width * hp_width;
      auto head =
          (head_offset == ddr_addr) ? 0 : (head_offset + hp_width - ddr_addr);
      auto remain = elem_num * sizeof(DPU_DATA_TYPE) - head;
      auto tail_offset = remain / hp_width * hp_width;
      auto tail = (tail_offset == remain) ? 0 : (remain - tail_offset);
      if (head) tin.push_back(std::make_tuple(reg_id, head_offset, hp_width));
      if (tail) {
        if (head)
          tail_offset += head_offset + hp_width;
        else
          tail_offset += head_offset;
        tin.push_back(std::make_tuple(reg_id, tail_offset, hp_width));
      }
    }
    DDR::Instance().SaveDDR(ddr_file, tin,
                            SimCfg::Instance().get_ddr_dump_format(),
                            (batch_idx_ != 0));
  }

  Layer layer(0, layer_id_++, layer_dbg_path, ac_code);
  layer.set_layer_debug(is_layer_debug);
  layer.Run();

  // dump output nodes' tensor
  if (SimCfg::Instance().get_debug_enable() &&
      (SimCfg::Instance().get_debug_layer() || is_layer_debug)) {
    for (auto tensor : super->get_output_tensors()) {
      std::string dump_file =
          tensor_name_to_full(tensor->get_name(), layer_dbg_path, batch_idx_);
      auto buffer = create_internal_tb(this->subgraph_, tensor);
      save_tensor_from_DDR(this->subgraph_, buffer.get(), batch_idx_);
      
      auto pos = dump_file.rfind("/");
      if (dump_file.size() - pos >= 256) {
        dump_file = dump_file.substr(0, 125) + "..." +
          dump_file.substr(dump_file.size() - 125);
      }

      mm::dump_to_file(buffer.get(), dump_file);
    }
  }

  // dump ddr after execution for each layer
  if (SimCfg::Instance().get_debug_enable() &&
      SimCfg::Instance().get_ddr_dump_layer() &&
      SimCfg::Instance().get_ddr_dump_end()) {
    std::string ddr_file = layer_dbg_path + "/" + "batch_" +
                           std::to_string(batch_idx_) + "_ddr_end";

    using RegEntry = std::tuple<int32_t, int32_t, int32_t>;
    std::vector<RegEntry> tout;
    for (auto& out_tensor : super->get_output_tensors()) {
      auto tensor = out_tensor;
      if (!tensor->has_attr("ddr_addr")) {
        auto tensor_producer = tensor->get_producer();
        if (tensor_producer->get_type() == "download") {
          // handle the download op
          tensor =
              tensor_producer->get_input_op("input", 0)->get_output_tensor();
        } else {
          // handle the upload op
          for (auto& fanout_op : tensor_producer->get_fanout_ops()) {
            if (fanout_op->get_type() == "upload") {
              tensor = fanout_op->get_output_tensor();
              break;
            } else {
              // do nothing
            }
          }  // for fanout
        }
      }  // handle some no ddr addr situation
      int32_t reg_id;
      int32_t ddr_addr;
      std::tie(reg_id, ddr_addr) = cal_reg_id_ddr_addr(tensor);

      std::vector<std::int32_t> ddr_strides =
          tensor->get_attr<std::vector<std::int32_t>>("stride");
      int32_t elem_num_real =
          tensor->get_element_num() / tensor->get_shape().at(0);
      int32_t elem_num_stride = ddr_strides.at(0);
      if (elem_num_stride != elem_num_real) {
        UNI_LOG_WARNING << "the elements in ddr is noncontinuous: "
                        << elem_num_stride << " != " << elem_num_real
                        << " for output tensor " << tensor->get_name()
                        << " of layer " << super_name << std::endl;
      }
      int32_t elem_num = elem_num_stride;
      tout.push_back(
          std::make_tuple(reg_id, ddr_addr, elem_num * sizeof(DPU_DATA_TYPE)));
    }
    DDR::Instance().SaveDDR(ddr_file, tout,
                            SimCfg::Instance().get_ddr_dump_format(),
                            (batch_idx_ != 0));
  }
}

std::vector<const xir::Tensor*> SimRunner::get_input_tensors() {
  return inputs_;
}

std::vector<const xir::Tensor*> SimRunner::get_output_tensors() {
  return outputs_;
}

int SimRunner::wait(int jobid, int timeout) { return 0; }

string tensor_name_to_file(const string& tensor_name) {
  std::string super_name = xir::remove_xfix(tensor_name);
  for_each(super_name.begin(), super_name.end(), [](char& c) {
    if (c == '/') c = '_';
  });
  return super_name;
}

string tensor_name_to_full(const string& tensor_name,
                           const string& layer_dbg_path, int batch_idx_) {
  auto super_name = tensor_name_to_file(tensor_name);
  auto dump_file =
      layer_dbg_path + (layer_dbg_path.empty() ? "" : "/") + "batch_" +
      std::to_string(batch_idx_) + "_" + super_name +
      Util::GetFileNameSuffix(SimCfg::Instance().get_layer_dump_format());
  return dump_file;
}

}  // namespace sim
}  // namespace vart

extern "C" vart::Runner* create_runner(const xir::Subgraph* subgraph) {
  auto ret = std::make_unique<vart::sim::SimRunner>(subgraph);
  return ret.release();
}
