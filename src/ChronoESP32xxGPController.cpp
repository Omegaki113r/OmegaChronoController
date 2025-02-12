/**
 * @file ChronoESP32xxGPController.cpp
 * @author Omegaki113r
 * @date Wednesday, 12th February 2025 12:07:35 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoESP32xxGPController.cpp
 * File Created: Wednesday, 12th February 2025 12:07:35 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 12th February 2025 11:48:47 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/semphr.h>
#include <FreeRTOS/task.h>

#include "OmegaChronoController/AL/ChronoESP32xxGPController.hpp"

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
        OmegaStatus ESP32xxGP::start(Duration update_period, Duration duration) noexcept
        {
            m_update_period = update_period;
            m_duration = update_period;
            struct Data
            {
                const char *name;
                const SemaphoreHandle_t semaphore;
                bool &should_end;
                const on_update_handler on_update;
            };

            const auto on_duration_expired_handler = [](gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
            {
                BaseType_t task_awoken = pdFALSE;
                const auto data = (Data *)user_data;
                if (data->semaphore)
                    xSemaphoreGiveFromISR(data->semaphore, &task_awoken);
                return task_awoken == pdTRUE;
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
            if (update_period != duration)
                looping = pdTRUE;
            Data data{"", duration_expire_semaphore, should_end, get_on_update()};

            const gptimer_config_t config{GPTIMER_CLK_SRC_DEFAULT, GPTIMER_COUNT_UP, 1 * 1000 * 1000};
            if (const auto err = gptimer_new_timer(&config, &m_handle); ESP_OK != err)
            {
                LOGE("gptimer_new_timer failed. %s", esp_err_to_name(err));
                return eFAILED;
            }
            const gptimer_event_callbacks_t callback{on_duration_expired_handler};
            if (const auto err = gptimer_register_event_callbacks(m_handle, &callback, &data); ESP_OK != err)
            {
                LOGE("gptimer_register_event_callbacks failed. %s", esp_err_to_name(err));
                return eFAILED;
            }
            if (const auto err = gptimer_enable(m_handle); ESP_OK != err)
            {
                LOGE("gptimer_enable failed. %s", esp_err_to_name(err));
                return eFAILED;
            }

            if (looping)
            {
                m_type = Type::ePERIODIC;
                u64 frequency = 1 * 1000 * 1000;
                frequency = frequency * 1000 * 1000 * Duration::to_secs(update_period);
                frequency = frequency / (1000 * 1000);
                const gptimer_alarm_config_t alarm_config{frequency, 0, {true}};
                LOGD("Alarm Count: %lld | Reload Count: %lld", alarm_config.alarm_count, alarm_config.reload_count);
                if (const auto err = gptimer_set_alarm_action(m_handle, &alarm_config); ESP_OK != err)
                {
                    LOGE("gptimer_set_alarm_action failed. %s", esp_err_to_name(err));
                    return eFAILED;
                }
            }
            else
            {
                m_type = Type::eSINGLE;
                u64 frequency = 1 * 1000 * 1000;
                frequency = frequency * 1000 * 1000 * Duration::to_usecs(update_period);
                frequency = frequency / (1000 * 1000);
                const gptimer_alarm_config_t alarm_config{frequency, 0, {false}};
                LOGD("Alarm Count: %lld | Reload Count: %lld", alarm_config.alarm_count, alarm_config.reload_count);
                if (const auto err = gptimer_set_alarm_action(m_handle, &alarm_config); ESP_OK != err)
                {
                    LOGE("gptimer_set_alarm_action failed. %s", esp_err_to_name(err));
                    return eFAILED;
                }
            }
            if (const auto err = gptimer_start(m_handle); ESP_OK != err)
            {
                LOGE("gptimer_start failed");
                return eFAILED;
            }
            const auto on_start = get_on_start();
            if (on_start)
                on_start("");
            do
            {
                xSemaphoreTake(duration_expire_semaphore, portMAX_DELAY);
                const auto on_update = get_on_update();
                if (on_update)
                    should_end = on_update("", {0});
            } while (looping && !should_end);

            if (const auto err = gptimer_stop(m_handle); ESP_OK != err)
            {
                LOGE("gptimer_stop failed. %s", esp_err_to_name(err));
                return eFAILED;
            }
            if (const auto err = gptimer_disable(m_handle); ESP_OK != err)
            {
                LOGE("gptimer_disable failed. %s", esp_err_to_name(err));
                return eFAILED;
            }
            if (const auto err = gptimer_del_timer(m_handle); ESP_OK != err)
            {
                LOGE("gptimer_del_timer failed. %s", esp_err_to_name(err));
                return eFAILED;
            }
            m_handle = nullptr;
            const auto on_stop = get_on_stop();
            if (on_stop)
                on_stop("");
            return eSUCCESS;
        }

        OmegaStatus ESP32xxGP::resume() noexcept
        {
            if (m_handle)
            {
                if (const auto err = gptimer_start(m_handle); ESP_OK != err)
                {
                    LOGE("gptimer_start failed. %s", esp_err_to_name(err));
                    return eFAILED;
                }
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus ESP32xxGP::pause() noexcept
        {

            if (m_handle)
            {
                if (const auto err = gptimer_stop(m_handle); ESP_OK != err)
                {
                    LOGE("gptimer_stop failed. %s", esp_err_to_name(err));
                    return eFAILED;
                }
                return eSUCCESS;
            }
            return eFAILED;
        }

        OmegaStatus ESP32xxGP::stop() noexcept
        {
            if (m_handle)
            {
                if (const auto err = gptimer_stop(m_handle); ESP_OK != err)
                {
                    LOGE("gptimer_stop failed. %s", esp_err_to_name(err));
                    return eFAILED;
                }
                return eSUCCESS;
            }
            return eFAILED;
        }
    }
}