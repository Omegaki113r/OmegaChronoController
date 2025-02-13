/**
 * @file ChronoESP32xxHiResController.hpp
 * @author Omegaki113r
 * @date Saturday, 8th February 2025 5:20:36 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: AL
 * File Name: ChronoESP32xxHiResController.hpp
 * File Created: Saturday, 8th February 2025 5:20:36 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 13th February 2025 3:42:24 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <esp_timer.h>

#include "OmegaChronoController/Abstraction/AbstractionBase.hpp"
#include "OmegaChronoController/Core/Callbacks.hpp"
#include "OmegaChronoController/Core/CoreBase.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class ESP32xxHiRes : public Core
        {
        public:
            ESP32xxHiRes() {}
            ~ESP32xxHiRes() {}

            OmegaStatus start(Duration update_period, Duration duration) noexcept override;
            OmegaStatus resume() noexcept override;
            OmegaStatus pause() noexcept override;
            OmegaStatus stop() noexcept override;

        private:
            enum class Type
            {
                eSINGLE,
                ePERIODIC
            };
            Type type = Type::eSINGLE;
            Duration duration;
            Duration update_period;
            esp_timer_handle_t handle;
        };
    } // namespace Chrono
} // namespace Omega
