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
 * Last Modified: Saturday, 8th February 2025 6:44:54 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstdio>

#include "OmegaChronoController/AL/ChronoESP32xxHiResController.hpp"
#include "OmegaChronoController/Base/ChronoCountdownController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

auto countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::ESP32xxHiRes())
                     .name("Countdown")
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

        countdown.add_on_start_callback(on_start);
        countdown.add_on_resume_callback(on_resume);
        countdown.add_on_update_callback(on_update);
        countdown.add_on_pause_callback(on_pause);
        countdown.add_on_stop_callback(on_stop);
        if (const auto err = countdown.start(); err != eSUCCESS)
        {
            OMEGA_LOGE("%s start failed", countdown.get_name());
        }

        delay_s(5);
        if (const auto state = countdown.pause(); eSUCCESS != state)
        {
            OMEGA_LOGE("countdown pause failed");
            return;
        }
        delay_s(5);
        if (const auto state = countdown.resume(); eSUCCESS != state)
        {
            OMEGA_LOGE("countdown resume failed");
            return;
        }
        delay_s(5);
        if (const auto state = countdown.stop(); eSUCCESS != state)
        {
            OMEGA_LOGE("countdown stop failed");
            return;
        }
    }
}