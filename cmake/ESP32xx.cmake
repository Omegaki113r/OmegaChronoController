set(PROJ_SOURCES    
                    ${CHRONO_ROOT_DIR}/src/FreeRTOSController.cpp
                    ${CHRONO_ROOT_DIR}/src/ESP32xxHiResController.cpp
                    ${CHRONO_ROOT_DIR}/src/ESP32xxGPController.cpp
                )
idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    ${CHRONO_ROOT_DIR}/inc
                        REQUIRES        OmegaUtilityDriver esp_timer esp_driver_gptimer
                        )