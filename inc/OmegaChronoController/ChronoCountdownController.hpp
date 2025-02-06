/**
 * @file ChronoCountdownController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:16:08 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoCountdownController.hpp
 * File Created: Wednesday, 29th January 2025 4:16:08 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 6th February 2025 10:51:27 pm
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
#include "OmegaChronoController/ChronoCore.hpp"
#include "OmegaChronoController/ChronoCountdownController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        template <typename T>
        class Countdown : public Base
        {
            T core;
            Duration elapsed_duration{0};

        public:
            constexpr Countdown(T in_core) : core(in_core) {}

            constexpr Countdown &name(const char *in_name) noexcept override
            {
                set_name(in_name);
                return *this;
            }

            constexpr Countdown &delay(Duration in_delay) noexcept override
            {
                set_delay(in_delay);
                return *this;
            }

            constexpr Countdown &update_duration(Duration in_update_period) noexcept override
            {
                set_update_period(in_update_period);
                return *this;
            }

            constexpr Countdown &duration(Duration in_duration) noexcept override
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
                elapsed_duration = m_duration;
                const auto timer_task = [](void *arg)
                {
                    Countdown *controller = (Countdown *)arg;
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
                            controller->elapsed_duration = controller->elapsed_duration - controller->m_update_period;
                            const auto on_update_handler = controller->get_on_update_handler();
                            if (on_update_handler)
                                on_update_handler(controller->m_name, controller->elapsed_duration);
                            if (Duration{0} == controller->elapsed_duration)
                                return true;
                            return false;
                        };
                        const auto on_end = [&](const char *name)
                        {
                            const auto on_stop_handler = controller->get_on_stop_handler();
                            if (on_stop_handler)
                                on_stop_handler(controller->m_name);
                        };
                        controller->core.add_on_start_callback(on_start);
                        controller->core.add_on_update_callback(on_update);
                        controller->core.add_on_stop_callback(on_end);
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
                return state;
            }
        };
    } // namespace Chrono
} // namespace Omega
