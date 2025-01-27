/**
 * @file ChronoController.cpp
 * @author Omegaki113r
 * @date Monday, 27th January 2025 8:22:29 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ChronoController.cpp
 * File Created: Monday, 27th January 2025 8:22:29 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Monday, 27th January 2025 8:48:49 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include "OmegaChronoController/ChronoController.hpp"
#include "OmegaUtilityDriver/UtilityDriver.hpp"

#ifdef __cplusplus
namespace Omega
{
    namespace Chrono
    {
        FreeRTOS::FreeRTOS(Type in_type)
        {
            switch (in_type)
            {
            case Type::eCOUNTDOWN:
            {
                break;
            }
            default:
            {
                break;
            }
            }
        }
        FreeRTOS::~FreeRTOS()
        {
        }
    } // namespace Chrono
} // namespace Omega
#else

#endif