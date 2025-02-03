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
 * Last Modified: Tuesday, 4th February 2025 1:21:07 am
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
#include "FreeRTOS/timers.h"

#include "OmegaChronoController/ChronoBaseController.hpp"
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
                while (xTimerIsTimerActive(handle))
                {
                }
                xTimerDelete(handle, portMAX_DELAY);
            }
            handle = nullptr;
        }

        void FreeRTOS::start_immediate() noexcept
        {
            start_immediate(delay, update_period, duration, on_start, on_update, on_end);
        }

        void FreeRTOS::start_immediate(const Duration &delay, const Duration &update_period, const Duration &duration, std::function<void(void)> start_handler, std::function<void(const ::Omega::Chrono::Duration &)> update_handler, std::function<void(void)> end_handler) noexcept
        {
            this->delay = delay;
            this->update_period = update_period;
            this->duration = duration;
            this->on_start = start_handler;
            this->on_update = update_handler;
            this->on_end = end_handler;
            const auto immediate_task = [](void *arg)
            {
                auto controller = (FreeRTOS *)arg;
                do
                {
                    const auto on_period_elapsed_handler = [](TimerHandle_t handle)
                    {
                        auto controller = (FreeRTOS *)pvTimerGetTimerID(handle);
                        Duration remaining_time = ZERO;
                        const auto duration = controller->duration;
                        const auto elapsed_duration = controller->update_period;
                        if (duration > elapsed_duration)
                        {
                            remaining_time = duration - elapsed_duration;
                            controller->duration = remaining_time;
                        }
                        const auto on_update = controller->on_update;
                        if (on_update)
                            on_update(remaining_time);
                        if (ZERO >= remaining_time)
                        {
                            if (const auto err = xTimerStop(controller->handle, 0); pdPASS != err)
                            {
                                LOGE("xTimerStop failed");
                                return;
                            }
                            const auto on_end = controller->on_end;
                            if (on_end)
                                on_end();
                            return;
                        }
                    };
                    if (0 >= get_ticks(controller->update_period))
                    {
                        LOGE("Invalid duration: %d ms | %ld ticks", controller->update_period.ms, pdMS_TO_TICKS(controller->update_period.ms));
                        break;
                    }
                    const auto handle = xTimerCreate("timer", pdMS_TO_TICKS(controller->update_period.get_in_msecs()), pdTRUE, controller, on_period_elapsed_handler);
                    if (nullptr == handle)
                    {
                        LOGE("xTimerCreate failed");
                        break;
                    }
                    controller->set_handle(handle);
                    const auto on_delay_expired_handler = [](TimerHandle_t handle)
                    {
                        const auto delay_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                        xSemaphoreGive(*delay_expire_semaphore);
                    };
                    const auto delay = controller->delay;
                    if (0 < get_ticks(delay))
                    {
                        StaticSemaphore_t semaphore_buffer;
                        auto delay_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
                        if (!delay_expire_semaphore)
                        {
                            LOGE("Semaphore Creation failed");
                            break;
                        }
                        LOGD("delay in ms: %d", delay.get_in_msecs());
                        const auto delay_expire_handle = xTimerCreate("timer1", pdMS_TO_TICKS(delay.get_in_msecs()), pdFALSE, &delay_expire_semaphore, on_delay_expired_handler);
                        if (!delay_expire_handle)
                        {
                            LOGE("Delay timer creation failed");
                            break;
                        }
                        if (const auto err = xTimerStart(delay_expire_handle, portMAX_DELAY); pdPASS != err)
                        {
                            LOGE("Delay timer start failed");
                            break;
                        }
                        xSemaphoreTake(delay_expire_semaphore, portMAX_DELAY);
                    }
                    const auto timer_handle = controller->handle;
                    if (nullptr == timer_handle)
                        break;
                    if (const auto err = xTimerStart(controller->handle, portMAX_DELAY); pdPASS != err)
                    {
                        LOGE("xTimerStart failed");
                        break;
                    }
                    LOGD("[Started]");
                    const auto on_start = controller->on_start;
                    if (on_start)
                        on_start();
                } while (0);
                vTaskDelete(nullptr);
            };
            if (const auto err = xTaskCreate(immediate_task, "immediate_task", configMINIMAL_STACK_SIZE * 2, this, configMAX_PRIORITIES - 2, nullptr); pdPASS != err)
                return;
            return;
        }

        OmegaStatus FreeRTOS::start() noexcept
        {
            return start(delay, update_period, duration, on_start, on_update, on_end);
        }

        OmegaStatus FreeRTOS::start(const Duration &delay, const Duration &update_period, const Duration &duration, std::function<void(void)> start_handler, std::function<void(const ::Omega::Chrono::Duration &)> update_handler, std::function<void(void)> end_handler) noexcept
        {
            this->delay = delay;
            this->update_period = update_period;
            this->duration = duration;
            this->on_start = start_handler;
            this->on_update = update_handler;
            this->on_end = end_handler;
            const auto on_delay_expired_handler = [](TimerHandle_t handle)
            {
                const auto delay_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                xSemaphoreGive(*delay_expire_semaphore);
            };
            const auto on_period_elapsed_handler = [](TimerHandle_t handle)
            {
                auto controller = (FreeRTOS *)pvTimerGetTimerID(handle);
                Duration remaining_time = ZERO;
                const auto duration = controller->duration;
                const auto elapsed_duration = controller->update_period;
                if (duration > elapsed_duration)
                {
                    remaining_time = duration - elapsed_duration;
                    controller->duration = remaining_time;
                }
                const auto on_update = controller->on_update;
                if (on_update)
                    on_update(remaining_time);
                if (ZERO >= remaining_time)
                {
                    if (const auto err = xTimerStop(controller->handle, 0); pdPASS != err)
                    {
                        LOGE("xTimerStop failed");
                        return;
                    }
                    const auto on_end = controller->on_end;
                    if (on_end)
                        on_end();
                    return;
                }
            };
            if (0 >= get_ticks(update_period))
            {
                LOGE("Invalid duration: %d ms | %ld ticks", update_period.ms, get_ticks(update_period));
                return eFAILED;
            }
            handle = xTimerCreate("timer", pdMS_TO_TICKS(update_period.get_in_msecs()), pdTRUE, this, on_period_elapsed_handler);
            if (nullptr == handle)
            {
                LOGE("xTimerCreate failed");
                return eFAILED;
            }
            if (0 < get_ticks(delay))
            {
                StaticSemaphore_t semaphore_buffer;
                auto delay_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
                if (!delay_expire_semaphore)
                {
                    LOGE("Semaphore Creation failed");
                    return eFAILED;
                }
                LOGD("[Delay Started] | %d", delay.get_in_msecs());
                const auto delay_expire_handle = xTimerCreate("timer1", pdMS_TO_TICKS(delay.get_in_msecs()), pdFALSE, &delay_expire_semaphore, on_delay_expired_handler);
                if (!delay_expire_handle)
                {
                    LOGE("Delay timer creation failed");
                    return eFAILED;
                }
                if (const auto err = xTimerStart(delay_expire_handle, portMAX_DELAY); pdPASS != err)
                {
                    LOGE("Delay timer start failed");
                    return eFAILED;
                }
                xSemaphoreTake(delay_expire_semaphore, portMAX_DELAY);
                LOGD("[Delay Ended]");
            }
            if (const auto err = xTimerStart(handle, portMAX_DELAY); pdPASS != err)
            {
                LOGE("xTimerStart failed");
                return eFAILED;
            }
            LOGD("[Started]");
            if (on_start)
                on_start();
            return eSUCCESS;
        }

        OmegaStatus FreeRTOS::pause() noexcept
        {
            LOGE("Unimplemented");
            assert(0);
            return eFAILED;
        }

        OmegaStatus FreeRTOS::resume() noexcept
        {
            LOGE("Unimplemented");
            assert(0);
            return eFAILED;
        }

        OmegaStatus FreeRTOS::stop() noexcept
        {
            LOGE("Unimplemented");
            assert(0);
            return eFAILED;
        }
    } // namespace Chrono
} // namespace Omega
