/*
 * @file gpio_button.c
 *
 * @brief This file contains the implementation of the GPIO button and LED initialization and handling.
 */


/***************************************************** Includes ******************************************************/

#include "gpio_button.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/****************************************************** Config *******************************************************/

LOG_MODULE_REGISTER(gpio_button, LOG_LEVEL_DBG);

/****************************************************** Defines ******************************************************/

#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

/***************************************************** Variables *****************************************************/

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_cb_data;

/******************************************** Local functions declarations *******************************************/

static void button_pressed(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t  pins);

/******************************************** Local functions definitions ********************************************/

static void button_pressed(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t  pins)
{
    gpio_pin_toggle_dt(&led);
    LOG_INF("Button pressed, toggling LED");
}

/******************************************* Exported functions definitions ******************************************/

void Gpio_LedInit(void)
{
    /* Initialization */
    if ((!gpio_is_ready_dt(&led)) ||
        (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0))
    {
        /* Initialization failed */
        LOG_ERR("Error: LED device %s is not ready", led.port->name);
    }
}

void Gpio_ButtonInit(void)
{
    /* Initialization */
    if ((!gpio_is_ready_dt(&button)) ||
        (gpio_pin_configure_dt(&button, GPIO_INPUT) < 0) ||
        (gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE) < 0))
    {
        /* Initialization failed */
        LOG_ERR("Error: Button device %s is not ready", button.port->name);
    }
    else
    {
        /* Initialization successful, add interrupt callback */
        gpio_init_callback(&button_cb_data, &button_pressed, BIT(button.pin));
        gpio_add_callback(button.port, &button_cb_data);
        LOG_INF("Initialized button at %s pin %d", button.port->name, button.pin);
    }
}

void Gpio_SwitchLed(void)
{
    /* This function can be used to toggle the LED state if needed */
    gpio_pin_toggle_dt(&led);
}
