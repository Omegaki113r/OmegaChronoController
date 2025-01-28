/**
 * @file ChronoController.hpp
 * @author Omegaki113r
 * @date Monday, 27th January 2025 8:21:55 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoController.hpp
 * File Created: Monday, 27th January 2025 8:21:55 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 29th January 2025 4:22:24 am
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
#include <functional>
#include <utility>
#include <variant>

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include "OmegaChronoController/ChronoBaseController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

#ifdef __cplusplus

namespace Omega
{
    namespace Chrono
    {

        class FreeRTOS : public Base
        {
        public:
            FreeRTOS(Type in_type = Type::eSINGLE_SHOT, Duration in_duration = {0}, Duration in_update_period = {0}, Duration in_delay = {0})
                : Base(in_type, in_duration, in_update_period, in_delay) {}
            ~FreeRTOS();

            inline void set_name(const char *in_name) override
            {
                if (nullptr == in_name || 0 == std::strlen(in_name))
                    return;
                UNUSED(std::memcpy(name, in_name, OMEGA_MIN(std::strlen(in_name), sizeof(name))));
            }
            inline void set_type(Type in_type) noexcept override { type = in_type; }
            inline void set_duration(Duration in_duration) noexcept override { duration = in_duration; }
            inline void set_update_period(Duration in_update_period) noexcept override { update_period = in_update_period; }
            inline void set_delay(Duration in_delay) noexcept override { delay = in_delay; }
            inline void add_on_start_callback(std::function<void(void)> in_callback) noexcept override { on_start = in_callback; }
            inline void add_on_update_callback(std::function<void(void)> in_callback) noexcept override { on_update = in_callback; }
            inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept override { on_end = in_callback; }
            OmegaStatus start() noexcept override;
            OmegaStatus pause() noexcept override;
            OmegaStatus resume() noexcept override;
            OmegaStatus stop() noexcept override;

            inline const char *get_name() const noexcept override { return name; }
            inline Type get_type() const noexcept override { return type; }
            inline Duration get_duration() const noexcept override { return duration; }
            inline Duration get_update_period() const noexcept override { return update_period; }
            inline Duration get_delay() const noexcept override { return delay; }
            inline std::function<void(void)> get_start() const noexcept { return on_start; }
            inline std::function<void(void)> get_update() const noexcept { return on_update; }
            inline std::function<void(void)> get_end() const noexcept { return on_end; }

            FreeRTOS(FreeRTOS &other) = delete;
            FreeRTOS &operator=(FreeRTOS &) = delete;
            FreeRTOS(FreeRTOS &&) = delete;
            FreeRTOS &operator=(FreeRTOS &&) = delete;

        private:
        };
    } // namespace Chrono
} // namespace Omega
#else
#endif