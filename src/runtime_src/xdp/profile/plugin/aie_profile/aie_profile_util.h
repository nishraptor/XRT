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

#ifndef AIE_PROFILE_UTIL_DOT_H
#define AIE_PROFILE_UTIL_DOT_H

#include "xdp/profile/database/static_info/aie_constructs.h"

extern "C" {
#include <xaiengine.h>
#include <xaiengine/xaiegbl_params.h>
}

namespace xdp::aie_profile {

std::vector<XAie_Events> mSSEventList = {
        XAIE_EVENT_PORT_RUNNING_0_CORE,
        XAIE_EVENT_PORT_STALLED_0_CORE,
        XAIE_EVENT_PORT_RUNNING_0_PL,
        XAIE_EVENT_PORT_RUNNING_0_MEM_TILE,
        XAIE_EVENT_PORT_STALLED_0_MEM_TILE,
        XAIE_EVENT_PORT_TLAST_0_MEM_TILE
      };

bool isStreamSwitchPortEvent(const XAie_Events event);
void configEventSelections(const XAie_LocType loc,
                        const module_type type,
                        const std::string metricSet,
                        const uint8_t channel0,
                        const uint8_t channel1); 
bool isValidType(module_type type, XAie_ModuleType mod);
module_type getModuleType(uint16_t absRow, uint16_t rowOffset, XAie_ModuleType mod);

                       
} // namespace xdp::aie

#endif
