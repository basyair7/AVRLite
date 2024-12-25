cmake_minimum_required(VERSION 3.10)
project(MyAVRProject)

# Find Boost
find_package(Boost 1.66 REQUIRED)

# MCU and frequency
set(MCU atmega328p)
set(F_CPU 16000000UL)

# Compiler and flags
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")

# Define the executable name as a variable
set(EXECUTABLE_NAME example1)

# Source files
add_executable(${EXECUTABLE_NAME} 
    # includes/AVRLite.cpp
    src/${EXECUTABLE_NAME}.cpp
)

# Include directories
target_include_directories(${EXECUTABLE_NAME} PUBLIC  
  ${CMAKE_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${EXECUTABLE_NAME} 
  ${Boost_LIBRARIES}
)

# Includes AVR Directories
include_directories(include)

# Generate HEX file directly after building the executable
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND avr-objcopy -O ihex ${EXECUTABLE_NAME} ../firmware/${EXECUTABLE_NAME}.hex
)
