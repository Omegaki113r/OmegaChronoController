/**
 * @file ChronoESP32xxGPController.hpp
 * @author Omegaki113r
 * @date Wednesday, 12th February 2025 12:04:30 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: AL
 * File Name: ChronoESP32xxGPController.hpp
 * File Created: Wednesday, 12th February 2025 12:04:30 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 13th February 2025 3:42:07 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <driver/gptimer.h>

#include "OmegaChronoController/Abstraction/AbstractionBase.hpp"
#include "OmegaChronoController/Core/Callbacks.hpp"
#include "OmegaChronoController/Core/CoreBase.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class ESP32xxGP : public Core
        {
        public:
            ESP32xxGP() {}
            ~ESP32xxGP() {}

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
            Type m_type = Type::eSINGLE;
            Duration m_duration;
            Duration m_update_period;
            gptimer_handle_t m_handle;
        };
    } // namespace Chrono
} // namespace Omega