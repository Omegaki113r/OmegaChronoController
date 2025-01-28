/**
 * @file ChronoController.cpp
 * @author Omegaki113r
 * @date Monday, 27th January 2025 8:22:29 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoController.cpp
 * File Created: Monday, 27th January 2025 8:22:29 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 29th January 2025 3:09:03 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include "OmegaChronoController/ChronoController.hpp"
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

#ifdef __cplusplus
namespace Omega
{
    namespace Chrono
    {
        FreeRTOS::~FreeRTOS()
        {
            if (handle)
            {
                xTimerDelete(handle, portMAX_DELAY);
            }
        }

        OmegaStatus FreeRTOS::start() noexcept
        {
            const auto on_period_elapsed_handler = [](TimerHandle_t handle)
            {
                auto controller = (FreeRTOS *)pvTimerGetTimerID(handle);
                const auto on_update = controller->get_update();
                LOGD("[%s] => [Elapsed]", controller->get_name());
                if (on_update)
                    on_update();
            };
            const auto on_delay_expired_handler = [](TimerHandle_t handle)
            {
                const auto delay_expire_semaphore = (SemaphoreHandle_t *)pvTimerGetTimerID(handle);
                xSemaphoreGive(*delay_expire_semaphore);
            };
            const auto zero = Duration(0);
            if (0 >= pdMS_TO_TICKS(duration.ms))
            {
                LOGE("Invalid duration: %ld ms | %ld ticks", duration.ms, pdMS_TO_TICKS(duration.ms));
                return eFAILED;
            }
            handle = xTimerCreate("timer", pdMS_TO_TICKS(duration.ms), pdTRUE, this, on_period_elapsed_handler);
            if (nullptr == handle)
            {
                LOGE("xTimerCreate failed");
                return eFAILED;
            }
            if (zero > pdMS_TO_TICKS(delay.ms))
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

        OmegaStatus FreeRTOS::pause() noexcept { return eSUCCESS; }
        OmegaStatus FreeRTOS::resume() noexcept { return eSUCCESS; }
        OmegaStatus FreeRTOS::stop() noexcept { return eSUCCESS; }
    } // namespace Chrono
} // namespace Omega
#else

#endif