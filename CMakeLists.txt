cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(tipsse_gnss)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(tipsse_gnss
    main.c
    max2771_spi.c
    server.c
    usb_descriptors.c
    reset_interface.c
    net.c
    packed_fs.c
    msc_disk.c
    mongoose/mongoose.c
    pico-sdk/lib/tinyusb/lib/networking/rndis_reports.c
)

pico_generate_pio_header(tipsse_gnss ${CMAKE_CURRENT_LIST_DIR}/max2771_spi.pio)

# enable usb output, disable uart output
pico_enable_stdio_usb(tipsse_gnss 1)
pico_enable_stdio_uart(tipsse_gnss 0)

target_include_directories(tipsse_gnss PUBLIC
    pico-sdk/lib/tinyusb/lib/networking
    mongoose
    ${CMAKE_SOURCE_DIR}
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(tipsse_gnss
    pico_stdlib
    hardware_pio
    tinyusb_device
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(tipsse_gnss)

find_package(Npm REQUIRED)
add_subdirectory(www)
add_custom_command(OUTPUT packed_fs.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/www
    COMMAND ${CMAKE_SOURCE_DIR}/bin/pack dist/* > ${CMAKE_BINARY_DIR}/packed_fs.c
    DEPENDS frontend
)

# Mongoose build flags
add_definitions(-DMG_ENABLE_TCPIP=1)
add_definitions(-DMG_ENABLE_PACKED_FS=1)
add_definitions(-DMG_ENABLE_FILE=0)
add_definitions(-DDISABLE_ROUTING=1)

# Example build options
add_definitions(-DHTTP_URL="http://0.0.0.0/")

# Extra build flags (enable if needed)
add_definitions(-DDUAL_CONFIG=1)

add_definitions(-DPICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1)
add_definitions(-DPICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_BOOTSEL=1)
add_definitions(-DPICO_STDIO_USB_RESET_INTERFACE_SUPPORT_RESET_TO_FLASH_BOOT=1)
add_definitions(-DPICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=1)
add_definitions(-DPICO_STDIO_USB_RESET_BOOTSEL_ACTIVITY_LED=1)