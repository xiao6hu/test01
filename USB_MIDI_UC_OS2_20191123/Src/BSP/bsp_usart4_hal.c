#include "includes.h"


extern  void SystemClock_Config(void);
//extern  void MX_GPIO_Init(void);
//void MX_USB_HOST_Process(void);




/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	Delay_init();
	AT24CXX_Init();
		
#if 1
	LodeParametersData();
	
	
	//FLASH_WRITE(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 	
	//FLASH_READ(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 
	
	
	/* Initialize all configured peripherals */
	//MX_GPIO_Init();
	MX_FATFS_Init();
	MX_USB_HOST_Init();
	/* USER CODE BEGIN 2 */
	//sd_detect_gpio_confing();
	KEY_Init();
	UI_Init();
	USART1_Config();
	USART3_Config();
	
	//TIM7_Configuration();
#endif
    LED_GPIO_Config();  /* LED �˿ڳ�ʼ�� */	
}




/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
    //SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/OS_TICKS_PER_SEC);
}



