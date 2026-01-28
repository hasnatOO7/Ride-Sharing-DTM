cmake_minimum_required(VERSION 3.10)
project(RideShareSystem)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    City.cpp
    Driver.cpp
    Rider.cpp
    Trip.cpp
    DispatchEngine.cpp
    RollbackManager.cpp
    RideShareSystem.cpp
)

# Create executable
add_executable(RideShareSystem ${SOURCES})

# Set compiler flags for better output
if(MSVC)
    target_compile_options(RideShareSystem PRIVATE /W4)
else()
    target_compile_options(RideShareSystem PRIVATE -Wall -Wextra -Wpedantic)
endif()
