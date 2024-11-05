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
#include "BaseDMA.hpp"
#include "BaseOps.hpp"

namespace cosim {
namespace xv2dpu {
namespace alu {

class OpTest : public BaseOps<int8_t> {
 public:
  OpTest(AIE2Cfg AIE_) : BaseOps(AIE_) {
    cout << "A param:" << AIE.ic << endl;
    this->ifm_size = AIE.ic * AIE.oc * AIE.src_w * AIE.src_h;
    this->wgt_size = 0;
    this->bias_size = 0;
    this->ofm_size = AIE.ic * AIE.oc * AIE.src_w * AIE.src_h;

    this->IWin1Iter =
        (AIE.owg_tile * AIE.OWp - 2) * AIE.stride_w + AIE.kernel_w;
    this->IHin1Iter =
        (AIE.ohg_tile * AIE.OHp - 1) * AIE.stride_h + AIE.kernel_h;
    this->ICin1Iter = AIE.icg_tile * AIE.ICp;
    this->OW_offset =
        AIE.dst_w % 8 == 0
            ? 0
            : (8 - AIE.dst_w % 8);  // dst_w should not larger than 8
    this->OCG_offset = 0;
    this->OWin1iter = AIE.OWp * AIE.owg_tile;
    this->OHin1iter = AIE.OHp * AIE.ohg_tile;

    this->img_wnd_byte_size = ICin1Iter * IWin1Iter * IHin1Iter * 2;
    this->wgt_wnd_byte_size = ICin1Iter * AIE.kernel_w * AIE.kernel_h;
    this->bias_wnd_byte_size = ICin1Iter;
    this->ofm_wnd_byte_size = ICin1Iter * OWin1iter * OHin1iter;

    this->img_mt_grp_byte_size = AIE.src_h * AIE.src_w * AIE.ic;
    this->ofm_mt_grp_byte_size =
        AIE.dst_h * AIE.dst_w * AIE.OCp * up_align(AIE.ocg, AIE.IMG_MT_GRP);
    this->weights_mt_grp_byte_size =
        AIE.kernel_h * AIE.kernel_w * AIE.ICp * AIE.icg;
    this->bias_mt_grp_byte_size = AIE.OCp * AIE.ocg;

    cout << AIE.OCp << " " << AIE.WGT_MT_GRP << endl;
    OCP_MT = AIE.OCp * AIE.WGT_MT_GRP;
    assert(up_align(AIE.oc, OCP_MT) % AIE.ocg_tile == 0);
    OHIter_NUM = up_align(AIE.dst_h, AIE.ohg_tile * AIE.OHP);
    OWIter_NUM = up_align(AIE.dst_w, AIE.owg_tile * AIE.OWP);
    OCIter_NUM = (up_align(AIE.oc, OCP_MT) / AIE.ocg_tile);
    ICIter_NUM = up_align(AIE.ic, AIE.icg_tile * AIE.ICp);
  }

  virtual void math_op(const vector<int8_t>& img, const vector<int8_t>& wgt,
                       const vector<int8_t>& bias, vector<int8_t>& rlt) {
    int outsize = ofm_size;
    for (int i = 0; i < outsize; i++) {
      rlt[i] = img[i];
    }
  }

  virtual string set_mt2co_ifm(
      int idx_mt_grp, int idx_mt_num, uint32_t img_bytes_size,
      uint32_t mt_img_ifm_addr, int count, int pad_left, int pad_top,
      int pad_right, int pad_bottom, int IWin1Iter, int IHin1Iter, int src_h,
      int src_w, int stride_w, int ICG, int ICG_tile, int Lock_Acq_ID,
      int Lock_Rel_ID, bool BD_casc_fir, bool BD_casc_last, int isPing) {
    cout << " -------------- !!! use this _____________" << endl;
    return setELEWSHIFTImgMT2CO(idx_mt_grp, idx_mt_num, img_bytes_size,
                                mt_img_ifm_addr, count, pad_left, pad_top,
                                pad_right, pad_bottom, IWin1Iter, IHin1Iter,
                                src_h, src_w, stride_w, ICG, Lock_Acq_ID,
                                Lock_Rel_ID, BD_casc_fir, BD_casc_last, isPing);
  }

  virtual void set_layer_params() {
    int32_t incS0 = 0;
    int32_t numAL1 = 0;
    int32_t incAL1 = 0;
    int32_t numAL2 = 0;
    int32_t incAL2 = 0;
    int32_t incAL3 = 0;

    int32_t numAO1 = 0;
    int32_t incAO1 = 0;
    int32_t numAO2 = 0;
    int32_t incAO2 = 0;
    int32_t incAO3 = 0;

    int32_t numB = 0;
    int32_t incB2 = 0;
    int32_t numBias = 0;
    int32_t incBias2 = 0;

    int32_t numOFM1 = 0;
    int32_t incOFM1 = 0;
    int32_t numOFM2 = 0;
    int32_t incOFM2 = 0;
    int32_t incOFM3 = 0;

    int32_t outer_count = 0;
    int32_t inner_count = 0;
    int32_t ofm_offset = 0;
    int32_t bias_offset = 0;
    int32_t shift_bias = 0;
    int32_t ic_iter_num = 0;
    int32_t act_type = 0;
    int32_t exec_type = 0;
    int32_t shift_res = 0;

    int32_t total_iter_num = 0;  // to do

    // uint32_t img_wnd_byte_size = ic * (src_w + src_w -2 ) * src_h;
    cout << " ========== misc mode: test  =============== " << endl;
    // input window is different from conv/macc
    int32_t oneIterSize = ICin1Iter * IWin1Iter * IHin1Iter;

    total_iter_num = ICIter_NUM * OHIter_NUM * OWIter_NUM;  // to do
    ofm_offset = int(16 * 1024 - ofm_wnd_byte_size) / 4;

    incS0 = oneIterSize / 4;

    layer_param[0] = incS0;
    layer_param[1] = numAL1;
    layer_param[2] = incAL1;
    layer_param[3] = numAL2;
    layer_param[4] = incAL2;
    layer_param[5] = incAL3;
    layer_param[6] = numAO1;
    layer_param[7] = incAO1;
    layer_param[8] = numAO2;
    layer_param[9] = incAO2;
    layer_param[10] = incAO3;
    layer_param[11] = numB;
    layer_param[12] = incB2;
    layer_param[13] = numBias;
    layer_param[14] = incBias2;
    layer_param[15] = numOFM1;
    layer_param[16] = incOFM1;
    layer_param[17] = numOFM2;
    layer_param[18] = incOFM2;
    layer_param[19] = incOFM3;
    layer_param[20] = total_iter_num;
    layer_param[21] = ic_iter_num;
    layer_param[22] = outer_count;
    layer_param[23] = inner_count;
    layer_param[24] = shift_res;
    layer_param[25] = shift_bias;
    layer_param[26] = ofm_offset;
    layer_param[27] = bias_offset;
    layer_param[28] = act_type;
    layer_param[29] = exec_type;
  }
};

}  // namespace alu
}  // namespace xv2dpu
}  // namespace cosim