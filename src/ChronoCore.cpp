/**
 * @file ChronoCore.cpp
 * @author Omegaki113r
 * @date Monday, 3rd February 2025 7:51:39 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoCore.cpp
 * File Created: Monday, 3rd February 2025 7:51:39 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Monday, 3rd February 2025 10:11:16 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include "OmegaChronoController/ChronoCore.hpp"
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
        // void Core::start_immediate() noexcept
        // {
        // }

        // OmegaStatus Core::start() noexcept
        // {
        //     return eFAILED;
        // }

        // OmegaStatus Core::pause() noexcept
        // {
        //     LOGE("Unimplemented");
        //     assert(0);
        //     return eFAILED;
        // }

        // OmegaStatus Core::resume() noexcept
        // {
        //     LOGE("Unimplemented");
        //     assert(0);
        //     return eFAILED;
        // }

        // OmegaStatus Core::stop() noexcept
        // {
        //     LOGE("Unimplemented");
        //     assert(0);
        //     return eFAILED;
        // }
    } // namespace Chrono
} // namespace Omega
#else
#endif