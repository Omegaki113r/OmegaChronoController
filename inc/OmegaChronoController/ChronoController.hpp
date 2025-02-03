/**
 * @file ChronoController.hpp
 * @author Omegaki113r
 * @date Monday, 27th January 2025 8:21:55 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoController.hpp
 * File Created: Monday, 27th January 2025 8:21:55 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 12:30:28 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include <cstring>
#include <functional>
#include <utility>
#include <variant>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include "OmegaChronoController/ChronoBaseController.hpp"
#include "OmegaChronoController/ChronoCore.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

#ifdef __cplusplus
namespace Omega
{
    namespace Chrono
    {
        template <typename T>
        class Controller
        {
        private:
            Type type;
            T core;

            char name[50 + 1];
            Duration duration;
            Duration update_period;
            Duration delay;
            std::function<void(void)> on_start;
            std::function<void(const ::Omega::Chrono::Duration &)> on_update;
            std::function<void(void)> on_end;
            State state{State::eIDLE};

        public:
            Controller(Type in_type, T in_core) : type(in_type), core(in_core) {}
            ~Controller() {}

            constexpr inline void set_name(const char *in_name)
            {
                if (nullptr == in_name || 0 == std::strlen(in_name))
                    return;
                UNUSED(std::memcpy(name, in_name, OMEGA_MIN(std::strlen(in_name), sizeof(name))));
            }
            constexpr inline void set_duration(Duration in_duration) noexcept { duration = in_duration; }
            constexpr inline void set_update_period(Duration in_update_period) noexcept { update_period = in_update_period; }
            constexpr inline void set_delay(Duration in_delay) noexcept { delay = in_delay; }
            constexpr inline void add_on_start_callback(std::function<void(void)> in_callback) { on_start = in_callback; }
            constexpr inline void add_on_update_callback(std::function<void(const ::Omega::Chrono::Duration &)> in_callback) noexcept { on_update = in_callback; }
            constexpr inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept { on_end = in_callback; }

            constexpr inline const char *get_name() const noexcept { return name; }
            constexpr inline Duration get_duration() const noexcept { return duration; }
            constexpr inline Duration get_update_period() const noexcept { return update_period; }
            constexpr inline Duration get_delay() const noexcept { return delay; }
            constexpr inline std::function<void(void)> get_start() const noexcept { return on_start; }
            constexpr inline std::function<void(const ::Omega::Chrono::Duration &)> get_update() const noexcept { return on_update; }
            constexpr inline std::function<void(void)> get_end() const noexcept { return on_end; }

            OmegaStatus start() noexcept
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
                return core.start(delay, update_period, duration, on_start_handler, on_update_handler, on_end_handler);
            }

            void start_immediate() noexcept
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
            };
        };
    } // namespace Chrono
} // namespace Omega
#else
#endif