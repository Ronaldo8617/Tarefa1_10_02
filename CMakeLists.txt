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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Tarefa1_10_02 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(Tarefa1_10_02 
    Tarefa1_10_02.c 
    inc/ssd1306.c 

)

pico_set_program_name(Tarefa1_10_02 "Tarefa1_10_02")
pico_set_program_version(Tarefa1_10_02 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Tarefa1_10_02 0)
pico_enable_stdio_usb(Tarefa1_10_02 1)  # Habilitar saída USB para depuração

# Add the standard library to the build
target_link_libraries(Tarefa1_10_02
    pico_stdlib
    hardware_pwm
    hardware_i2c
    hardware_adc
)

# Add the standard include files to the build
target_include_directories(Tarefa1_10_02 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    inc  # Incluir diretório com ssd1306.h e font.h
)

# Add any user requested libraries
target_link_libraries(Tarefa1_10_02 
    # Adicionar bibliotecas adicionais, se necessário
)

pico_add_extra_outputs(Tarefa1_10_02)
