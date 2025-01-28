/**
 * @file ChronoCountdownController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:16:08 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoCountdownController.hpp
 * File Created: Wednesday, 29th January 2025 4:16:08 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Wednesday, 29th January 2025 4:17:49 am
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
#include "OmegaChronoController/ChronoController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class Countdown : public Base
        {
        public:
            Countdown(Duration in_duration = {0}, Duration in_update_period = {0}, Duration in_delay = {0}) : Base(Type::eCOUNTDOWN, in_duration, in_update_period, in_delay) {}
            ~Countdown();
        };
    } // namespace Chrono
} // namespace Omega
