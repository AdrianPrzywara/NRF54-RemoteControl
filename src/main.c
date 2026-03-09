#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/*  500 msec */
#define SLEEP_TIME_MS 500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led))
	{
		return -1;
	}
	if (!gpio_is_ready_dt(&button))
	{
        return -1;
	}
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return -1;
	}
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret != 0)
	{
        return -1;
    }


	while (1) {
		int button_state = gpio_pin_get_dt(&button);

		if (0 > button_state)
		{
			return -1;
		}
		else if (button_state > 0)
		{
			ret = gpio_pin_toggle_dt(&led);
			if (ret < 0) {
				return -1;
			}
			led_state = !led_state;
			k_msleep(SLEEP_TIME_MS);
		}
		else
		{
			/* Button not active*/
		}
	}
	return 0;
}
