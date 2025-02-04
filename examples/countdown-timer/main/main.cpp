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
 * Last Modified: Tuesday, 4th February 2025 4:09:30 pm
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
#include "OmegaChronoController/ChronoSingleshotController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

auto singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::FreeRTOS());

extern "C" void app_main(void)
{

    {
        const auto on_start = [&]()
        { OMEGA_LOGI("started"); };
        const auto on_update = [&](const ::Omega::Chrono::Duration &duration)
        { OMEGA_LOGI("Update: %.2d:%.2d:%.2d.%.3d.%.3lld", duration.h, duration.m, duration.s, duration.ms, duration.us); };
        const auto on_end = [&]()
        { OMEGA_LOGI("end"); };

        singleshot.set_name("Count Down");
        singleshot.set_delay(::Omega::Chrono::Duration(0, 0, 10));
        singleshot.set_duration(::Omega::Chrono::Duration(0, 1));
        singleshot.set_update_period(::Omega::Chrono::Duration(0, 0, 1));
        singleshot.add_on_start_callback(on_start);
        singleshot.add_on_update_callback(on_update);
        singleshot.add_on_end_callback(on_end);
        // singleshot.start_immediate();
        singleshot.start();

        // delay(15 * 1000);
    }
}