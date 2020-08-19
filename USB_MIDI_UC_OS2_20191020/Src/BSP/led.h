#ifndef __LED_H
#define	__LED_H

//#include "stm32f10x.h"

#include "stm32f2xx_hal.h"

#define ON	0
#define OFF	1

#define WORK_LED_GPIO GPIOC
#define WORK_LED_PIN   GPIO_PIN_7
#define WORK_LED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define WORK_LED(PinState)	HAL_GPIO_WritePin(WORK_LED_GPIO, WORK_LED_PIN, (GPIO_PinState)PinState)
                        
void LED_GPIO_Config(void);

#endif /* __LED_H */
