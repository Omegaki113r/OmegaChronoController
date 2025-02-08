/**
 * @file ChronoESP32xxHiResController.cpp
 * @author Omegaki113r
 * @date Saturday, 8th February 2025 5:22:19 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoESP32xxHiResController.cpp
 * File Created: Saturday, 8th February 2025 5:22:19 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Saturday, 8th February 2025 6:42:49 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/semphr.h>
#include <FreeRTOS/timers.h>

#include "OmegaChronoController/AL/ChronoESP32xxHiResController.hpp"

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
        OmegaStatus ESP32xxHiRes::start(Duration in_update_period, Duration in_duration) noexcept
        {
            update_period = in_update_period;
            duration = in_update_period;
            struct Data
            {
                const char *name;
                const SemaphoreHandle_t semaphore;
                bool &should_end;
                const on_update_handler on_update;
            };

            const auto on_duration_expired_handler = [](void *arg)
            {
                const auto data = (Data *)arg;
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
            if (in_update_period != in_duration)
                looping = pdTRUE;
            Data data{"", duration_expire_semaphore, should_end, get_on_update()};

            const esp_timer_create_args_t arg{on_duration_expired_handler, &data, ESP_TIMER_TASK, "timer"};
            if (const auto err = esp_timer_create(&arg, &handle); ESP_OK != err)
            {
                LOGE("esp_timer_create failed");
                return eSUCCESS;
            }
            if (looping)
            {
                type = Type::ePERIODIC;
                if (const auto err = esp_timer_start_periodic(handle, in_update_period.to_us()); ESP_OK != err)
                {
                    LOGE("esp_timer_start_periodic failed");
                    return eFAILED;
                }
            }
            else
            {
                type = Type::eSINGLE;
                if (const auto err = esp_timer_start_once(handle, in_update_period.to_us()); ESP_OK != err)
                {
                    LOGE("esp_timer_start_once failed");
                    return eFAILED;
                }
            }
            const auto on_start = get_on_start();
            if (on_start)
                on_start("");
            do
            {
                xSemaphoreTake(duration_expire_semaphore, portMAX_DELAY);
            } while (looping && !should_end);
            if (const auto err = esp_timer_stop(handle); err != ESP_OK)
            {
                LOGE("Timer stop failed");
                return eFAILED;
            }
            if (const auto err = esp_timer_delete(handle); ESP_OK != err)
            {
                LOGE("esp_timer_delete failed");
                return eFAILED;
            }
            handle = nullptr;
            const auto on_stop = get_on_stop();
            if (on_stop)
                on_stop("");
            return eSUCCESS;
        }

        OmegaStatus ESP32xxHiRes::resume() noexcept
        {
            if (handle)
            {
                const auto timeout = Type::ePERIODIC == type ? update_period.to_us() : 0;
                if (!esp_timer_is_active(handle))
                {
                    if (Type::ePERIODIC == type)
                    {
                        if (const auto err = esp_timer_start_periodic(handle, update_period.to_us()); ESP_OK != err)
                        {
                            LOGE("esp_timer_start_periodic failed with timeout: %lld and err: %s", timeout, esp_err_to_name(err));
                            return eFAILED;
                        }
                    }
                    else
                    {
                        if (const auto err = esp_timer_start_once(handle, update_period.to_us()); ESP_OK != err)
                        {
                            LOGE("esp_timer_start_once failed with timeout: %lld and err: %s", timeout, esp_err_to_name(err));
                            return eFAILED;
                        }
                    }
                }
                if (m_on_resumed)
                    m_on_resumed("");
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus ESP32xxHiRes::pause() noexcept
        {
            if (handle)
            {
                if (const auto err = esp_timer_stop(handle); ESP_OK != err)
                {
                    LOGE("esp_timer_stop failed");
                    return eFAILED;
                }
                do
                {
                } while (esp_timer_is_active(handle));
                if (m_on_paused)
                    m_on_paused("");
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus ESP32xxHiRes::stop() noexcept
        {
            if (handle)
            {
                if (const auto err = esp_timer_stop(handle); ESP_OK != err)
                {
                    LOGE("esp_timer_stop failed");
                    return eFAILED;
                }
                do
                {
                } while (esp_timer_is_active(handle));
                if (const auto err = esp_timer_delete(handle); ESP_OK != err)
                {
                    LOGE("esp_timer_delete failed");
                    return eFAILED;
                }
            }
            if (m_on_stopped)
                m_on_stopped("");
            handle = nullptr;
            return eSUCCESS;
        }
    } // namespace Chrono
} // namespace Omega
