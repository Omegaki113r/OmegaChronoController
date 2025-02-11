set(PROJ_SOURCES    
                    src/ChronoFreeRTOSController.cpp
                    src/ChronoESP32xxHiResController.cpp
                    src/ChronoESP32xxGPController.cpp
                )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver esp_timer esp_driver_gptimer
                        )
