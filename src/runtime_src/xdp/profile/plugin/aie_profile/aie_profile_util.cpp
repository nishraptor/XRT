/**
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. - All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#define XDP_SOURCE


// ***************************************************************
// Anonymous namespace for helper functions local to this file
// ***************************************************************
namespace xdp::aie_profile {
  
namespace pt = boost::property_tree;
using severity_level = xrt_core::message::severity_level;

  void configEventSelections(const XAie_LocType loc,
                            const XAie_ModuleType mod,
                            const module_type type,
                            const std::string metricSet,
                            const uint8_t channel0,
                            const uint8_t channel1) 
  {
    if (type != module_type::mem_tile)
      return;

    XAie_DmaDirection dmaDir = aie::isInputSet(type, metricSet) ? DMA_S2MM : DMA_MM2S;
    XAie_EventSelectDmaChannel(aieDevInst, loc, 0, dmaDir, channel0);
    XAie_EventSelectDmaChannel(aieDevInst, loc, 1, dmaDir, channel1);
  }


}