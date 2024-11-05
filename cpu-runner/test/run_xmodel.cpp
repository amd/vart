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

#include "glob_init.hpp"
#include "cpu_runner.hpp"
#include "cpu_util.hpp"
#include "xmodel_run_sample.hpp"

int main(int argc, char* argv[]) {
  auto gi = GlobInit::make(argc, argv, "./log/");

  vart::cpu::CPUCfg::Instance().LoadConfig();
  vart::cpu::CPUCfg::Instance().PrintConfig();

  auto xmodel = vart::cpu::CPUCfg::Instance().get_model_name();
  auto input = vart::cpu::CPUCfg::Instance().get_input_name();
  auto inputs = vart::cpu::Split(input, "|");
  auto random_flag = vart::cpu::CPUCfg::Instance().get_input_random_flag();
  auto random_seed = vart::cpu::CPUCfg::Instance().get_input_random_seed();

  auto g = xir::Graph::deserialize(xmodel);

  auto sample = make_unique<vart::cpu::XModelRunSample>(std::move(g));

  auto input_tbs = sample->get_input_tbs();
  CHECK(input_tbs.size() == inputs.size())
      << ", " << input_tbs.size() << " != " << inputs.size();

  if (random_flag) {
    for (auto* tb : input_tbs) {
      vart::cpu::Random(vart::cpu::TBPTR(tb), vart::cpu::TBSIZE(tb),
                        static_cast<char>(-16), static_cast<char>(16),
                        random_seed);
    }
  } else {
    for (size_t i = 0; i < input_tbs.size(); i++) {
      vart::cpu::LoadBin(inputs[i], vart::cpu::TBPTR(input_tbs[i]),
                         input_tbs[i]->get_tensor()->get_data_size());
    }
  }

  sample->run();

  return 0;
}
