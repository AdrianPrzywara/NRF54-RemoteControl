/* Main application file */


/***************************************************** Includes ******************************************************/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_GPIO_BUTTON
#include "gpio_button.h"
#endif /* CONFIG_GPIO_BUTTON */

/****************************************************** Config *******************************************************/

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/****************************************************** Defines ******************************************************/

#define SLEEP_TIME_MS (1000u)

/***************************************************** Variables *****************************************************/

/******************************************** Local functions declarations *******************************************/

/******************************************** Local functions definitions ********************************************/

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

	LOG_INF("Main application finished initialization");
	for (;;)
	{
		k_msleep(SLEEP_TIME_MS);
	}
}
