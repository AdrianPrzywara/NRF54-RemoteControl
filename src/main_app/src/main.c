/* Main application file */


/***************************************************** Includes ******************************************************/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>

#ifdef CONFIG_GPIO_BUTTON
#include "gpio_button.h"
#endif /* CONFIG_GPIO_BUTTON */

/****************************************************** Config *******************************************************/

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/****************************************************** Defines ******************************************************/

#define SLEEP_TIME_MS 			(1000u)
#define UART_BUFFER_SIZE 		(64u)
#define UART_RECEIVE_TIMEOUT	(100)

/***************************************************** Variables *****************************************************/

const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart20));

static uint8_t rx_buffer[UART_BUFFER_SIZE] = {0};

/******************************************** Local functions declarations *******************************************/

/******************************************** Local functions definitions ********************************************/

static void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
	/* Handle UART events here if needed */

	switch (evt->type)
	{
		case UART_RX_RDY:
			LOG_INF("UART RX Ready: Received %d bytes", evt->data.rx.len);
			if (evt->data.rx.len > 0u)
			{
				if (evt->data.rx.buf[evt->data.rx.offset] == 't')
				{
					LOG_INF("Received 't', toggling LED");
					Gpio_SwitchLed();
				}
			}
			break;
		
		case UART_RX_DISABLED:
			LOG_INF("UART RX Disabled");
			uart_rx_enable(dev, rx_buffer, sizeof(rx_buffer), UART_RECEIVE_TIMEOUT);
			break;

		default:
			LOG_DBG("UART Event: %d", evt->type);
			break;
	}
}

/******************************************* Exported functions definitions ******************************************/
int main(void)
{
	LOG_INF("Main application started\n");

#ifdef CONFIG_GPIO_BUTTON
	LOG_DBG("GPIO Button Switching Enabled");
	Gpio_LedInit();
	Gpio_ButtonInit();
#else
	LOG_DBG("GPIO Button Switching Disabled");
#endif /* CONFIG_GPIO_BUTTON */

	if (!device_is_ready(uart_dev))
	{
		LOG_ERR("UART device %s is not ready", uart_dev->name);
		return -1;
	}

	if (uart_callback_set(uart_dev, uart_callback, NULL) != 0)
	{
		LOG_ERR("Failed to set UART callback");
		return -1;
	}

	if (uart_rx_enable(uart_dev, rx_buffer, sizeof(rx_buffer), UART_RECEIVE_TIMEOUT) != 0)
	{
		LOG_ERR("Failed to enable UART RX");
		return -1;
	}

	LOG_INF("Main application finished initialization");
	for (;;)
	{
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
