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
 * Last Modified: Monday, 27th January 2025 8:48:16 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#pragma once

#include "OmegaUtilityDriver/UtilityDriver.hpp"

#ifdef __cplusplus

namespace Omega
{
    namespace Chrono
    {
        enum class Type
        {
            ePERIODIC,
            eSINGLE_SHOT,
            eCOUNTDOWN,
            eCOUNTUP
        };

        class FreeRTOS
        {
        public:
            FreeRTOS(Type in_type = Type::eSINGLE_SHOT);
            ~FreeRTOS();

        private:
        };
    } // namespace Chrono
} // namespace Omega
#else
#endif