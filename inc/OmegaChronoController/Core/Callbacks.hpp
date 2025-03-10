/**
 * @file ChronoCallbacks.hpp
 * @author Omegaki113r
 * @date Thursday, 6th February 2025 5:36:44 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoCallbacks.hpp
 * File Created: Thursday, 6th February 2025 5:36:44 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 12th February 2025 1:29:05 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <functional>

#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        using on_delay_started_handler = std::function<void(const char *)>;
        using on_delay_stopped_handler = std::function<void(const char *)>;
        using on_started_handler = std::function<void(const char *)>;
        using on_resumed_handler = std::function<void(const char *)>;
        using on_update_handler = std::function<bool(const char *, const Duration)>;
        using on_paused_handler = std::function<void(const char *)>;
        using on_stopped_handler = std::function<void(const char *)>;
    } // namespace Chrono
} // namespace Omega
