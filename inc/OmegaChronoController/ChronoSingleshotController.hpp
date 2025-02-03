/**
 * @file ChronoSingleshotController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:19:29 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoSingleshotController.hpp
 * File Created: Wednesday, 29th January 2025 4:19:29 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 1:08:23 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/ChronoBaseController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        template <typename T>
        class Singleshot : public Base
        {
            T core;

        public:
            Singleshot(T in_core) : core(in_core) {}

            OmegaStatus start() noexcept override { return start(delay, update_period, duration, on_start, on_update, on_end); }

            OmegaStatus start(const Duration &delay, const Duration &update_period, const Duration &duration,
                              std::function<void(void)> on_start, std::function<void(const ::Omega::Chrono::Duration &)> on_update, std::function<void(void)> on_end) noexcept override
            {
                const auto on_start_handler = [this]()
                {
                    OMEGA_LOGI("[%s] => on start", get_name());
                    if (this->on_start)
                        this->on_start();
                };
                const auto on_update_handler = [this](const ::Omega::Chrono::Duration &duration)
                {
                    OMEGA_LOGI("[%s] => on update | [%.2d:%.2d:%.2d.%.3d.%.3lld]", get_name(), duration.h, duration.m, duration.s, duration.ms, duration.us);
                    if (this->on_update)
                        this->on_update(duration);
                };
                const auto on_end_handler = [this]()
                {
                    OMEGA_LOGI("[%s] => on end", get_name());
                    if (this->on_end)
                        this->on_end();
                };
                return core.start(delay, update_period, duration, on_start_handler, on_update_handler, on_end_handler);
            }
            void start_immediate() noexcept override
            {
                core.start_immediate(delay, update_period, duration, on_start, on_update, on_end);
            }

            void start_immediate(const Duration &delay, const Duration &update_period, const Duration &duration,
                                 std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept override
            {
                const auto on_start_handler = [this]()
                {
                    OMEGA_LOGI("[%s] => on start", get_name());
                    if (on_start)
                        on_start();
                };
                const auto on_update_handler = [this](const ::Omega::Chrono::Duration &duration)
                {
                    OMEGA_LOGI("[%s] => on update | [%.2d:%.2d:%.2d.%.3d.%.3lld]", get_name(), duration.h, duration.m, duration.s, duration.ms, duration.us);
                    if (on_update)
                        on_update(duration);
                };
                const auto on_end_handler = [this]()
                {
                    OMEGA_LOGI("[%s] => on end", get_name());
                    if (on_end)
                        on_end();
                };
                core.start_immediate(delay, update_period, duration, on_start_handler, on_update_handler, on_end_handler);
            }
            OmegaStatus pause() noexcept override
            {
                return eFAILED;
            }
            OmegaStatus resume() noexcept override { return eFAILED; }
            OmegaStatus stop() noexcept override { return eFAILED; }
        };
    } // namespace Chrono
} // namespace Omega