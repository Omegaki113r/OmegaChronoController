set(PROJ_INC_DIR ${PROJ_ROOT_DIR}/inc)
set(PROJ_SOURCES    
                    ${PROJ_ROOT_DIR}/src/FreeRTOSController.cpp
                    ${PROJ_ROOT_DIR}/src/ESP32xxHiResController.cpp
                    ${PROJ_ROOT_DIR}/src/ESP32xxGPController.cpp
                )
idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    ${PROJ_INC_DIR}
                        REQUIRES        OmegaUtilityDriver esp_timer esp_driver_gptimer
                        )