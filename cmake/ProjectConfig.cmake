# Select project type
set(_PN ${PROJECT_NAME})

#Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release'.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Target
# TODO: make target
# for example: add_library(${LIBRARY_NAME} ${LIBRARY_TYPE} ${SOURCES} ${HEADERS})

# Install project
# TODO: make install project
