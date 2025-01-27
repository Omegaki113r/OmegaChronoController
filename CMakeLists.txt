set(PROJ_SOURCES    
                    src/ChronoController.cpp
                    )

idf_component_register( SRCS            ${PROJ_SOURCES}
                        INCLUDE_DIRS    inc
                        REQUIRES        OmegaUtilityDriver
                        )