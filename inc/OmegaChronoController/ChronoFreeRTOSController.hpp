/**
 * @file ChronoFreeRTOSController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:51:27 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoFreeRTOSController.hpp
 * File Created: Wednesday, 29th January 2025 4:51:27 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 6th February 2025 5:43:59 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/timers.h>

#include "OmegaChronoController/ChronoBase.hpp"
#include "OmegaChronoController/ChronoCallbacks.hpp"
#include "OmegaChronoController/ChronoCore.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class FreeRTOS : public Core
        {
        public:
            FreeRTOS() {}
            ~FreeRTOS();

            void set_handle(TimerHandle_t timer_handle) noexcept { handle = timer_handle; }

            OmegaStatus start(Duration update_period, Duration duration) noexcept override;
            OmegaStatus resume() noexcept override;
            OmegaStatus pause() noexcept override;
            OmegaStatus stop() noexcept override;

        private:
            TimerHandle_t handle{0};
        };
    } // namespace Chrono
} // namespace Omega
