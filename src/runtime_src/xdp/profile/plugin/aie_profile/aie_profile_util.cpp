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

#include "aie_profile_util.h"

// ***************************************************************
// Anonymous namespace for helper functions local to this file
// ***************************************************************
namespace xdp::aie_profile {
  
using severity_level = xrt_core::message::severity_level;

bool
isStreamSwitchPortEvent(const XAie_Events event)
{
  return (std::find(mSSEventList.begin(), mSSEventList.end(), event) != mSSEventList.end());
}

void 
configEventSelections(const XAie_LocType loc,
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

  std::stringstream msg;
  msg << "Configured mem tile selection " << (aie::isInputSet(type, metricSet) ? "S2MM" : "MM2S") << "DMA for metricset " << metricSet << " and channels " << (int)channel0 << "and" << (int) channel1<< ".";
  xrt_core::message::send(severity_level::debug, "XRT", msg.str());
}

bool isValidType(module_type type, XAie_ModuleType mod)
{
  if ((mod == XAIE_CORE_MOD) && ((type == module_type::core) 
      || (type == module_type::dma)))
    return true;
  if ((mod == XAIE_MEM_MOD) && ((type == module_type::dma) 
      || (type == module_type::mem_tile)))
    return true;
  if ((mod == XAIE_PL_MOD) && (type == module_type::shim)) 
    return true;
  return false;
}

module_type getModuleType(uint16_t absRow, uint16_t rowOffset, XAie_ModuleType mod)
{
  if (absRow == 0)
    return module_type::shim;
  if (absRow < rowOffset)
    return module_type::mem_tile;
  return ((mod == XAIE_CORE_MOD) ? module_type::core : module_type::dma);
}

  void 
  configGroupEvents(const XAie_LocType loc,
                    const XAie_ModuleType mod, const module_type type,
                    const std::string metricSet, const XAie_Events event,
                    const uint8_t channel)  
  {
    // Set masks for group events
    // NOTE: Group error enable register is blocked, so ignoring
    if (event == XAIE_EVENT_GROUP_DMA_ACTIVITY_MEM)
      XAie_EventGroupControl(aieDevInst, loc, mod, event, GROUP_DMA_MASK);
    else if (event == XAIE_EVENT_GROUP_LOCK_MEM)
      XAie_EventGroupControl(aieDevInst, loc, mod, event, GROUP_LOCK_MASK);
    else if (event == XAIE_EVENT_GROUP_MEMORY_CONFLICT_MEM)
      XAie_EventGroupControl(aieDevInst, loc, mod, event, GROUP_CONFLICT_MASK);
    else if (event == XAIE_EVENT_GROUP_CORE_PROGRAM_FLOW_CORE)
      XAie_EventGroupControl(aieDevInst, loc, mod, event, GROUP_CORE_PROGRAM_FLOW_MASK);
    else if (event == XAIE_EVENT_GROUP_CORE_STALL_CORE)
      XAie_EventGroupControl(aieDevInst, loc, mod, event, GROUP_CORE_STALL_MASK);
    else if (event == XAIE_EVENT_GROUP_DMA_ACTIVITY_PL) {
      uint32_t bitMask = aie::isInputSet(type, metricSet) 
          ? ((channel == 0) ? GROUP_SHIM_S2MM0_STALL_MASK : GROUP_SHIM_S2MM1_STALL_MASK)
          : ((channel == 0) ? GROUP_SHIM_MM2S0_STALL_MASK : GROUP_SHIM_MM2S1_STALL_MASK);
      XAie_EventGroupControl(aieDevInst, loc, mod, event, bitMask);
    }
  }


}