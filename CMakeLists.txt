cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(tipsse_gnss)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(tipsse_gnss
    main.c
)

pico_generate_pio_header(tipsse_gnss ${CMAKE_CURRENT_LIST_DIR}/max2771_spi.pio)

# enable usb output, disable uart output
pico_enable_stdio_usb(tipsse_gnss 1)
pico_enable_stdio_uart(tipsse_gnss 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(tipsse_gnss
    pico_stdlib
    hardware_pio
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(tipsse_gnss)