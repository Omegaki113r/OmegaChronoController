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
 * Last Modified: Tuesday, 28th January 2025 1:55:11 am
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

#include "OmegaUtilityDriver/UtilityDriver.hpp"

#ifdef __cplusplus

namespace Omega
{
    namespace Chrono
    {
        enum class Type
        {
            ePERIODIC,
            eSINGLE_SHOT,
            eCOUNTDOWN,
            eCOUNTUP
        };

        enum class State
        {
            eIDLE,
            eCREATED,
            eSTARTED,
            ePAUSED,
            eSTOPPED,
            eDELETED,
        };

        struct Duration
        {
            u64 us;
            u32 ms;
            u32 s;
            u16 m;
            u16 h;

            Duration(u64 in_duration_us);
            bool operator==(const Duration &other) const
            {
                return h == other.h && m == other.m && s == other.s && ms == other.ms && us == other.us;
            }
        };

        class FreeRTOS
        {
        public:
            FreeRTOS(Type in_type = Type::eSINGLE_SHOT, Duration in_duration = {0}, Duration in_update_period = {0}, Duration in_delay = {0})
                : type(in_type), duration(in_duration), update_period(in_update_period), delay(in_delay) {}
            ~FreeRTOS();

            inline void set_type(Type in_type) noexcept { type = in_type; }
            inline void set_duration(Duration in_duration) noexcept { duration = in_duration; }
            inline void set_update_period(Duration in_update_period) noexcept { update_period = in_update_period; }
            inline void set_delay(Duration in_delay) noexcept { delay = in_delay; }
            inline void add_on_start_callback(std::function<void(void)> in_callback) noexcept { on_start = in_callback; }
            inline void add_on_update_callback(std::function<void(void)> in_callback) noexcept { on_update = in_callback; }
            inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept { on_end = in_callback; }
            OmegaStatus start() noexcept;
            OmegaStatus pause() noexcept;
            OmegaStatus resume() noexcept;
            OmegaStatus stop() noexcept;

            inline Type get_type() const noexcept { return type; }
            inline Duration get_duration() const noexcept { return duration; }
            inline Duration get_update_period() const noexcept { return update_period; }
            inline Duration get_delay() const noexcept { return delay; }

            FreeRTOS(FreeRTOS &other) = delete;
            FreeRTOS &operator=(FreeRTOS &) = delete;
            FreeRTOS(FreeRTOS &&) = delete;
            FreeRTOS &operator=(FreeRTOS &&) = delete;

        private:
            Type type;
            Duration duration;
            Duration update_period;
            Duration delay;
            std::function<void(void)> on_start;
            std::function<void(void)> on_update;
            std::function<void(void)> on_end;
            State state{State::eIDLE};
            TimerHandle_t handle;
        };
    } // namespace Chrono
} // namespace Omega
#else
#endif