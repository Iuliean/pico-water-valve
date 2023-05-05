cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_valve)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project


if (TARGET tinyusb_device)
    add_executable(pico_valve
            main.cpp
            )

    # pull in common dependencies
    target_link_libraries(pico_valve pico_stdlib hardware_rtc)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(pico_valve 1)
    pico_enable_stdio_uart(pico_valve 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(pico_valve)

    # add url via pico_set_program_url

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()