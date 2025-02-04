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
 * Last Modified: Tuesday, 4th February 2025 8:36:14 pm
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
        protected:
            char m_name[50 + 1];
            Duration m_duration;
            Duration m_update_period;
            Duration m_delay;
            State m_state{State::eIDLE};
            std::function<void(void)> m_on_delay_start;
            std::function<void(void)> m_on_delay_end;
            std::function<void(void)> m_on_start;
            std::function<void(const Duration &)> m_on_update;
            std::function<void(void)> m_on_end;

        public:
            void set_name(const char *in_name);
            constexpr inline void set_delay(Duration in_delay) noexcept { m_delay = in_delay; }
            constexpr inline void set_update_period(Duration in_update_period) noexcept { m_update_period = in_update_period; }
            constexpr inline void set_duration(Duration in_duration) noexcept { m_duration = in_duration; }
            inline void add_on_delay_start_callback(std::function<void(void)> in_callback) { m_on_delay_start = in_callback; }
            inline void add_on_delay_end_callback(std::function<void(void)> in_callback) { m_on_delay_end = in_callback; }
            inline void add_on_start_callback(std::function<void(void)> in_callback) { m_on_start = in_callback; }
            inline void add_on_update_callback(std::function<void(const Duration &)> in_callback) noexcept { m_on_update = in_callback; }
            inline void add_on_end_callback(std::function<void(void)> in_callback) noexcept { m_on_end = in_callback; }

            constexpr inline const char *get_name() const noexcept { return m_name; }
            constexpr inline const std::function<void(void)> &get_on_delay_start_handler() const noexcept { return m_on_delay_start; }
            constexpr inline const std::function<void(void)> &get_on_delay_end_handler() const noexcept { return m_on_delay_end; }
            constexpr inline const std::function<void(void)> &get_on_start_handler() const noexcept { return m_on_start; }
            constexpr inline const std::function<void(const Duration &)> &get_on_update_handler() const noexcept { return m_on_update; }
            constexpr inline const std::function<void(void)> &get_on_end_handler() const noexcept { return m_on_end; }

            virtual OmegaStatus start() noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;

            virtual constexpr Base &name(const char *in_name) noexcept = 0;
            virtual constexpr Base &delay(Duration) noexcept = 0;
            virtual constexpr Base &update_duration(Duration) noexcept = 0;
            virtual constexpr Base &duration(Duration) noexcept = 0;
        };
    } // namespace Chrono
} // namespace Omega