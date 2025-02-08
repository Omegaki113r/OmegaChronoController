set(PROJ_SOURCES    
                    src/ChronoFreeRTOSController.cpp
                    src/ChronoESP32xxHiResController.cpp
                )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver esp_timer
                        )
