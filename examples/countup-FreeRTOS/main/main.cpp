/**
 * @file main.cpp
 * @author Omegaki113r
 * @date Sunday, 19th January 2025 12:42:20 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: main
 * File Name: main.cpp
 * File Created: Sunday, 19th January 2025 12:42:20 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 8th February 2025 4:59:39 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstdio>

#include "OmegaChronoController/ChronoCountupController.hpp"
#include "OmegaChronoController/ChronoFreeRTOSController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

auto countup = ::Omega::Chrono::Countup(::Omega::Chrono::FreeRTOS())
                   .name("Countup")
                   .delay({0})
                   .update_duration({0, 0, 1})
                   .duration({0, 1});

extern "C" void app_main(void)
{

    {
        const auto on_start = [&](const char *name)
        { OMEGA_LOGI("[%s] started", name); };
        const auto on_resume = [&](const char *name)
        { OMEGA_LOGI("[%s] resumed", name); };
        const auto on_update = [&](const char *name, const ::Omega::Chrono::Duration &duration)
        { OMEGA_LOGI("[%s] Update: %.2d:%.2d:%.2d.%.3d.%.3lld",name, duration.h, duration.m, duration.s, duration.ms, duration.us); return true; };
        const auto on_pause = [&](const char *name)
        { OMEGA_LOGI("[%s] paused", name); };
        const auto on_stop = [&](const char *name)
        { OMEGA_LOGI("[%s] stopped", name); };

        countup.add_on_start_callback(on_start);
        countup.add_on_resume_callback(on_resume);
        countup.add_on_update_callback(on_update);
        countup.add_on_pause_callback(on_pause);
        countup.add_on_stop_callback(on_stop);
        if (const auto err = countup.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", countup.get_name());
        }
    }
}