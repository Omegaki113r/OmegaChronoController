/**
 * @file ChronoBaseController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:14:46 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoBaseController.hpp
 * File Created: Wednesday, 29th January 2025 4:14:46 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 30th January 2025 9:59:18 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include "OmegaChronoController/ChronoController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

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

            constexpr Duration(u64 in_duration_us) : us(in_duration_us), ms(us / 1000), s(ms / 1000), m(s / 60), h(m / 60) {}

            constexpr bool operator==(const Duration &other) const { return h == other.h && m == other.m && s == other.s && ms == other.ms && us == other.us; }

            constexpr bool operator>(const Duration &other) const
            {
                const auto calculate_total_us = [](const Duration &duration)
                {
                    const auto hour_us = duration.h * 60 * 60 * 1000 * 1000;
                    const auto minutes_us = duration.m * 60 * 1000 * 1000;
                    const auto seconds_us = duration.s * 1000 * 1000;
                    const auto milliseconds_us = duration.ms * 1000;
                    return hour_us + minutes_us + seconds_us + milliseconds_us + duration.us;
                };
                return calculate_total_us(*this) > calculate_total_us(other);
            }

            constexpr bool operator<(const Duration &other) const
            {
                const auto calculate_total_us = [](const Duration &duration)
                {
                    const auto hour_us = duration.h * 60 * 60 * 1000 * 1000;
                    const auto minutes_us = duration.m * 60 * 1000 * 1000;
                    const auto seconds_us = duration.s * 1000 * 1000;
                    const auto milliseconds_us = duration.ms * 1000;
                    return hour_us + minutes_us + seconds_us + milliseconds_us + duration.us;
                };
                return calculate_total_us(*this) < calculate_total_us(other);
            }

            constexpr bool operator<=(const Duration &other) const
            {
                const auto calculate_total_us = [](const Duration &duration)
                {
                    const auto hour_us = duration.h * 60 * 60 * 1000 * 1000;
                    const auto minutes_us = duration.m * 60 * 1000 * 1000;
                    const auto seconds_us = duration.s * 1000 * 1000;
                    const auto milliseconds_us = duration.ms * 1000;
                    return hour_us + minutes_us + seconds_us + milliseconds_us + duration.us;
                };
                return calculate_total_us(*this) <= calculate_total_us(other);
            }

            constexpr bool operator>=(const Duration &other) const
            {
                const auto calculate_total_us = [](const Duration &duration)
                {
                    const auto hour_us = duration.h * 60 * 60 * 1000 * 1000;
                    const auto minutes_us = duration.m * 60 * 1000 * 1000;
                    const auto seconds_us = duration.s * 1000 * 1000;
                    const auto milliseconds_us = duration.ms * 1000;
                    return hour_us + minutes_us + seconds_us + milliseconds_us + duration.us;
                };
                return calculate_total_us(*this) >= calculate_total_us(other);
            }

            constexpr Duration operator-(const Duration &other) const noexcept
            {
                return {this->us - other.us};
            };
        };

        constexpr Duration ZERO{0};

        class Base
        {
        protected:
            char name[50 + 1];
            Type type;
            Duration duration;
            Duration update_period;
            Duration delay;
            std::function<void(void)> on_start;
            std::function<void(const ::Omega::Chrono::Duration &)> on_update;
            std::function<void(void)> on_end;
            State state{State::eIDLE};

            Base(Type in_type = Type::eSINGLE_SHOT, Duration in_duration = {0}, Duration in_update_period = {0}, Duration in_delay = {0})
                : type(in_type), duration(in_duration), update_period(in_update_period), delay(in_delay) {}

            virtual inline void set_name(const char *in_name) = 0;
            virtual inline void set_type(Type in_type) noexcept = 0;
            virtual inline void set_duration(Duration in_duration) noexcept = 0;
            virtual inline void set_update_period(Duration in_update_period) noexcept = 0;
            virtual inline void set_delay(Duration in_delay) noexcept = 0;
            virtual inline void add_on_start_callback(std::function<void(void)> in_callback) = 0;
            virtual inline void add_on_update_callback(std::function<void(const ::Omega::Chrono::Duration &)> in_callback) noexcept = 0;
            virtual inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept = 0;
            virtual OmegaStatus start() noexcept = 0;
            virtual OmegaStatus start_immediate() noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;

            virtual inline const char *get_name() const noexcept = 0;
            virtual inline Type get_type() const noexcept = 0;
            virtual inline Duration get_duration() const noexcept = 0;
            virtual inline Duration get_update_period() const noexcept = 0;
            virtual inline Duration get_delay() const noexcept = 0;
            virtual inline std::function<void(void)> get_start() const noexcept = 0;
            virtual inline std::function<void(const ::Omega::Chrono::Duration &)> get_update() const noexcept = 0;
            virtual inline std::function<void(void)> get_end() const noexcept = 0;
        };

    } // namespace Chrono
} // namespace Omega