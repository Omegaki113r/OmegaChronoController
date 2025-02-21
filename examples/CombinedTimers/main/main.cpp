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
 * Last Modified: Friday, 21st February 2025 2:06:50 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstdio>

#include "OmegaChronoController/Abstraction/ESP32xxGPController.hpp"
#include "OmegaChronoController/Abstraction/ESP32xxHiResController.hpp"
#include "OmegaChronoController/Abstraction/FreeRTOSController.hpp"
#include "OmegaChronoController/Core/CountdownController.hpp"
#include "OmegaChronoController/Core/CountupController.hpp"
#include "OmegaChronoController/Core/PeriodicController.hpp"
#include "OmegaChronoController/Core/SingleshotController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

auto freertos = ::Omega::Chrono::Countup(::Omega::Chrono::FreeRTOS())
                    .name("freertos")
                    .delay({0})
                    .update_period({0, 0, 0, 100})
                    .duration({24, 0, 0});
auto general_purpose = ::Omega::Chrono::Countup(::Omega::Chrono::ESP32xxGP())
                           .name("general")
                           .delay({0})
                           .update_period({0, 0, 0, 100})
                           .duration({24, 0, 0});
auto high_res = ::Omega::Chrono::Countup(::Omega::Chrono::ESP32xxHiRes())
                    .name("high res")
                    .delay({0})
                    .update_period({0, 0, 0, 100})
                    .duration({24, 0, 0});

extern "C" void app_main(void)
{

    {
        const auto on_start = [&](const char *name)
        { OMEGA_LOGI("[%s] started", name); };
        const auto on_resume = [&](const char *name)
        { OMEGA_LOGI("[%s] resumed", name); };
        const auto on_update = [&](const char *name, const Duration &duration)
        { OMEGA_LOGI("[%s] Update: %.2d:%.2d:%.2d.%.3d.%.3lld",name, duration.h, duration.m, duration.s, duration.ms, duration.us); return true; };
        const auto on_pause = [&](const char *name)
        { OMEGA_LOGI("[%s] paused", name); };
        const auto on_stop = [&](const char *name)
        { OMEGA_LOGI("[%s] stopped", name); };

        freertos.add_on_start_callback(on_start);
        freertos.add_on_resume_callback(on_resume);
        freertos.add_on_update_callback(on_update);
        freertos.add_on_pause_callback(on_pause);
        freertos.add_on_stop_callback(on_stop);
        if (const auto err = freertos.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", freertos.get_name());
        }

        general_purpose.add_callbacks(on_update, on_start, on_resume, on_pause, on_stop);
        if (const auto err = general_purpose.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", general_purpose.get_name());
        }

        high_res.add_callbacks(on_update, on_start, on_resume, on_pause, on_stop);
        if (const auto err = high_res.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", high_res.get_name());
        }
    }
    for (;;)
    {
        delay({0, 0, 0, 20});
    }
}