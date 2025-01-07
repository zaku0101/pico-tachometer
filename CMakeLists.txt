cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico-tachometer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME}
  tachometer.c
  font.c
  ssh1106.c
)

pico_add_extra_outputs(${PROJECT_NAME})

# Specify the source files for the library
target_sources(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/kiss_fft.c
    ${CMAKE_CURRENT_LIST_DIR}/kiss_fftr.c
    ${CMAKE_CURRENT_LIST_DIR}/pico_tachometer.c
    ${CMAKE_CURRENT_LIST_DIR}/calculation.c
)

# Specify the include directories for the library
target_include_directories(${PROJECT_NAME} INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  hardware_timer
  hardware_i2c
  pico_binary_info
  hardware_adc
  hardware_uart
  hardware_dma
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

