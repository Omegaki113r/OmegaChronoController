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
 * Last Modified: Wednesday, 12th February 2025 5:40:03 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/Base/ChronoBase.hpp"
#include "OmegaChronoController/Base/ChronoCallbacks.hpp"
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
        template <typename T>
        class Singleshot : public Base
        {
            T core;

        public:
            constexpr Singleshot(T in_core) : core(in_core) {}

            ~Singleshot() {}

            constexpr Singleshot &name(const char *in_name) noexcept override
            {
                set_name(in_name);
                return *this;
            }

            constexpr Singleshot &delay(Duration in_delay) noexcept override
            {
                set_delay(in_delay);
                return *this;
            }

            constexpr Singleshot &update_period(Duration in_update_period) noexcept override
            {
                set_update_period(in_update_period);
                return *this;
            }

            constexpr Singleshot &duration(Duration in_duration) noexcept override
            {
                set_duration(in_duration);
                return *this;
            }

            OmegaStatus start() noexcept override
            {
                if (Duration{0} == m_duration)
                {
                    LOGE("Invalid Duration");
                    return eFAILED;
                }
                const auto timer_task = [](void *arg)
                {
                    Singleshot *controller = (Singleshot *)arg;
                    if (Duration{0} < controller->m_delay)
                    {
                        const auto on_start = [&](const char *name)
                        {
                            const auto on_start_handler = controller->get_on_delay_start_handler();
                            if (on_start_handler)
                                on_start_handler(controller->m_name);
                        };
                        const auto on_end = [&](const char *name)
                        {
                            const auto on_end_handler = controller->get_on_delay_stopped_handler();
                            if (on_end_handler)
                                on_end_handler(controller->m_name);
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
                        const auto on_end = [&](const char *name)
                        {
                            const auto on_end_handler = controller->get_on_stop_handler();
                            if (on_end_handler)
                                on_end_handler(controller->m_name);
                        };
                        controller->core.add_on_start_callback(on_start);
                        controller->core.add_on_stop_callback(on_end);
                        controller->core.start(controller->m_duration, controller->m_duration);
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