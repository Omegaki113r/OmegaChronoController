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
 * Last Modified: Tuesday, 4th February 2025 9:02:10 pm
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

#include "OmegaChronoController/ChronoBase.hpp"
#include "OmegaChronoController/ChronoFreeRTOSController.hpp"
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
        constexpr inline TickType_t get_ticks(const Duration &in_duration) { return pdMS_TO_TICKS(in_duration.get_in_msecs()); }

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

        OmegaStatus FreeRTOS::start(Duration update_period, Duration in_duration) noexcept
        {
            if (0 >= get_ticks(in_duration))
            {
                LOGE("Invalid duration: %d ms | %ld ticks", in_duration.ms, get_ticks(in_duration));
                return eFAILED;
            }
            const auto on_duration_expired_handler = [](TimerHandle_t handle)
            {
                const auto duration_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                xSemaphoreGive(*duration_expire_semaphore);
            };
            StaticSemaphore_t semaphore_buffer;
            auto duration_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
            if (!duration_expire_semaphore)
            {
                LOGE("Semaphore Creation failed");
                return eFAILED;
            }
            LOGD("duration in ms: %lld", in_duration.get_in_msecs());

            const auto duration_expire_handle = xTimerCreate("timer1", pdMS_TO_TICKS(update_period.get_in_msecs()), update_period == in_duration ? pdFALSE : pdTRUE, &duration_expire_semaphore, on_duration_expired_handler);
            if (!duration_expire_handle)
            {
                LOGE("duration timer creation failed");
                return eFAILED;
            }
            if (const auto err = xTimerStart(duration_expire_handle, portMAX_DELAY); pdPASS != err)
            {
                LOGE("duration timer start failed");
                return eFAILED;
            }
            xSemaphoreTake(duration_expire_semaphore, portMAX_DELAY);
            return eSUCCESS;
        }
    } // namespace Chrono
} // namespace Omega
