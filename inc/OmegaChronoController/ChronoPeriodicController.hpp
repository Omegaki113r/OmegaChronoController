/**
 * @file ChronoPeriodicController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:20:06 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoPeriodicController.hpp
 * File Created: Wednesday, 29th January 2025 4:20:06 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 5th February 2025 12:43:43 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/ChronoBase.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        template <typename T>
        class Periodic : public Base
        {
            T core;

        public:
            constexpr Periodic(T in_core) : core(in_core) {}

            constexpr Periodic &name(const char *in_name) noexcept override
            {
                set_name(in_name);
                return *this;
            }

            constexpr Periodic &delay(Duration in_delay) noexcept override
            {
                set_delay(in_delay);
                return *this;
            }

            constexpr Periodic &update_duration(Duration in_update_period) noexcept override
            {
                set_update_period(in_update_period);
                return *this;
            }

            constexpr Periodic &duration(Duration in_duration) noexcept override
            {
                set_duration(in_duration);
                return *this;
            }

            OmegaStatus start() noexcept override
            {
                const auto timer_task = [](void *arg)
                {
                    Periodic *controller = (Periodic *)arg;
                    if (Duration{0} < controller->m_delay)
                    {
                        const auto on_delay_start = controller->get_on_delay_start_handler();
                        if (on_delay_start)
                            on_delay_start();
                        OMEGA_LOGD("[%s] => %.2d:%.2d:%.2d.%.3d.%.3lld", controller->m_name,
                                   controller->m_delay.h, controller->m_delay.m, controller->m_delay.s, controller->m_delay.ms, controller->m_delay.us);
                        controller->core.start(controller->m_delay, controller->m_delay, controller->m_on_update);
                        const auto on_delay_end = controller->get_on_delay_end_handler();
                        if (on_delay_end)
                            on_delay_end();
                    }
                    if (Duration(0) < controller->m_duration)
                    {
                        const auto on_start = controller->get_on_start_handler();
                        if (on_start)
                            on_start();
                        OMEGA_LOGD("[%s] => %.2d:%.2d:%.2d.%.3d.%.3lld", controller->m_name,
                                   controller->m_duration.h, controller->m_duration.m, controller->m_duration.s, controller->m_duration.ms, controller->m_duration.us);
                        controller->core.start(controller->m_update_period, controller->m_duration, controller->m_on_update);
                        const auto on_end = controller->get_on_end_handler();
                        if (on_end)
                            on_end();
                    }
                    vTaskDelete(nullptr);
                };
                if (const auto err = xTaskCreate(timer_task, "timer_task", configMINIMAL_STACK_SIZE * 2, this, configMAX_PRIORITIES - 2, nullptr); pdPASS != err)
                    return eFAILED;
                return eSUCCESS;
            }

            OmegaStatus pause() noexcept override { return eFAILED; }

            OmegaStatus resume() noexcept override { return eFAILED; }

            OmegaStatus stop() noexcept override { return eFAILED; }
        };
    } // namespace Chrono
} // namespace Omega