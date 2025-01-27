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
 * Last Modified: Tuesday, 28th January 2025 2:04:36 am
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
        Duration::Duration(u64 in_duration_us) : us(in_duration_us), ms(us / 1000), s(ms / 1000), m(s / 60), h(m / 60) { LOGD("%lld %ld %ld %d %d", us, ms, s, m, h); }

        FreeRTOS::~FreeRTOS()
        {
            // if (nullptr == handle)
            //     return;
            // xTimerDelete(handle, portMAX_DELAY);
            LOGD("Cleaned");
        }

        OmegaStatus FreeRTOS::start() noexcept
        {
            const auto x = [](TimerHandle_t handle)
            { LOGE("Critical error"); };
            if (Duration(0) == duration)
            {
                LOGE("Invalid duration: %lld", duration.us);
                return eFAILED;
            }
            handle = xTimerCreate("timer", pdMS_TO_TICKS(duration.ms), pdFALSE, nullptr, x);
            if (nullptr == handle)
            {
                LOGE("xTimerCreate failed");
                return eFAILED;
            }
            delay_ms(delay.ms);
            if (const auto err = xTimerStart(handle, portMAX_DELAY); pdPASS != err)
            {
                LOGE("xTimerStart failed");
                return eFAILED;
            }
            if (nullptr != on_start)
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