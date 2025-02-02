set(PROJ_SOURCES    
                    src/ChronoBaseController.cpp
                    src/ChronoController.cpp
                    src/ChronoCountdownController.cpp
                    src/ChronoCountupController.cpp
                    src/ChronoFreeRTOSController.cpp
                    src/ChronoPeriodicController.cpp
                    src/ChronoSingleshotController.cpp
                )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver
                        )
