/* Main application file */


/***************************************************** Includes ******************************************************/

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#ifdef CONFIG_GPIO_BUTTON
#include "gpio_button.h"
#endif /* CONFIG_GPIO_BUTTON */

/****************************************************** Defines ******************************************************/

/***************************************************** Variables *****************************************************/

/******************************************** Local functions declarations *******************************************/

/******************************************** Local functions definitions ********************************************/

/******************************************* Exported functions definitions ******************************************/
int main(void)
{
#ifdef CONFIG_GPIO_BUTTON
	printk("GPIO Button Switching Enabled\n");
	Gpio_LedInit();
	Gpio_ButtonInit();
#else
	printk("GPIO Button Switching Disabled\n");
#endif /* CONFIG_GPIO_BUTTON */

	for (;;)
	{
		k_yield();
	}
}
