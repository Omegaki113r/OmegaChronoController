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
 * Last Modified: Tuesday, 28th January 2025 1:56:29 am
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
#include "OmegaUtilityDriver/UtilityDriver.hpp"

extern "C" void app_main(void)
{

    {
        const auto on_start = [&]()
        { OMEGA_LOGD("started"); };
        const auto on_update = [&]()
        { OMEGA_LOGD("update"); };
        const auto on_end = [&]()
        { OMEGA_LOGD("end"); };

        ::Omega::Chrono::FreeRTOS countdown;
        countdown.set_delay(::Omega::Chrono::Duration(10000));
        countdown.set_duration(::Omega::Chrono::Duration(10000));
        countdown.add_on_start_callback(on_start);
        countdown.add_on_update_callback(on_update);
        countdown.add_on_end_callback(on_end);
        OMEGA_LOGI("%ld", countdown.get_duration().ms);
        countdown.start();

        delay(10_s);
    }
}