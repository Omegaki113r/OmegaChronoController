/**
 * @file ChronoBaseController.cpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:38:04 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoBaseController.cpp
 * File Created: Wednesday, 29th January 2025 4:38:04 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 5:32:45 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include "OmegaChronoController/ChronoBase.hpp"
#include "OmegaChronoController/Duration.hpp"
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
        void Base::set_name(const char *in_name)
        {
            if (nullptr == in_name || 0 == std::strlen(in_name))
                return;
            UNUSED(std::memcpy(name, in_name, OMEGA_MIN(std::strlen(in_name), sizeof(name))));
        }
    } // namespace Chrono
} // namespace Omega