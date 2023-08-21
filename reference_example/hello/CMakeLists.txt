cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(../../pico-sdk/pico_sdk_init.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(hello
    main.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello 1)
pico_enable_stdio_uart(hello 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(hello pico_stdlib)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(hello)