/**
 * @file ChronoCore.hpp
 * @author Omegaki113r
 * @date Monday, 3rd February 2025 6:01:19 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoCore.hpp
 * File Created: Monday, 3rd February 2025 6:01:19 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 3:41:40 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include "OmegaChronoController/ChronoBaseController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

namespace Omega
{
    namespace Chrono
    {
        class Core
        {
            virtual void start(Duration update_period, Duration duration) noexcept = 0;
        };
    } // namespace Chrono
} // namespace Omega
