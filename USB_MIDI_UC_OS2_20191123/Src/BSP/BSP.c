#include "includes.h"
#include "mcohw.h"

extern  void SystemClock_Config(void);
extern  void MX_GPIO_Init(void);
extern void HAL_UART_MspInit(UART_HandleTypeDef* huart);

IWDG_HandleTypeDef hiwdg;
BYTE MCOHW_Init (void);


u8   Baud_Data=0x01;
//u16 setData=0x45;

extern OD_TABLE OD[MAX_OD_SIZE];
extern WORD volatile gTimCnt;
extern BYTE ODNodeID;

typedef struct
{
  u8 BaudRate_Data;
  u8 ODNodeID_Data;
}e2prom;
e2prom Image;
#define SIZE sizeof(Image)

u32 *pTPRO1_val[2];
u32 *pRPRO1_val[2];
u32 * pODNodeID;



void initTimer(void);
unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);


void RESET_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RESET_GPIO_CLK_ENABLE();
    GPIO_InitStructure.Pin = RESET_PIN;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;       
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(RESET_GPIO, &GPIO_InitStructure);
    RESET_ENABLE();
	delay_ms(10);
	RESET_DISABLE();
}

void T1000_POWER_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    T1000_POWER_GPIO_CLK_ENABLE();
    GPIO_InitStructure.Pin = T1000_POWER_PIN;	
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;       
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(T1000_POWER_GPIO, &GPIO_InitStructure);
    T1000_POWER_OFF();
}

/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	Delay_init();
	delay_ms(1000);
	
	
	AT24CXX_Init();
		
#if 1
	LodeParametersData();
	
	
	//FLASH_WRITE(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 	
	//FLASH_READ(FlashSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime,RamSetParameters.MusicDelayTime); 
	
	
	/* Initialize all configured peripherals */
	
	
	
	T1000_POWER_GPIO_Config();
	RESET_GPIO_Config();
	MX_GPIO_Init();
	MX_USB_HOST_Init();
	
	
	
	
	MX_USART2_UART_Init();
	
	//MX_FATFS_Init();
	/* USER CODE BEGIN 2 */
	sd_detect_gpio_confing();
	KEY_Init();
	UI_Init();
	//USART1_Config();
	
	XFS5152CE_Config();
	
	UART4_Config();
	/*init the Canopen thread*/
	
	CanopenInit(); 
	
	#if 0
	//MCOHW_Init ();
	
	Image.ODNodeID_Data=1;
	Image.BaudRate_Data=9;

	ODNodeID=Image.ODNodeID_Data;
	Baud_Data=Image.BaudRate_Data;
	//SYS_Tick_Configuration();
	//NVIC_Configuration();

	initTimer();
	canInit(CAN1,CAN_BAUD_125K);    
	MCOUSER_ResetCommunication();
	
	
	pTPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x2000,0x00)].Val;
	pTPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x2000,0x01)].Val;
	
	pRPRO1_val[0]=&OD[MCO_Search_ODTABLE(0x6000,0x00)].Val;
	pRPRO1_val[1]=&OD[MCO_Search_ODTABLE(0x6000,0x01)].Val;
	
	pODNodeID=&OD[MCO_Search_ODTABLE((0x1800 + 0),0x01)].Val;
	#endif
	
	
	//TIM7_Configuration();
#endif
    //LED_GPIO_Config();  /* LED 端口初始化 */	


	RTC_Init();

}





/*
 * 函数名：SysTick_init
 * 描述  ：配置SysTick定时器
 * 输入  ：无
 * 输出  ：无
 */
void SysTick_init(void)
{
    //SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);//初始化并使能SysTick定时器
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/OS_TICKS_PER_SEC);
}

void IWDG_Init(void)
{
	hiwdg.Instance=IWDG;
	hiwdg.Init.Prescaler= IWDG_PRESCALER_32;           
    hiwdg.Init.Reload=1250;  
	HAL_IWDG_Init(&hiwdg);
}




