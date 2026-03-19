/*
 * @file gpio_button.c
 *
 * @brief This file contains the implementation of the GPIO button and LED initialization and handling.
 *
 * @author [Your Name]
 * @date [Date]
 */


/***************************************************** Includes ******************************************************/

#include "gpio_button.h"

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
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

/******************************************* Exported functions definitions ******************************************/

void Gpio_LedInit(void)
{
    /* Initialization */
    if ((!gpio_is_ready_dt(&led)) ||
        (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0))
    {
        /* Initialization failed */
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
    }
    else
    {
        /* Initialization successful, add interrupt callback */
        gpio_init_callback(&button_cb_data, &button_pressed, BIT(button.pin));
        gpio_add_callback(button.port, &button_cb_data);
    }
}

