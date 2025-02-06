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
 * Last Modified: Friday, 7th February 2025 12:25:55 am
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

#include "OmegaChronoController/ChronoCallbacks.hpp"
#include "OmegaChronoController/Duration.hpp"
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
        enum class State
        {
            eIDLE,
            eCREATED,
            eSTARTED,
            ePAUSED,
            eSTOPPED,
            eDELETED,
        };

        class Base
        {

        protected:
            char m_name[50 + 1];
            Duration m_duration;
            Duration m_update_period;
            Duration m_delay;
            State m_state{State::eIDLE};
            on_delay_started_handler m_on_delay_started;
            on_delay_stopped_handler m_on_delay_stopped;
            on_started_handler m_on_started;
            on_resumed_handler m_on_resumed;
            on_update_handler m_on_update;
            on_paused_handler m_on_paused;
            on_stopped_handler m_on_stopped;

        public:
            void set_name(const char *in_name)
            {
                if (nullptr == in_name || 0 == std::strlen(in_name))
                    return;
                const auto name_length = std::strlen(in_name);
                UNUSED(std::memcpy(m_name, in_name, OMEGA_MIN(name_length, sizeof(m_name))));
                m_name[name_length] = '\0';
            }
            constexpr inline void set_delay(Duration in_delay) noexcept { m_delay = in_delay; }
            constexpr inline void set_update_period(Duration in_update_period) noexcept { m_update_period = in_update_period; }
            constexpr inline void set_duration(Duration in_duration) noexcept { m_duration = in_duration; }
            inline void add_on_delay_start_callback(on_delay_started_handler in_callback) { m_on_delay_started = in_callback; }
            inline void add_on_delay_stopped_callback(on_delay_stopped_handler in_callback) { m_on_delay_stopped = in_callback; }
            inline void add_on_start_callback(on_started_handler in_callback) { m_on_started = in_callback; }
            inline void add_on_resume_callback(on_resumed_handler in_callback) { m_on_resumed = in_callback; }
            inline void add_on_update_callback(on_update_handler in_callback) noexcept { m_on_update = in_callback; }
            inline void add_on_pause_callback(on_paused_handler in_callback) noexcept { m_on_paused = in_callback; }
            inline void add_on_stop_callback(on_stopped_handler in_callback) noexcept { m_on_stopped = in_callback; }

            constexpr inline const char *get_name() const noexcept { return m_name; }
            constexpr inline const on_delay_started_handler &get_on_delay_start_handler() const noexcept { return m_on_delay_started; }
            constexpr inline const on_delay_stopped_handler &get_on_delay_stopped_handler() const noexcept { return m_on_delay_stopped; }
            constexpr inline const on_started_handler &get_on_start_handler() const noexcept { return m_on_started; }
            constexpr inline const on_resumed_handler &get_on_resume_handler() const noexcept { return m_on_started; }
            constexpr inline const on_update_handler &get_on_update_handler() const noexcept { return m_on_update; }
            constexpr inline const on_paused_handler &get_on_pause_handler() const noexcept { return m_on_paused; }
            constexpr inline const on_stopped_handler &get_on_stop_handler() const noexcept { return m_on_stopped; }

            virtual OmegaStatus start() noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;

            virtual constexpr Base &name(const char *in_name) noexcept = 0;
            virtual constexpr Base &delay(Duration) noexcept = 0;
            virtual constexpr Base &update_duration(Duration) noexcept = 0;
            virtual constexpr Base &duration(Duration) noexcept = 0;
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