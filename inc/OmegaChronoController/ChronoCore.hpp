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
 * Last Modified: Wednesday, 5th February 2025 12:40:09 am
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
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class Core
        {
            std::function<void(void)> m_on_start;
            std::function<void(const Duration &)> m_on_update;
            std::function<void(void)> m_on_end;

            virtual OmegaStatus start(Duration update_period, Duration duration, std::function<void(const Duration &)> update_callback) noexcept = 0;

        public:
            inline void add_on_start_callback(std::function<void(void)> in_callback) { m_on_start = in_callback; }
            inline void add_on_update_callback(std::function<void(const Duration &)> in_callback) { m_on_update = in_callback; }
            inline void add_on_end_callback(std::function<void(void)> in_callback) { m_on_end = in_callback; }
        };
    } // namespace Chrono
} // namespace Omega
