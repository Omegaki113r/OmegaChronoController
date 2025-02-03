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
 * Last Modified: Tuesday, 4th February 2025 1:03:15 am
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
        protected:
        public:
            char name[50 + 1];
            Duration duration;
            Duration update_period;
            Duration delay;
            std::function<void(void)> on_start;
            std::function<void(const ::Omega::Chrono::Duration &)> on_update;
            std::function<void(void)> on_end;
            State state{State::eIDLE};

            constexpr inline const char *get_name() const noexcept { return name; }
            constexpr inline void set_duration(Duration in_duration) noexcept { duration = in_duration; }
            constexpr inline void set_update_period(Duration in_update_period) noexcept { update_period = in_update_period; }
            constexpr inline void set_delay(Duration in_delay) noexcept { delay = in_delay; }
            constexpr inline void add_on_start_callback(std::function<void(void)> in_callback) { on_start = in_callback; }
            constexpr inline void add_on_update_callback(std::function<void(const ::Omega::Chrono::Duration &)> in_callback) noexcept { on_update = in_callback; }
            constexpr inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept { on_end = in_callback; }
            virtual OmegaStatus start() noexcept = 0;
            virtual OmegaStatus start(const Duration &delay, const Duration &update_period, const Duration &duration,
                                      std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept = 0;
            virtual void start_immediate() noexcept = 0;
            virtual void start_immediate(const Duration &delay, const Duration &update_period, const Duration &duration,
                                         std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;

            void set_name(const char *in_name)
            {
                if (nullptr == in_name || 0 == std::strlen(in_name))
                    return;
                UNUSED(std::memcpy(name, in_name, OMEGA_MIN(std::strlen(in_name), sizeof(name))));
            }
        };

    } // namespace Chrono
} // namespace Omega