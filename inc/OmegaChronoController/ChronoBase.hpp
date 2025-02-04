/**
 * @file ChronoBaseController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:14:46 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoBaseController.hpp
 * File Created: Wednesday, 29th January 2025 4:14:46 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 5:33:10 pm
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

#include "OmegaChronoController/Duration.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        enum class State
        {
            eIDLE,
            eCREATED,
            eSTARTED,
            ePAUSED,
            eSTOPPED,
            eDELETED,
        };

        class Base
        {
        private:
            std::function<void(void)> on_delay_start;
            std::function<void(void)> on_delay_end;
            std::function<void(void)> on_start;
            std::function<void(const Duration &)> on_update;
            std::function<void(void)> on_end;

        public:
            char name[50 + 1];
            Duration duration;
            Duration update_period;
            Duration delay;
            State state{State::eIDLE};

            void set_name(const char *in_name);
            constexpr inline void set_duration(Duration in_duration) noexcept { duration = in_duration; }
            constexpr inline void set_update_period(Duration in_update_period) noexcept { update_period = in_update_period; }
            constexpr inline void set_delay(Duration in_delay) noexcept { delay = in_delay; }
            constexpr inline void add_on_delay_start_callback(std::function<void(void)> in_callback) { on_delay_start = in_callback; }
            constexpr inline void add_on_delay_end_callback(std::function<void(void)> in_callback) { on_delay_end = in_callback; }
            constexpr inline void add_on_start_callback(std::function<void(void)> in_callback) { on_start = in_callback; }
            constexpr inline void add_on_update_callback(std::function<void(const Duration &)> in_callback) noexcept { on_update = in_callback; }
            constexpr inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept { on_end = in_callback; }

            constexpr inline const char *get_name() const noexcept { return name; }
            constexpr inline const std::function<void(void)> &get_on_delay_start_handler() const noexcept { return on_delay_start; }
            constexpr inline const std::function<void(void)> &get_on_delay_end_handler() const noexcept { return on_delay_end; }
            constexpr inline const std::function<void(void)> &get_on_start_handler() const noexcept { return on_start; }
            constexpr inline const std::function<void(const Duration &)> &get_on_update_handler() const noexcept { return on_update; }
            constexpr inline const std::function<void(void)> &get_on_end_handler() const noexcept { return on_end; }

            virtual OmegaStatus start() noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;
        };
    } // namespace Chrono
} // namespace Omega