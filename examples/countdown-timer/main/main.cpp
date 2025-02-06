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
 * Last Modified: Thursday, 6th February 2025 9:05:24 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstdio>

#include "OmegaChronoController/ChronoFreeRTOSController.hpp"
#include "OmegaChronoController/ChronoPeriodicController.hpp"
#include "OmegaChronoController/ChronoSingleshotController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

auto periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                    .name("Periodic")
                    .delay({0})
                    .update_duration({0, 0, 1})
                    .duration({0, 0, 10});

extern "C" void app_main(void)
{

    {
        const auto on_start = [&](const char *name)
        { OMEGA_LOGI("started"); };
        const auto on_resume = [&](const char *name)
        { OMEGA_LOGI("resumed"); };
        const auto on_update = [&](const char *name, const ::Omega::Chrono::Duration &duration)
        { OMEGA_LOGI("Update: %.2d:%.2d:%.2d.%.3d.%.3lld", duration.h, duration.m, duration.s, duration.ms, duration.us); return true; };
        const auto on_pause = [&](const char *name)
        { OMEGA_LOGI("paused"); };
        const auto on_stop = [&](const char *name)
        { OMEGA_LOGI("stopped"); };

        auto singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::FreeRTOS())
                              .name("Singleshot")
                              .delay({0})
                              .duration({0, 0, 10});
        singleshot.add_on_start_callback(on_start);
        singleshot.add_on_resume_callback(on_resume);
        singleshot.add_on_update_callback(on_update);
        singleshot.add_on_pause_callback(on_pause);
        singleshot.add_on_stop_callback(on_stop);
        if (const auto err = singleshot.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", singleshot.get_name());
        }

        periodic.add_on_start_callback(on_start);
        periodic.add_on_resume_callback(on_resume);
        periodic.add_on_update_callback(on_update);
        periodic.add_on_pause_callback(on_pause);
        periodic.add_on_stop_callback(on_stop);
        if (const auto err = periodic.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", periodic.get_name());
        }
        delay(5 * 1000);
        periodic.pause();
        delay(5 * 1000);
        periodic.resume();
        delay(5 * 1000);
        periodic.stop();
    }
}