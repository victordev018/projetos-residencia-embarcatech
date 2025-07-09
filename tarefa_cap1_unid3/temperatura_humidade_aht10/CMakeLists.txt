# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(temperatura_humidade_aht10 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Reference to main code
file(GLOB_RECURSE MAIN "src/temperatura_humidade_aht10.c")

# Reference to utility files
file(GLOB_RECURSE UTILS "src/utils/**/*.c")

# Reference to drivers
file(GLOB_RECURSE DRIVERS "src/drivers/*.c")

# Add executable. Default name is the project name, version 0.1
add_executable(temperatura_humidade_aht10 
    ${MAIN}
    ${UTILS}
    ${DRIVERS}
)

pico_set_program_name(temperatura_humidade_aht10 "temperatura_humidade_aht10")
pico_set_program_version(temperatura_humidade_aht10 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(temperatura_humidade_aht10 0)
pico_enable_stdio_usb(temperatura_humidade_aht10 1)

# Add the standard library to the build
target_link_libraries(temperatura_humidade_aht10
    pico_stdlib
    hardware_i2c
)

# Add the standard include files to the build
target_include_directories(temperatura_humidade_aht10 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/aht10
    ${CMAKE_CURRENT_LIST_DIR}/src/drivers
    ${CMAKE_CURRENT_LIST_DIR}/src/utils/display
)

pico_add_extra_outputs(temperatura_humidade_aht10)

