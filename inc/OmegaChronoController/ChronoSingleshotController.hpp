/**
 * @file ChronoSingleshotController.hpp
 * @author Omegaki113r
 * @date Wednesday, 29th January 2025 4:19:29 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaChronoController
 * File Name: ChronoSingleshotController.hpp
 * File Created: Wednesday, 29th January 2025 4:19:29 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Tuesday, 4th February 2025 4:21:00 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
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
        template <typename T>
        class Singleshot : public Base
        {
            T core;

        public:
            constexpr Singleshot(T in_core) : core(in_core) {}

            OmegaStatus start() noexcept override
            {
                const auto timer_task = [](void *arg)
                {
                    Singleshot *controller = (Singleshot *)arg;
                    if (Duration{0} < controller->delay)
                    {
                        const auto on_delay_start = controller->get_on_delay_start_handler();
                        if (on_delay_start)
                            on_delay_start();
                        controller->core.start(controller->delay, controller->delay);
                        const auto on_delay_end = controller->get_on_delay_end_handler();
                        if (on_delay_end)
                            on_delay_end();
                    }
                    if (Duration(0) < controller->duration)
                    {
                        const auto on_start = controller->get_on_start_handler();
                        if (on_start)
                            on_start();
                        controller->core.start(controller->duration, controller->duration);
                        const auto on_end = controller->get_on_end_handler();
                        if (on_end)
                            on_end();
                    }
                    vTaskDelete(nullptr);
                };
                if (const auto err = xTaskCreate(timer_task, "timer_task", configMINIMAL_STACK_SIZE * 2, this, configMAX_PRIORITIES - 2, nullptr); pdPASS != err)
                    return eFAILED;
                return eSUCCESS;
            }

            // OmegaStatus start(const Duration &delay, const Duration &update_period, const Duration &duration,
            //                   std::function<void(void)> on_start, std::function<void(const ::Omega::Chrono::Duration &)> on_update, std::function<void(void)> on_end) noexcept override
            // {
            // const auto on_start_handler = [this]()
            // {
            //     OMEGA_LOGI("[%s] => on start", get_name());
            //     if (this->on_start)
            //         this->on_start();
            // };
            // const auto on_update_handler = [this](const ::Omega::Chrono::Duration &duration)
            // {
            //     OMEGA_LOGI("[%s] => on update | [%.2d:%.2d:%.2d.%.3d.%.3lld]", get_name(), duration.h, duration.m, duration.s, duration.ms, duration.us);
            //     if (this->on_update)
            //         this->on_update(duration);
            // };
            // const auto on_end_handler = [this]()
            // {
            //     OMEGA_LOGI("[%s] => on end", get_name());
            //     if (this->on_end)
            //         this->on_end();
            // };
            // return core.start(delay, update_period, duration, on_start_handler, on_update_handler, on_end_handler);
            //     return eFAILED;
            // }

            // void start_immediate() noexcept override
            // {
            // core.start_immediate(delay, update_period, duration, on_start, on_update, on_end);
            // }

            // void start_immediate(const Duration &delay, const Duration &update_period, const Duration &duration,
            //                      std::function<void(void)>, std::function<void(const ::Omega::Chrono::Duration &)>, std::function<void(void)>) noexcept override
            // {
            //     const auto on_start_handler = [this]()
            //     {
            //         OMEGA_LOGI("[%s] => on start", get_name());
            //         if (on_start)
            //             on_start();
            //     };
            //     const auto on_update_handler = [this](const ::Omega::Chrono::Duration &duration)
            //     {
            //         OMEGA_LOGI("[%s] => on update | [%.2d:%.2d:%.2d.%.3d.%.3lld]", get_name(), duration.h, duration.m, duration.s, duration.ms, duration.us);
            //         if (on_update)
            //             on_update(duration);
            //     };
            //     const auto on_end_handler = [this]()
            //     {
            //         OMEGA_LOGI("[%s] => on end", get_name());
            //         if (on_end)
            //             on_end();
            //     };
            //     // core.start_immediate(delay, update_period, duration, on_start_handler, on_update_handler, on_end_handler);
            // }

            OmegaStatus pause() noexcept override { return eFAILED; }

            OmegaStatus resume() noexcept override { return eFAILED; }

            OmegaStatus stop() noexcept override { return eFAILED; }
        };
    } // namespace Chrono
} // namespace Omega