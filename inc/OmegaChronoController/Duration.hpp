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
 * Last Modified: Friday, 7th February 2025 12:48:22 am
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

#include <sdkconfig.h>
#if CONFIG_OMEGA_CHRONO_CONTROLLER_DEBUG
#define LOGD(format, ...) OMEGA_LOGD(format, ##__VA_ARGS__)
#define HEX_LOGD(buffer, length) OMEGA_HEX_LOGD(buffer, length)
#else
#define LOGD(format, ...)
#define HEX_LOGD(buffer, length)
#endif
#if CONFIG_OMEGA_CHRONO_CONTROLLER_INFO
#define LOGI(format, ...) OMEGA_LOGI(format, ##__VA_ARGS__)
#define HEX_LOGI(buffer, length) OMEGA_HEX_LOGI(buffer, length)
#else
#define LOGI(format, ...)
#define HEX_LOGI(buffer, length)
#endif

#if CONFIG_OMEGA_CHRONO_CONTROLLER_WARN
#define LOGW(format, ...) OMEGA_LOGW(format, ##__VA_ARGS__)
#define HEX_LOGW(buffer, length) OMEGA_HEX_LOGW(buffer, length)
#else
#define LOGW(format, ...)
#define HEX_LOGW(buffer, length)
#endif

#if CONFIG_OMEGA_CHRONO_CONTROLLER_ERROR
#define LOGE(format, ...) OMEGA_LOGE(format, ##__VA_ARGS__)
#define HEX_LOGE(buffer, length) OMEGA_HEX_LOGE(buffer, length)
#else
#define LOGE(format, ...)
#define HEX_LOGE(buffer, length)
#endif

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

            constexpr u16 us_to_ms(const auto in_us) { return in_us / 1000; }
            constexpr u8 ms_to_s(const auto in_ms) { return in_ms / 1000; }
            constexpr u8 s_to_mins(const auto in_s) { return in_s / 60; }
            constexpr u16 mins_to_hrs(const auto in_mins) { return in_mins / 60; }

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
                // Convert both durations to microseconds
                u64 total_us = us + ms * 1000 + s * 1000000 + m * 60000000 + h * 3600000000;
                u64 other_total_us = other.us + other.ms * 1000 + other.s * 1000000 + other.m * 60000000 + other.h * 3600000000;

                // Perform the subtraction
                u64 result_us = total_us + other_total_us;

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

#if CONFIG_OMEGA_CHRONO_CONTROLLER_DEBUG
#undef LOGD
#undef HEX_LOGD
#endif

#if CONFIG_OMEGA_CHRONO_CONTROLLER_INFO
#undef LOGI
#undef HEX_LOGI
#endif

#if CONFIG_OMEGA_CHRONO_CONTROLLER_WARN
#undef LOGW
#undef HEX_LOGW
#endif

#if CONFIG_OMEGA_CHRONO_CONTROLLER_ERROR
#undef LOGE
#undef HEX_LOGE
#endif