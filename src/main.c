#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

/* Button callback data */
static struct gpio_callback button_cb_data;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	gpio_pin_toggle_dt(&led);
}

int main(void)
{
	/* Initialization */
	if (!gpio_is_ready_dt(&led))
	{
		return -1;
	}
	if (!gpio_is_ready_dt(&button))
	{
        return -1;
	}
	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
	{
		return -1;
	}
    if (gpio_pin_configure_dt(&button, GPIO_INPUT) < 0)
	{
        return -1;
    }
	if (gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_FALLING) < 0)
	{
		return -1;
	}
	gpio_init_callback(&button_cb_data, &button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	return 0;
}
