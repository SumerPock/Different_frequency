/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

///**
//  * @brief This function handles System service call via SWI instruction.
//  */
//void SVC_Handler(void)
//{
//  /* USER CODE BEGIN SVCall_IRQn 0 */

//  /* USER CODE END SVCall_IRQn 0 */
//  /* USER CODE BEGIN SVCall_IRQn 1 */

//  /* USER CODE END SVCall_IRQn 1 */
//}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

///**
//  * @brief This function handles Pendable request for system service.
//  */
//void PendSV_Handler(void)
//{
//  /* USER CODE BEGIN PendSV_IRQn 0 */

//  /* USER CODE END PendSV_IRQn 0 */
//  /* USER CODE BEGIN PendSV_IRQn 1 */

//  /* USER CODE END PendSV_IRQn 1 */
//}

///**
//  * @brief This function handles System tick timer.
//  */
//void SysTick_Handler(void)
//{
//  /* USER CODE BEGIN SysTick_IRQn 0 */

//  /* USER CODE END SysTick_IRQn 0 */
//  HAL_IncTick();
//  /* USER CODE BEGIN SysTick_IRQn 1 */

//  /* USER CODE END SysTick_IRQn 1 */
//}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart1);
  HAL_UART_IRQHandler(&husart1);
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART2_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart2);
  HAL_UART_IRQHandler(&husart2);
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART3_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart3);
  HAL_UART_IRQHandler(&husart3);
}

/*
*********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void UART4_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&huart4);
  HAL_UART_IRQHandler(&huart4);
}


/*
*********************************************************************************************************
*	函 数 名: UART4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void UART5_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&huart5);
  HAL_UART_IRQHandler(&huart5);
}


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void USART6_IRQHandler(void)
{
  BSP_UART_IDLE_Handler(&husart6);
  HAL_UART_IRQHandler(&husart6);
}


/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream2_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream7_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream5_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream6_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream3_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream2_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
}


/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart5_rx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream4_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA1_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_uart5_tx);
}


/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream6_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream1_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   					
*********************************************************************************************************
*/
void DMA2_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
}
/*
*********************************************************************************************************
*	函 数 名: DMA2_Stream7_IRQHandler
*	功能说明: 
*	形    参: 无
*	返 回 值: 无
* 优 先 级:  
stm32f4xx_it.c 调用
*********************************************************************************************************
*/
void BSP_UART_IDLE_Handler(UART_HandleTypeDef *huart)
{
	unsigned int rxCnt = 0;
	if(__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET)
	{
		
		if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
		{	
			__HAL_UART_CLEAR_IDLEFLAG(huart);	
			unsigned char dmaBit = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
			if(dmaBit)
			{		
				rxCnt = __HAL_DMA_GET_COUNTER(huart->hdmarx);
			}
			else
			{
				rxCnt = huart->RxXferCount;
			}		
			HAL_UART_AbortReceive(huart);
			
			if(huart->Instance == USART1)
			{
				g_tBspUsart1.rxLen  = g_tBspUsart1.rxSize - rxCnt;
				g_tBspUsart1.rxFlag = 1;
				osEventFlagsSet(event_uart1_ID , 0x01U << 0);
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart1 , g_tBspUsart1.pRxBuf , g_tBspUsart1.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart1 , g_tBspUsart1.pRxBuf , g_tBspUsart1.rxSize);
				}
			}
			
			else if(huart->Instance == USART2)
			{
				g_tBspUsart2.rxLen  = g_tBspUsart2.rxSize - rxCnt;
				g_tBspUsart2.rxFlag = 1;
				osEventFlagsSet(event_uart2_ID , 0x01U << 0);
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart2 , g_tBspUsart2.pRxBuf , g_tBspUsart2.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart2 , g_tBspUsart2.pRxBuf , g_tBspUsart2.rxSize);
				}
			}
			
			else if(huart->Instance == USART3)
			{
				g_tBspUsart3.rxLen  = g_tBspUsart3.rxSize - rxCnt;
				g_tBspUsart3.rxFlag = 1;
				osEventFlagsSet(event_uart3_ID , 0x01U << 0);  
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart3 , g_tBspUsart3.pRxBuf , g_tBspUsart3.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart3 , g_tBspUsart3.pRxBuf , g_tBspUsart3.rxSize);
				}
			}			
			
			else if(huart->Instance == UART4)
			{
				g_tBspUart4.rxLen  = g_tBspUart4.rxSize - rxCnt;
				g_tBspUart4.rxFlag = 1;
				osEventFlagsSet(event_uart4_ID , 0x01U << 0);			
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&huart4 , g_tBspUart4.pRxBuf , g_tBspUart4.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&huart4 , g_tBspUart4.pRxBuf , g_tBspUart4.rxSize);
				}
			}
			
			else if(huart->Instance == UART5)
			{
				g_tBspUart5.rxLen  = g_tBspUart5.rxSize - rxCnt;
				g_tBspUart5.rxFlag = 1;
				osEventFlagsSet(event_uart5_ID , 0x01U << 0);
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&huart5 , g_tBspUart5.pRxBuf , g_tBspUart5.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&huart5 , g_tBspUart5.pRxBuf , g_tBspUart5.rxSize);
				}
			}
			
			else if(huart->Instance == USART6)
			{
				g_tBspUsart6.rxLen  = g_tBspUsart6.rxSize - rxCnt;
				g_tBspUsart6.rxFlag = 1;
				osEventFlagsSet(event_uart6_ID , 0x01U << 0);	
				if(dmaBit)
				{
					HAL_UART_Receive_DMA(&husart6 , g_tBspUsart6.pRxBuf , g_tBspUsart6.rxSize);
				}
				else
				{
					HAL_UART_Receive_IT(&husart6 , g_tBspUsart6.pRxBuf , g_tBspUsart6.rxSize);
				}
			}	
		}
	}
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
