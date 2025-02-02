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
 * Last Modified: Sunday, 2nd February 2025 8:48:28 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstdio>

#include "OmegaChronoController/ChronoController.hpp"
#include "OmegaChronoController/ChronoFreeRTOSController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

::Omega::Chrono::FreeRTOS countdown;

extern "C" void app_main(void)
{

    {
        const auto on_start = [&]()
        { OMEGA_LOGI("started"); };
        const auto on_update = [&](const ::Omega::Chrono::Duration &duration)
        { OMEGA_LOGI("update: %ld", duration.ms); };
        const auto on_end = [&]()
        { OMEGA_LOGI("end"); };

        countdown.set_name("Count Down");
        countdown.set_delay(::Omega::Chrono::Duration(30_s));
        countdown.set_duration(::Omega::Chrono::Duration(25_s));
        countdown.set_update_period(::Omega::Chrono::Duration(1_s));
        countdown.add_on_start_callback(on_start);
        countdown.add_on_update_callback(on_update);
        countdown.add_on_end_callback(on_end);
        countdown.start_immediate();
        // countdown.start();

        // delay(15 * 1000);
    }
}