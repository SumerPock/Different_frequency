/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "includes.h"
#include "bsp.h"

void AppTaskStart(void *argument);
const osThreadAttr_t ThreadStart_Attr = 	
{
	.name = "osRtxStartThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh7,
	.stack_size = 2048,
};
osThreadId_t ThreadIdStart = NULL;				

void AppTaskLwIPPro(void *argument);	
const osThreadAttr_t ThreadLwIPPro_Attr = 		
{
	.name = "osRtxLwIPProThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskLwIPPro = NULL;	

void AppTaskEthCheck(void *argument);
const osThreadAttr_t ThreadEthCheck_Attr =
{
	.name = "osRtxEthCheckThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskEthCheck = NULL;	


void AppTaskUdpNetWork(void *argument);
const osThreadAttr_t ThreadUdpNetWork_Attr =
{
	.name = "osRtxUdpNetWorkThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 1024,
};
osThreadId_t ThreadIdTaskUdpNetWork = NULL;


void AppTaskCan1_answer(void *argument);
const osThreadAttr_t ThreadCan1answer_Attr = 
{
	.name = "osRtxCan1answerThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskcan1_anwser = NULL;


void AppTaskLED(void *argument);
const osThreadAttr_t ThreadLED_Attr = 
{
	.name = "osRtxLEDThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh6,
	.stack_size = 4096,
};
osThreadId_t ThreadIdTaskLED = NULL;

void AppTaskMotorFindZero(void *argument);
const osThreadAttr_t ThreadMotorFindZero_Attr =
{
	.name = "osRtxMotorFindZeroThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskMotorFindZero = NULL;

void AppTaskUnderMotorFindZero(void *argument);
const osThreadAttr_t ThreadUnderMotorFindZero_Attr =
{
	.name = "osRtxUnderMotorFindZeroThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskUnderMotorFindZero = NULL;

void AppTaskMotorPosition(void *argument);
const osThreadAttr_t TaskMotorPosition_Attr =
{
	.name = "osRtxMotorPositionThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskMotorPosition = NULL;

void AppTaskUnderMotorPosition(void *argument);
const osThreadAttr_t TaskUnderMotorPosition_Attr =
{
	.name = "osRtxUnderMotorPositionThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskUnderMotorPosition = NULL;


void AppTaskMotorSppedScanning(void *argument);
const osThreadAttr_t TaskMotorSppedScanning_Attr =
{
	.name = "osRtxMotorSppedScanningThread",
	.attr_bits = osThreadDetached,
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskMotorSppedScanning = NULL;

void AppTaskUnderMotorSppedScanning(void *argument);
const osThreadAttr_t TaskUnderMotorSppedScanning_Attr =
{
	.name = "osRtxUnderMotorSppedScanningThread",
	.attr_bits = osThreadDetached,
	.priority = osPriorityHigh5,
	.stack_size = 2048,
};
osThreadId_t ThreadIdTaskUnderMotorSppedScanning = NULL;


osMessageQueueId_t msgQueue_lm1266Num1Data; 
const osMessageQueueAttr_t lm1266Num1Data_Attr =	
{
	.name = "lm1266Num1Data",   																															
};


osMessageQueueId_t msgQueue_lm1266Num2Data; 
const osMessageQueueAttr_t lm1266Num2Data_Attr =	
{
	.name = "lm1266Num2Data",   								
																							
};


osMessageQueueId_t msgQueue_PositionData; 
const osMessageQueueAttr_t PositionData_Attr =	
{
	.name = "PositionData",   										
																								
};

osMessageQueueId_t msgQueue_UnderPositionData; 
const osMessageQueueAttr_t UnderPositionData_Attr =	
{
	.name = "UnderPositionData",   							
																							
};


osMessageQueueId_t msgQueue_SppedScanning; 
const osMessageQueueAttr_t SppedScanning_Attr =	
{
	.name = "SppedScanningData",   								
																								
};

osMessageQueueId_t msgQueue_UnderSppedScanning; 
const osMessageQueueAttr_t UnderSppedScanning_Attr =	
{
	.name = "UnderSppedScanningData",   					
																							
};


osMessageQueueId_t msgQueue_Can1ResData; 
const osMessageQueueAttr_t Can1ResData_Attr =	
{
	.name = "Can1ResData",   										
																							
};


osMessageQueueId_t msgQueue_Can2ResData; 			
const osMessageQueueAttr_t Can2ResData_Attr =
{
	.name = "Can2ResData",   					
};


osMessageQueueId_t msgQueue_UdpResData; 			
const osMessageQueueAttr_t UdpResData_Attr =
{
	.name = "UdpResData",   					
};


osMessageQueueId_t msgQueue_UdpSendData; 			
const osMessageQueueAttr_t UdpSendData_Attr =
{
	.name = "UdpSendData",   					
};


osMessageQueueId_t msgQueue_DytData; 			
const osMessageQueueAttr_t DytData_Attr =
{
	.name = "DytData",   					
};


osMessageQueueId_t msgQueue_MotorData; 			
const osMessageQueueAttr_t MotorData_Attr =
{
	.name = "MotorData",   					
};


const osEventFlagsAttr_t event_cable_Attr = 
{
	.name = "event_cable",   	
														
};
osEventFlagsId_t event_cable_ID = NULL;


const osEventFlagsAttr_t event_sppedzero_Attr = 
{
	.name = "event_sppedzero",  
															
};
osEventFlagsId_t event_sppedzero_ID = NULL;

const osEventFlagsAttr_t event_positionzero_Attr = 
{
	.name = "event_positionzero",   
																	
};
osEventFlagsId_t event_positionzero_ID = NULL; 


const osEventFlagsAttr_t event_positionarrive_Attr = 
{
	.name = "event_positionarrive",   
																		
};
osEventFlagsId_t event_positionarrive_ID = NULL; 


const osEventFlagsAttr_t event_negativelimit_Attr = 
{
	.name = "event_negativelimit",   
																	 
};
osEventFlagsId_t event_negativelimit_ID = NULL;

const osEventFlagsAttr_t event_belownegativelimit_Attr = 
{
	.name = "event_belownegativelimit",   
																				
};
osEventFlagsId_t event_belownegativelimit_ID = NULL; 


const osEventFlagsAttr_t event_closurefinddata_Attr = 
{
	.name = "event_closurefinddata",   	
																			
};
osEventFlagsId_t event_closurefinddata_ID = NULL; 

const osEventFlagsAttr_t event_belowclosurefinddata_Attr = 
{
	.name = "event_belowclosurefinddata",   	
																						
};
osEventFlagsId_t event_belowclosurefinddata_ID = NULL; 



const osEventFlagsAttr_t event_SppedScanning_Attr = 
{
	.name = "event_SppedScanning",   	
																		
};
osEventFlagsId_t event_SppedScanning_ID = NULL; 

const osEventFlagsAttr_t event_belowSppedScanning_Attr = 
{
	.name = "event_belowSppedScanning", 
																			
};
osEventFlagsId_t event_belowSppedScanning_ID = NULL; 


const osEventFlagsAttr_t event_lm1266_Attr = 
{
	.name = "event_lm1266",
															
};
osEventFlagsId_t event_lm1266_ID = NULL;


const osEventFlagsAttr_t event_uart1_Attr = 
{
	.name = "event_uart1",   	
														
};
osEventFlagsId_t event_uart1_ID = NULL;


const osEventFlagsAttr_t event_uart2_Attr = 
{
	.name = "event_uart2",   	
														
};
osEventFlagsId_t event_uart2_ID = NULL;


const osEventFlagsAttr_t event_uart3_Attr = 
{
	.name = "event_uart3",  
													
};
osEventFlagsId_t event_uart3_ID = NULL;


const osEventFlagsAttr_t event_uart4_Attr = 
{
	.name = "event_uart4",   	
														
};
osEventFlagsId_t event_uart4_ID = NULL;


const osEventFlagsAttr_t event_uart5_Attr = 
{
	.name = "event_uart5",   	
														
};
osEventFlagsId_t event_uart5_ID = NULL;


const osEventFlagsAttr_t event_uart6_Attr = 
{
	.name = "event_uart6",   
														
};
osEventFlagsId_t event_uart6_ID = NULL;



const osEventFlagsAttr_t event_UdpGetData_Attr = 
{
	.name = "event_UdpGetData",   	
																	
};
osEventFlagsId_t event_UdpGetData_ID = NULL;


const osEventFlagsAttr_t event_UdpSendData_Attr = 
{
	.name = "event_UdpSendData",   	
																	
};
osEventFlagsId_t event_UdpSendData_ID = NULL;


CAN_HandleTypeDef   CAN1_Handler;  
CAN_HandleTypeDef   CAN2_Handler;



/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	HAL_SuspendTick();

	osKernelInitialize();                                  

	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr);  

	osKernelStart();
}


void AppTaskStart(void *argument)
{
	const uint16_t usFrequency = 100;
	uint32_t tick;	

	HAL_ResumeTick();
	ConfigStm32HalCan();
	GPIO_LEDInitialization();
//	bsp_InitUart();
	bsp_InitGpio();
	
//	bsp_GpioOn(1);
//	bsp_GpioOn(2);
//	bsp_GpioOn(3);
	
	MX_DMA_Init();
	

	MX_UART5_UART_Init();	
	/**UART DMA config**/
	//HAL_UART_Transmit(&huart5, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);	
	#define UART_RX_MODE_DMA	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart5, g_tBspUart5.pRxBuf, g_tBspUart5.rxSize);
	#else
	HAL_UART_Receive_IT(&husart1, g_tBspUart1.pRxBuf, g_tBspUart1.rxSize);
	#endif
	

	MX_USART2_UART_Init();	
	/**UART DMA config**/
	//HAL_UART_Transmit(&husart2, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&husart2, UART_IT_IDLE);	
	#define UART_RX_MODE_DMA	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#else
	HAL_UART_Receive_IT(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#endif	


	MX_UART4_UART_Init();	
	/**UART DMA config**/
	//HAL_UART_Transmit(&husart2, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);	
	#define UART_RX_MODE_DMA		
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&huart4, g_tBspUart4.pRxBuf, g_tBspUart4.rxSize);
	#else
	HAL_UART_Receive_IT(&husart2, g_tBspUsart2.pRxBuf, g_tBspUsart2.rxSize);
	#endif


	MX_USART3_UART_Init();	
	/**UART DMA config**/
	//HAL_UART_Transmit(&husart3, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&husart3, UART_IT_IDLE);	
	#define UART_RX_MODE_DMA	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart3, g_tBspUsart3.pRxBuf, g_tBspUsart3.rxSize);
	#else
	HAL_UART_Receive_IT(&husart3, g_tBspUsart3.pRxBuf, g_tBspUsart3.rxSize);
	#endif	


	MX_USART1_UART_Init();	
	/**UART DMA config**/
	//HAL_UART_Transmit(&husart3, (uint8_t *)"this is uart1 send data\r\n", 24, 1000);	
	__HAL_UART_ENABLE_IT(&husart1, UART_IT_IDLE);	
	#define UART_RX_MODE_DMA	
	#ifdef UART_RX_MODE_DMA
	HAL_UART_Receive_DMA(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#else
	HAL_UART_Receive_IT(&husart1, g_tBspUsart1.pRxBuf, g_tBspUsart1.rxSize);
	#endif	
	

	
	EventRecorderInitialize(EventRecordAll, 1U);	
	EventRecorderStart();		

	
	msgQueue_Can1ResData = osMessageQueueNew(
							32,  										 			
							sizeof(struct CAN_RESDATA), 	
							&Can1ResData_Attr  			 			
							);								
	msgQueue_PositionData = osMessageQueueNew(12 , sizeof(struct MOTORPOSITION) , &PositionData_Attr);								/*创建电机位置模式信息 消息队列*/
	msgQueue_UnderPositionData = osMessageQueueNew(12 , sizeof(struct MOTORPOSITION) , &UnderPositionData_Attr);			/*创建下壁电机位置模式信息 消息队列*/		
	msgQueue_UdpResData = osMessageQueueNew(12 , sizeof(struct GETUDPDATA) , &UdpResData_Attr);											/*接收UDP数据消息队列*/
	msgQueue_SppedScanning = osMessageQueueNew(12 , sizeof(struct MOTORSPPEDSCANNING) , &SppedScanning_Attr);					/*传递速度模式指令值*/
	msgQueue_UnderSppedScanning = osMessageQueueNew(12 , sizeof(struct MOTORSPPEDSCANNING) , &UnderSppedScanning_Attr);
	msgQueue_UdpSendData = osMessageQueueNew(12 , sizeof(struct SendUDPData) , &UdpSendData_Attr);										/*udp数据消息队列*/							
	msgQueue_MotorData = osMessageQueueNew(12 , sizeof(struct MOTORDATA) , &MotorData_Attr);	

	event_cable_ID 					= osEventFlagsNew(&event_cable_Attr);
//	event_lm1266_ID 				= osEventFlagsNew(&event_lm1266_Attr);	
//	event_sppedzero_ID 			= osEventFlagsNew(&event_sppedzero_Attr);								
//	event_positionarrive_ID = osEventFlagsNew(&event_positionarrive_Attr);					
//	event_positionzero_ID 	= osEventFlagsNew(&event_positionzero_Attr);					
							
	event_closurefinddata_ID = osEventFlagsNew(&event_closurefinddata_Attr);						
	event_belowclosurefinddata_ID = osEventFlagsNew(&event_belowclosurefinddata_Attr);	
	
	event_uart1_ID = osEventFlagsNew(&event_uart1_Attr);
	event_uart2_ID = osEventFlagsNew(&event_uart2_Attr);
	event_uart3_ID = osEventFlagsNew(&event_uart3_Attr);
	event_uart4_ID = osEventFlagsNew(&event_uart4_Attr);
	event_uart5_ID = osEventFlagsNew(&event_uart5_Attr);
	event_uart6_ID = osEventFlagsNew(&event_uart6_Attr);
	
	event_UdpGetData_ID = osEventFlagsNew(&event_UdpGetData_Attr);
	event_UdpSendData_ID = osEventFlagsNew(&event_UdpSendData_Attr);							
							
	if(net_init() != 0)	
	{
	}
	ThreadIdTaskLwIPPro = osThreadNew(AppTaskLwIPPro, NULL, &ThreadLwIPPro_Attr);
	tick += usFrequency + 200;                          
	osDelayUntil(tick);	
								
	ThreadIdTaskEthCheck = osThreadNew(AppTaskEthCheck, NULL, &ThreadEthCheck_Attr); 
	tick += usFrequency + 1000;                          
	osDelayUntil(tick);
						

//	ThreadIdTaskMotorFindZero = osThreadNew(AppTaskMotorFindZero, NULL, &ThreadMotorFindZero_Attr);		
//	tick += usFrequency + 1000;                          
//	osDelayUntil(tick);							

//	ThreadIdTaskUnderMotorFindZero = osThreadNew(AppTaskUnderMotorFindZero, NULL, &ThreadUnderMotorFindZero_Attr);	
	

	ThreadIdTaskUdpNetWork = osThreadNew(AppTaskUdpNetWork, NULL, &ThreadUdpNetWork_Attr);
	tick += usFrequency + 5000;                          
	osDelayUntil(tick);		
	ThreadIdTaskLED = osThreadNew(AppTaskLED, NULL, &ThreadLED_Attr);

	ThreadIdTaskcan1_anwser = osThreadNew(AppTaskCan1_answer, NULL, &ThreadCan1answer_Attr); 	


	tick = osKernelGetTickCount(); 
	while(1)
	{
		tick += usFrequency;                          
		osDelayUntil(tick);
		osThreadExit();
	}
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}
