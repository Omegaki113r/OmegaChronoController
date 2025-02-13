/**
 * @file ChronoFreeRTOSController.cpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:51:41 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoFreeRTOSController.cpp
 * File Created: Wednesday, 29th January 2025 4:51:41 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 13th February 2025 3:43:55 pm
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

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/timers.h"

#include "OmegaChronoController/Abstraction/FreeRTOSController.hpp"
#include "OmegaChronoController/Core/CoreBase.hpp"
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
        constexpr inline TickType_t get_ticks(const Duration &in_duration) { return pdMS_TO_TICKS(static_cast<u64>(Duration::to_msecs(in_duration))); }

        FreeRTOS::~FreeRTOS()
        {
            if (handle)
            {
                xTimerStop(handle, portMAX_DELAY);
                do
                {
                } while (xTimerIsTimerActive(handle));
                xTimerDelete(handle, portMAX_DELAY);
            }
            handle = nullptr;
        }

        OmegaStatus FreeRTOS::start(Duration in_update_period, Duration in_duration) noexcept
        {
            struct Data
            {
                const char *name;
                const SemaphoreHandle_t semaphore;
                bool &should_end;
                const on_update_handler on_update;
            };
            if (0 >= get_ticks(in_duration))
            {
                LOGE("Invalid duration: %d ms | %ld ticks", in_duration.ms, get_ticks(in_duration));
                return eFAILED;
            }
            if (0 >= get_ticks(in_update_period))
            {
                LOGE("Invalid duration: %d ms | %ld ticks", in_update_period.ms, get_ticks(in_update_period));
                return eFAILED;
            }

            const auto on_duration_expired_handler = [](TimerHandle_t handle)
            {
                const auto data = (Data *)pvTimerGetTimerID(handle);
                if (data->on_update)
                    data->should_end = data->on_update("", {0});
                if (data->semaphore)
                    xSemaphoreGive(data->semaphore);
            };
            StaticSemaphore_t semaphore_buffer;
            auto duration_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
            if (!duration_expire_semaphore)
            {
                LOGE("Semaphore Creation failed");
                return eFAILED;
            }
            auto should_end = false;
            auto looping = pdFALSE;
            const auto update_ticks = pdMS_TO_TICKS(static_cast<u64>(Duration::to_msecs(in_update_period)));
            if (in_update_period != in_duration)
                looping = pdTRUE;
            Data data{"", duration_expire_semaphore, should_end, get_on_update()};
            handle = xTimerCreate("timer1", update_ticks, looping, &data, on_duration_expired_handler);
            if (!handle)
            {
                LOGE("duration timer creation failed");
                return eFAILED;
            }
            if (const auto err = xTimerStart(handle, portMAX_DELAY); pdPASS != err)
            {
                LOGE("duration timer start failed");
                return eFAILED;
            }
            const auto on_start = get_on_start();
            if (on_start)
                on_start("");
            do
            {
                xSemaphoreTake(duration_expire_semaphore, portMAX_DELAY);
            } while (looping && !should_end);
            if (const auto err = xTimerStop(handle, portMAX_DELAY); err != pdPASS)
            {
                LOGE("Timer stop failed");
                return eFAILED;
            }
            xTimerDelete(handle, portMAX_DELAY);
            handle = nullptr;
            const auto on_stop = get_on_stop();
            if (on_stop)
                on_stop("");
            return eSUCCESS;
        }

        OmegaStatus FreeRTOS::resume() noexcept
        {
            if (handle)
            {
                if (pdFALSE == xTimerIsTimerActive(handle))
                    xTimerStart(handle, portMAX_DELAY);
                if (m_on_resumed)
                    m_on_resumed("");
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus FreeRTOS::pause() noexcept
        {
            if (handle)
            {
                xTimerStop(handle, portMAX_DELAY);
                do
                {
                } while (xTimerIsTimerActive(handle));
                if (m_on_paused)
                    m_on_paused("");
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus FreeRTOS::stop() noexcept
        {
            if (handle)
            {
                xTimerStop(handle, portMAX_DELAY);
                do
                {
                } while (xTimerIsTimerActive(handle));
                xTimerDelete(handle, portMAX_DELAY);
            }
            if (m_on_stopped)
                m_on_stopped("");
            handle = nullptr;
            return eSUCCESS;
        }
    } // namespace Chrono
} // namespace Omega
