/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
   Copyright (C) 2020-2021 The exTHmUI Project.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream> 
#include <android-base/properties.h>

#define THERMAL_SCONFIG "/sys/class/thermal/thermal_message/sconfig"
#define THERMAL_LEVEL_DEFAULT 0
#define THERMAL_LEVEL_GAMING 13
#define THERMAL_LEVEL_BENCHMARK 10

#define POWERCFG_EXEC "/system/bin/sh /data/powercfg.sh"

#define PERFORMANCE_LEVEL_PROP "sys.performance.level"

int main(void)
{
    /* 取得当前模式 */
    const int current = android::base::GetIntProperty(PERFORMANCE_LEVEL_PROP, 3);

    /* 温控配置 */
    std::ofstream thermal;
    thermal.open(THERMAL_SCONFIG, std::ios::out); 
    if (current <= 4) {
        thermal << THERMAL_LEVEL_DEFAULT << std::endl;
    } else if (current == 5) {
        thermal << THERMAL_LEVEL_GAMING << std::endl;
    } else if (current == 6) {
        thermal << THERMAL_LEVEL_BENCHMARK << std::endl;
    }
    thermal.close();

    /* 调度 */
    std::ostringstream oss;
    oss << POWERCFG_EXEC << " ";
    if (current == -1 || current == 3) {
        oss << "balance" << std::endl;
    } else if (current < 3) {
        oss << "powersave" << std::endl;
    } else if (current <= 5) {
        oss << "performance" << std::endl;
    } else if (current == 6) {
        oss << "fast" << std::endl;
    }
    system(oss.str().c_str());

    return 0;
}
