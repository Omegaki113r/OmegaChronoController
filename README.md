# OmegaChronoController
 
 Provide highly abstracted API to control hardware or software timers. Also provide an abstract class that can be extended to integrate more timer varients other than the ones provided out of the box. 
 
 Currently supported varients,
 - Singleshot timers
 - Periodic timers
 	- Indefinitely running periodic timers
	- Countdown timers
	- Countup timers

 Countdown timer and countup timer is just an extended implementation of periodic timers

 Timer varients extends a base class called `::Omega::Chrono::Base`

 Currently supported abstration layers,
 - FreeRTOS
 - ESP32 High Resolution Timer
 - ESP32 General Purpose Timer

 Each abstraction layer extends a base class called `::Omega::Chrono::Core`

 So usually when instantiating a timer following flow is used,

 `::Omega:Chrono::Base timer = ::Omega::Chrono::Base(::Omega::Chrono::Core())`

### Dependencies
>- [OmegaUtilityDriver](https://github.com/Omegaki113r/OmegaUtilityDriver "OmegaUtilityDriver")
>
> This is an Opensource MIT licenced supporting library that includes the logging feature that is being used in the OmegaChornoController as well as the definitions and implementation of ``Duration`` which defines data structure for timing (Delay, Duration, Update Period)
 
## Singleshot Controller

```cpp
::Omega::Chrono::Singleshot singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::Core());
```

## Periodic Controller

1. Periodic
```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::Core());
```
2. Countdown
```cpp
::Omega::Chrono::Countdown countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::Core());
```
3. Countup
```cpp
::Omega::Chrono::Countup countup = ::Omega::Chrono::Countup(::Omega::Chrono::Core());
```


## Abstraction Layers
1. FreeRTOS
```cpp
::Omega::Chrono::Singleshot singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::FreeRTOS());
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS());
::Omega::Chrono::Countdown countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::FreeRTOS());
::Omega::Chrono::Countup countup = ::Omega::Chrono::Countup(::Omega::Chrono::FreeRTOS());
```

2. ESP32 High Resolution Timer
```cpp
::Omega::Chrono::Singleshot singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::ESP32xxHiRes());
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::ESP32xxHiRes());
::Omega::Chrono::Countdown countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::ESP32xxHiRes());
::Omega::Chrono::Countup countup = ::Omega::Chrono::Countup(::Omega::Chrono::ESP32xxHiRes());
```
3. ESP32 General Purpose Timer
```cpp
::Omega::Chrono::Singleshot singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::ESP32xxGP());
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::ESP32xxGP());
::Omega::Chrono::Countdown countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::ESP32xxGP());
::Omega::Chrono::Countup countup = ::Omega::Chrono::Countup(::Omega::Chrono::ESP32xxGP());
```

## Events from timers

Following events are supported by `::Omega::Chrono::Base` and callbacks can be registered on the timers to receive the events. 
1. on start `std::function<void(const char *)>`

2. on resumed `std::function<void(const char *)>`

3. on update `std::function<bool(const char *,const Duration)>`

4. on paused `std::function<void(const char *)>`

5. on stopped `std::function<void(const char *)>`

#### Examples
```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS());

const auto on_start = [&](const char *name)
{ 
    OMEGA_LOGI("[%s] started", name); 
};
const auto on_resume = [&](const char *name)
{ 
    OMEGA_LOGI("[%s] resumed", name); 
};
const auto on_update = [&](const char *name, const ::Omega::Chrono::Duration &duration)
{ 
    OMEGA_LOGI("[%s] Update: %.2d:%.2d:%.2d.%.3d.%.3lld",name, duration.h, duration.m, duration.s, duration.ms, duration.us); 
    return true; 
};
const auto on_pause = [&](const char *name)
{ 
    OMEGA_LOGI("[%s] paused", name); 
};
const auto on_stop = [&](const char *name)
{ 
    OMEGA_LOGI("[%s] stopped", name); 
};

singleshot.add_on_start_callback(on_start);
singleshot.add_on_resume_callback(on_resume);
singleshot.add_on_update_callback(on_update);
singleshot.add_on_pause_callback(on_pause);
singleshot.add_on_stop_callback(on_stop);

periodic.start();
```

## Controling timers

Timers do not automatically run once instantiated. They need to be explicitly started to be ran. Just like starting there are additional ways to control the timers

1. Start
2. Resume
3. Pause
4. Stop

All timers can be controlled by the above functions eventhough some of them doesn't make any sense to control. Like Singleshot. unlike Periodic timers singleshot timers doesn't usually needs to be stopped or paused or resumed in the real world applications. Atleast i can't think of any reason for that. But i have provided that feature just in case.

#### Examples
```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS());

periodic.start();
periodic.pause();
periodic.resume();
periodic.stop();
```

Even if the start function is called on timers there is a option to delay the start of the timer by a set amount. this is set by using `delay` setters inside the `::Omega::Chrono::Base`
```cpp
constexpr Base &delay(Duration) noexcept;
constexpr inline void set_delay(Duration in_delay) noexcept;
```

#### Examples
```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                                        .delay({0,0,10});   // This delays the start of the timer by 10 seconds
::Omega::Chrono::Periodic periodic1 = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                                        .delay({0,10,0});   // This delays the start of the timer by 10 minutes
::Omega::Chrono::Periodic periodic2 = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                                        .delay({0,0,0,50}); // This delays the start of the timer by 50 milliseconds                                        


::Omega::Chrono::Periodic periodic3 = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS());

periodic.start();
periodic1.start();
periodic2.start();

periodic3.set_delay({0,0,0,100}); // This delays the start of the timer by 100 milliseconds  
periodic3.start();
```

Duration and Update Period of all timers can be set by `duration` and `update period` setters inside the `::Omega::Chrono::Base`

update period is set to get the on_update event in a pre-defined interval. 
Assume that you are making a countdown timer. usually you need to update the relavent systems (UI) atleast once a second. Therefore you can set the update period to 1 second. Then when the timer start running you will receive an event on the on_update funtion if you have registered any.

```cpp
constexpr Base &duration(Duration) noexcept;
constexpr inline void set_duration(Duration in_delay) noexcept;

constexpr Base &update_period(Duration) noexcept;
constexpr inline void set_update_period(Duration in_delay) noexcept;
```

#### Examples
```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                                        .duration({0,0,10});        // This duration of the timer is 10 seconds                                      


::Omega::Chrono::Periodic periodic2 = ::Omega::Chrono::Periodic(::Omega::Chrono::FreeRTOS())
                                        .update_period({0,0,1});    // This update period of the timer is 10 seconds  
                                        .duration({0,0,10});        // This duration of the timer is 10 seconds  

periodic.start();
periodic2.start();
```