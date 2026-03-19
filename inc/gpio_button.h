#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

/****************************************************** Defines ******************************************************/

#include <zephyr/drivers/gpio.h>

/******************************************* Exported functions definitions ******************************************/

void Gpio_LedInit(void);
void Gpio_ButtonInit(void);

#endif /* GPIO_BUTTON_H */