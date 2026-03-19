cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(remotecontrol)

target_include_directories(app PRIVATE
    inc/
    )

target_sources(app PRIVATE
    src/main.c
    )

target_sources_ifdef(CONFIG_GPIO_BUTTON app PRIVATE
    src/gpio_button.c
    )
