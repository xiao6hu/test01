#ifndef __BSP_H
#define	__BSP_H



#define RESET_GPIO 						GPIOC
#define RESET_PIN   					GPIO_PIN_8
#define RESET_GPIO_CLK_ENABLE()   		__HAL_RCC_GPIOC_CLK_ENABLE()

#define RESET_ENABLE() 					HAL_GPIO_WritePin(RESET_GPIO, RESET_PIN, GPIO_PIN_SET)
#define RESET_DISABLE() 				HAL_GPIO_WritePin(RESET_GPIO, RESET_PIN, GPIO_PIN_RESET)



#define T1000_POWER_GPIO 				GPIOC
#define T1000_POWER_PIN   				GPIO_PIN_10
#define T1000_POWER_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()

#define T1000_POWER_ON() 				HAL_GPIO_WritePin(T1000_POWER_GPIO, T1000_POWER_PIN, GPIO_PIN_SET)
#define T1000_POWER_OFF() 				HAL_GPIO_WritePin(T1000_POWER_GPIO, T1000_POWER_PIN, GPIO_PIN_RESET)


extern IWDG_HandleTypeDef hiwdg;


void SysTick_init(void);
void BSP_Init(void);
void IWDG_Init(void);


#endif // __BSP_H
