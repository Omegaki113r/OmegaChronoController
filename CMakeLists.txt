set(PROJ_SOURCES    
                    src/FreeRTOSController.cpp
                    src/ESP32xxHiResController.cpp
                    src/ESP32xxGPController.cpp
                )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver esp_timer esp_driver_gptimer
                        )
