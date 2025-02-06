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
 * Last Modified: Thursday, 6th February 2025 10:20:29 pm
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
#include "OmegaChronoController/ChronoCallbacks.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        template <typename T>
        class Periodic : public Base
        {
            T core;
            Duration elapsed_duration{0};

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
                if (Duration{0} == m_duration)
                {
                    OMEGA_LOGE("Invalid Duration");
                    return eFAILED;
                }
                if (Duration{0} == m_update_period)
                {
                    OMEGA_LOGE("Invalid Update Period");
                    return eFAILED;
                }
                const auto timer_task = [](void *arg)
                {
                    Periodic *controller = (Periodic *)arg;
                    if (Duration{0} < controller->m_delay)
                    {
                        const auto on_start = [&](const char *name)
                        {
                            const auto on_delay_start = controller->get_on_delay_start_handler();
                            if (on_delay_start)
                                on_delay_start(controller->m_name);
                        };
                        const auto on_end = [&](const char *name)
                        {
                            const auto on_delay_end = controller->get_on_delay_stopped_handler();
                            if (on_delay_end)
                                on_delay_end(controller->m_name);
                        };
                        controller->core.add_on_start_callback(on_start);
                        controller->core.add_on_stop_callback(on_end);
                        controller->core.start(controller->m_delay, controller->m_delay);
                    }
                    if (Duration(0) < controller->m_duration)
                    {
                        const auto on_start = [&](const char *name)
                        {
                            const auto on_start_handler = controller->get_on_start_handler();
                            if (on_start_handler)
                                on_start_handler(controller->m_name);
                        };
                        const auto on_update = [&](const char *name, const Duration duration)
                        {
                            controller->elapsed_duration = controller->elapsed_duration + controller->m_update_period;
                            const auto on_update_handler = controller->get_on_update_handler();
                            if (on_update_handler)
                                on_update_handler(controller->m_name, controller->elapsed_duration);
                            return false;
                        };
                        controller->core.add_on_start_callback(on_start);
                        controller->core.add_on_update_callback(on_update);
                        controller->core.start(controller->m_update_period, controller->m_duration);
                    }
                    vTaskDelete(nullptr);
                };
                if (const auto err = xTaskCreate(timer_task, "timer_task", configMINIMAL_STACK_SIZE * 2, this, configMAX_PRIORITIES - 2, nullptr); pdPASS != err)
                    return eFAILED;
                return eSUCCESS;
            }

            OmegaStatus pause() noexcept override
            {
                const auto state = core.pause();
                if (m_on_paused)
                    m_on_paused(m_name);
                return state;
            }

            OmegaStatus resume() noexcept override
            {
                const auto state = core.resume();
                if (m_on_resumed)
                    m_on_resumed(m_name);
                return state;
            }

            OmegaStatus stop() noexcept override
            {
                const auto state = core.stop();
                if (m_on_stopped)
                    m_on_stopped(m_name);
                return state;
            }
        };
    } // namespace Chrono
} // namespace Omega