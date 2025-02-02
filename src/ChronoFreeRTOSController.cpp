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
 * Last Modified: Sunday, 2nd February 2025 8:50:02 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/ChronoFreeRTOSController.hpp"
#include "OmegaChronoController/ChronoBaseController.hpp"
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
            duration = 0;
            update_period = 0;
            delay = 0;
            on_start = nullptr;
            on_update = nullptr;
            on_end = nullptr;
            handle = nullptr;
        }

        OmegaStatus FreeRTOS::start_immediate() noexcept
        {
            const auto on_period_elapsed_handler = [](TimerHandle_t handle)
            {
                auto controller = (FreeRTOS *)pvTimerGetTimerID(handle);
                Duration remaining_time = ZERO;
                const auto duration = controller->get_duration();
                const auto elapsed_duration = controller->get_update_period();
                if (duration > elapsed_duration)
                {
                    remaining_time = duration - elapsed_duration;
                    controller->set_duration(remaining_time);
                }
                const auto on_update = controller->get_update();
                if (on_update)
                    on_update(remaining_time);
                if (ZERO >= remaining_time)
                {
                    if (const auto err = xTimerStop(controller->get_handle(), 0); pdPASS != err)
                    {
                        LOGE("xTimerStop failed");
                        return;
                    }
                    const auto on_end = controller->get_end();
                    if (on_end)
                        on_end();
                    return;
                }
            };
            if (0 >= pdMS_TO_TICKS(update_period.ms))
            {
                LOGE("Invalid duration: %ld ms | %ld ticks", update_period.ms, pdMS_TO_TICKS(update_period.ms));
                return eFAILED;
            }
            handle = xTimerCreate("timer", pdMS_TO_TICKS(update_period.ms), pdTRUE, this, on_period_elapsed_handler);
            if (nullptr == handle)
            {
                LOGE("xTimerCreate failed");
                return eFAILED;
            }
                    controller->set_handle(handle);
                    const auto on_delay_expired_handler = [](TimerHandle_t handle)
                    {
                        const auto delay_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                        xSemaphoreGive(*delay_expire_semaphore);
                    };
                    if (0 <= pdMS_TO_TICKS(controller->get_delay().ms))
                    {
                        StaticSemaphore_t semaphore_buffer;
                        auto delay_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
                        if (!delay_expire_semaphore)
                        {
                            LOGE("Semaphore Creation failed");
                            break;
                        }
                        const auto delay_expire_handle = xTimerCreate("timer1", pdMS_TO_TICKS(controller->get_delay().ms), pdFALSE, &delay_expire_semaphore, on_delay_expired_handler);
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
                    const auto timer_handle = controller->get_handle();
                    if (nullptr == timer_handle)
                        break;
                    if (const auto err = xTimerStart(controller->get_handle(), portMAX_DELAY); pdPASS != err)
                    {
                        LOGE("xTimerStart failed");
                        break;
                    }
                    LOGD("[%s] => [Started]", controller->get_name());
                    const auto on_start = controller->get_start();
                    if (on_start)
                        on_start();
                } while (0);
                vTaskDelete(nullptr);
            };
            if (const auto err = xTaskCreate(immediate_task, "immediate_task", configMINIMAL_STACK_SIZE * 2, this, configMAX_PRIORITIES - 2, nullptr); pdPASS != err)
                return eSUCCESS;
            return eFAILED;
        }

        OmegaStatus FreeRTOS::start() noexcept
        {
            const auto on_delay_expired_handler = [](TimerHandle_t handle)
            {
                const auto delay_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                xSemaphoreGive(*delay_expire_semaphore);
            };
            const auto on_period_elapsed_handler = [](TimerHandle_t handle)
            {
                auto controller = (FreeRTOS *)pvTimerGetTimerID(handle);
                Duration remaining_time = ZERO;
                const auto duration = controller->get_duration();
                const auto elapsed_duration = controller->get_update_period();
                if (duration > elapsed_duration)
                {
                    remaining_time = duration - elapsed_duration;
                    controller->set_duration(remaining_time);
                }
                const auto on_update = controller->get_update();
                if (on_update)
                    on_update(remaining_time);
                if (ZERO >= remaining_time)
                {
                    if (const auto err = xTimerStop(controller->get_handle(), 0); pdPASS != err)
                    {
                        LOGE("xTimerStop failed");
                        return;
                    }
                    const auto on_end = controller->get_end();
                    if (on_end)
                        on_end();
                    return;
                }
            };
            if (0 >= pdMS_TO_TICKS(update_period.ms))
            {
                LOGE("Invalid duration: %ld ms | %ld ticks", update_period.ms, pdMS_TO_TICKS(update_period.ms));
                return eFAILED;
            }
            handle = xTimerCreate("timer", pdMS_TO_TICKS(update_period.ms), pdTRUE, this, on_period_elapsed_handler);
            if (nullptr == handle)
            {
                LOGE("xTimerCreate failed");
                return eFAILED;
            }
            if (0 <= pdMS_TO_TICKS(delay.ms))
            {
                StaticSemaphore_t semaphore_buffer;
                auto delay_expire_semaphore = xSemaphoreCreateBinaryStatic(&semaphore_buffer);
                if (!delay_expire_semaphore)
                {
                    LOGE("Semaphore Creation failed");
                    return eFAILED;
                }
                const auto delay_expire_handle = xTimerCreate("timer1", pdMS_TO_TICKS(delay.ms), pdFALSE, &delay_expire_semaphore, on_delay_expired_handler);
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
            }
            if (const auto err = xTimerStart(handle, portMAX_DELAY); pdPASS != err)
            {
                LOGE("xTimerStart failed");
                return eFAILED;
            }
            LOGD("[%s] => [Started]", get_name());
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
