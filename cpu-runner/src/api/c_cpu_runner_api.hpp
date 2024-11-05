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

#include "cpu_std_inc.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// Interface structure for python
struct APIRunnerOverview {
  char* runner_name; // runner name
  char* device_name; // runner device name
  int op_num; // runner op number
  int op_type_num; // runner op type number
  char* op_type_list; // runner op type name list
};

/*!
 * \brief get given runner's overview,
 *  details are in APIRunnerOverview structure
 *  runner: the runner pointer given as input
 *  info: runner's overview which will be returned
 *
 *  The function is thread-safe,
 *  different thread can call this func.
 */
VART_CPU_DLL void GetCPURunnerOverview(void* runner, APIRunnerOverview* info);

/*!
 * \brief get all op with given runner,
 *  runner: the runner pointer given as input
 *  ops_arr: used to store all returned op
 *
 *  The function is thread-safe,
 *  different thread can call this func.
 */
VART_CPU_DLL void GetOps(void* runner, void** ops_arr);

#ifdef __cplusplus
}
#endif

