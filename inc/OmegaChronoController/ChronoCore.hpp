/**
 * @file ChronoCore.hpp
 * @author Omegaki113r
 * @date Monday, 3rd February 2025 6:01:19 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoCore.hpp
 * File Created: Monday, 3rd February 2025 6:01:19 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 6th February 2025 5:38:37 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/ChronoBase.hpp"
#include "OmegaChronoController/ChronoCallbacks.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {

        class Core
        {
            virtual OmegaStatus start(Duration update_period, Duration duration) noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;

        protected:
            on_started_handler m_on_started;
            on_resumed_handler m_on_resumed;
            on_update_handler m_on_update;
            on_paused_handler m_on_paused;
            on_stopped_handler m_on_stopped;

        public:
            inline void add_on_start_callback(on_started_handler in_callback) { m_on_started = in_callback; }
            inline void add_on_resume_callback(on_resumed_handler in_callback) { m_on_resumed = in_callback; }
            inline void add_on_update_callback(on_update_handler in_callback) { m_on_update = in_callback; }
            inline void add_on_pause_callback(on_paused_handler in_callback) { m_on_paused = in_callback; }
            inline void add_on_stop_callback(on_stopped_handler in_callback) { m_on_stopped = in_callback; }

            inline const on_started_handler &get_on_start() const noexcept { return m_on_started; }
            inline const on_resumed_handler &get_on_resume() const noexcept { return m_on_resumed; }
            inline const on_update_handler &get_on_update() const noexcept { return m_on_update; }
            inline const on_paused_handler &get_on_pause() const noexcept { return m_on_paused; }
            inline const on_stopped_handler &get_on_stop() const noexcept { return m_on_stopped; }
        };
    } // namespace Chrono
} // namespace Omega
