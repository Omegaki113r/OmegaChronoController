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
> This is an Opensource MIT licenced supporting library that includes the logging feature that is being used in the OmegaChornoController as well as the definitions and implementation of ``Duration`` which defines the time
 
## Singleshot Controller

```cpp
::Omega::Chrono::Singleshot singleshot = ::Omega::Chrono::Singleshot(::Omega::Chrono::Core());
```

## Periodic Controller

1. Periodic

```cpp
::Omega::Chrono::Periodic periodic = ::Omega::Chrono::Periodic(::Omega::Chrono::Core());
```
---
2. Countdown

```cpp
::Omega::Chrono::Countdown countdown = ::Omega::Chrono::Countdown(::Omega::Chrono::Core());
```
---
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
