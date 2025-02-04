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
 * Last Modified: Wednesday, 5th February 2025 12:53:06 am
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

auto singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::FreeRTOS())
                      .name("Singleshot")
                      .delay({0})
                      .update_duration({0, 0, 1})
                      .duration({0, 0, 10});

auto periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                    .name("Periodic")
                    .delay({0})
                    .update_duration({0, 0, 1})
                    .duration({0, 5, 0});

extern "C" void app_main(void)
{

    {
        const auto on_start = [&]()
        { OMEGA_LOGI("started"); };
        const auto on_update = [&](const ::Omega::Chrono::Duration &duration)
        { OMEGA_LOGI("Update: %.2d:%.2d:%.2d.%.3d.%.3lld", duration.h, duration.m, duration.s, duration.ms, duration.us); };
        const auto on_end = [&]()
        { OMEGA_LOGI("end"); };

        // singleshot.add_on_start_callback(on_start);
        // singleshot.add_on_update_callback(on_update);
        // singleshot.add_on_end_callback(on_end);
        // if (const auto err = singleshot.start(); err != eSUCCESS)
        // {
        //     OMEGA_LOGE("%s start failed", singleshot.get_name());
        // }
        // OMEGA_LOGI("[%s]", singleshot.get_name());

        periodic.add_on_start_callback(on_start);
        periodic.add_on_update_callback(on_update);
        periodic.add_on_end_callback(on_end);
        if (const auto err = periodic.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", periodic.get_name());
        }
        OMEGA_LOGI("[%s]", periodic.get_name());
        // delay(15 * 1000);
    }
}