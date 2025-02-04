/**
 * @file Duration.hpp
 * @author Omegaki113r
 * @date Monday, 3rd February 2025 6:40:37 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: Duration.hpp
 * File Created: Monday, 3rd February 2025 6:40:37 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 9:53:33 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        struct Duration
        {
            u16 h;
            u8 m;
            u8 s;
            u16 ms;
            u64 us;

            constexpr Duration() : h(0), m(0), s(0), ms(0), us(0) {}
            constexpr Duration(u16 in_h) : h(in_h), m(0), s(0), ms(0), us(0) {}
            constexpr Duration(u16 in_h, u16 in_m) : h(in_h), m(in_m), s(0), ms(0), us(0) {}
            constexpr Duration(u16 in_h, u16 in_m, u32 in_s) : h(in_h), m(in_m), s(in_s), ms(0), us(0) {}
            constexpr Duration(u16 in_h, u16 in_m, u32 in_s, u32 in_ms) : h(in_h), m(in_m), s(in_s), ms(in_ms), us(0) {}
            constexpr Duration(u16 in_h, u16 in_m, u32 in_s, u32 in_ms, u64 in_us) : h(in_h), m(in_m), s(in_s), ms(in_ms), us(in_us) {}

            constexpr static u64 to_usecs(const Duration &duration) noexcept
            {
                return duration.us + duration.ms * 1000 + duration.s * 1000000 + duration.m * 60000000 + duration.h * 3600000000;
            }

            constexpr bool operator==(const Duration &other) const { return h == other.h && m == other.m && s == other.s && ms == other.ms && us == other.us; }

            constexpr bool operator!=(const Duration &other) const { return h != other.h || m != other.m || s != other.s || ms != other.ms || us != other.us; }

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

            constexpr Duration operator+(const Duration &other) const noexcept
            {
                u64 usecs = this->us + other.us;
                u16 msecs = this->ms - other.ms;
                u8 secs = this->s - other.s;
                u8 mins = this->m - other.m;
                u16 hrs = this->h - other.h;
                if (usecs >= 1000)
                {
                    usecs = usecs % 1000;
                    u64 i = usecs / 1000;
                    msecs += i;
                }
                if (msecs >= 1000)
                {
                    msecs = msecs % 1000;
                    u64 i = msecs / 1000;
                    secs += i;
                }
                if (secs >= 60)
                {
                    secs = secs % 60;
                    u64 i = secs / 60;
                    mins += i;
                }
                if (mins >= 60)
                {
                    mins = mins % 60;
                    u64 i = mins / 60;
                    hrs += i;
                }
                return {hrs, mins, secs, msecs, usecs};
            };

            constexpr Duration operator-(const Duration &other) const noexcept
            {
                // Convert both durations to microseconds
                u64 total_us = us + ms * 1000 + s * 1000000 + m * 60000000 + h * 3600000000;
                u64 other_total_us = other.us + other.ms * 1000 + other.s * 1000000 + other.m * 60000000 + other.h * 3600000000;

                // If the right-hand side duration is larger, return a zero duration
                if (other_total_us > total_us)
                {
                    return Duration(0, 0, 0, 0, 0);
                }

                // Perform the subtraction
                u64 result_us = total_us - other_total_us;

                // Reconstruct the Duration from the result in microseconds
                u64 result_h = static_cast<u64>(result_us / 3600000000);
                result_us %= 3600000000;

                u64 result_m = static_cast<u64>(result_us / 60000000);
                result_us %= 60000000;

                u64 result_s = static_cast<u64>(result_us / 1000000);
                result_us %= 1000000;

                u64 result_ms = static_cast<u64>(result_us / 1000);
                result_us %= 1000;

                u64 result_us_remaining = result_us; // Remaining microseconds

                return Duration(result_h, result_m, result_s, result_ms, result_us_remaining);
            }

            constexpr Duration operator=(u64 in_time_us) noexcept
            {
                return {0, 0, 0, 0, in_time_us};
            }

            constexpr u64 get_in_hours() const noexcept
            {
                return h + (m / 60) + (s / (60 * 60)) + (ms / (60 * 60 * 1000)) + (us / (60 * 60 * 1000 * 1000));
            }
            constexpr u64 get_in_msecs() const noexcept
            {
                return (h * 60 * 60 * 1000) + (m * 60 * 1000) + (s * 1000) + ms + (us / 1000);
            }

            constexpr Duration hours(const Duration in_duration)
            {
                h = in_duration.h;
                return *this;
            }
            constexpr Duration hours(const u64 in_duration)
            {
                h = in_duration;
                return *this;
            }

            constexpr Duration minutes(const Duration in_duration)
            {
                m = in_duration.m;
                return *this;
            }
            constexpr Duration minutes(const u64 in_duration)
            {
                m = in_duration;
                return *this;
            }

            constexpr Duration seconds(const Duration in_duration)
            {
                s = in_duration.s;
                return *this;
            }
            constexpr Duration seconds(const u64 in_duration)
            {
                s = in_duration;
                return *this;
            }

            constexpr Duration milliseconds(const Duration in_duration)
            {
                ms = in_duration.ms;
                return *this;
            }
            constexpr Duration milliseconds(const u64 in_duration)
            {
                ms = in_duration;
                return *this;
            }

            constexpr Duration microseconds(const Duration in_duration)
            {
                us = in_duration.us;
                return *this;
            }
            constexpr Duration microseconds(const u64 in_duration)
            {
                us = in_duration;
                return *this;
            }
        };
    } // namespace Chrono
} // namespace Omega
