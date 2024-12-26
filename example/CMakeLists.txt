cmake_minimum_required(VERSION 3.10)
project(MyAVRProjectExample)

# Find Boost
find_package(Boost 1.66 REQUIRED)

# MCU and frequency
set(MCU atmega328p)
set(F_CPU 16000000UL)

# Compiler and flags
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os")

# Find all example files in the src directory
file(GLOB EXAMPLE_FILES src/example*.cpp)

foreach(EXAMPLE_FILE IN LISTS EXAMPLE_FILES)
    # Get the filename without the directory (example1.cpp)
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_FILE} NAME_WE) # example1, example2, etc.
    
    # Define the executable for each example
    add_executable(${EXAMPLE_NAME}
      ${EXAMPLE_FILE}
      include/AVRLite.cpp
    )
    
    # Include directories
    target_include_directories(${EXAMPLE_NAME} PUBLIC  
      ${CMAKE_SOURCE_DIR}/include
      ${Boost_INCLUDE_DIRS}
    )
    
    # Link libraries
    target_link_libraries(${EXAMPLE_NAME} 
      ${Boost_LIBRARIES}
    )
    
    # Generate HEX file directly after building the executable
    add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
      COMMAND avr-objcopy -O ihex ${EXAMPLE_NAME} ../firmware/${EXAMPLE_NAME}_out.hex
    )
endforeach()
