set(PROJ_SOURCES    
                    src/ChronoBase.cpp
                    src/ChronoCore.cpp
                    src/ChronoSingleshotController.cpp
                    src/ChronoFreeRTOSController.cpp
                )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver
                        )
