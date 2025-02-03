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
 * Last Modified: Tuesday, 4th February 2025 12:18:54 am
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
        // enum class Type
        // {
        //     ePERIODIC,
        //     eSINGLE_SHOT,
        //     eCOUNTDOWN,
        //     eCOUNTUP
        // };

        class Type
        {
        private:
            /* data */
        public:
        };

        class Singleshot : public Type
        {
        };

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
            Duration delay;
            Duration update_period;
            Duration duration;
            std::function<void(void)> on_start;
            std::function<void(const ::Omega::Chrono::Duration &)> on_update;
            std::function<void(void)> on_end;
            virtual OmegaStatus start(const Duration &delay, const Duration &update_period, const Duration &duration,
                                      std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept = 0;
            virtual void start_immediate(const Duration &delay, const Duration &update_period, const Duration &duration,
                                         std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept = 0;
            virtual OmegaStatus pause() noexcept = 0;
            virtual OmegaStatus resume() noexcept = 0;
            virtual OmegaStatus stop() noexcept = 0;
        };

    } // namespace Chrono
} // namespace Omega