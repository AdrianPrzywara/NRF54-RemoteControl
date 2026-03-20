target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_sources_ifdef(CONFIG_GPIO_BUTTON app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gpio_button.c
)
