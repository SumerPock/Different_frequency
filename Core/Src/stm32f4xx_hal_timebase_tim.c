
#include "includes.h"



#if 0

TIM_HandleTypeDef   TimHandle = {0};
	

HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{

	RCC_ClkInitTypeDef    clkconfig;
	uint32_t              uwTimclock, uwAPB1Prescaler = 0U;
	uint32_t              uwPrescalerValue = 0U;
	uint32_t              pFLatency;

	
	/* 复位定时器7 */
	TimHandle.Instance = TIM7;
	if(HAL_TIM_Base_DeInit(&TimHandle) != HAL_OK)
	{
		Error_Handler(__FILE__, __LINE__);
	}
	
	/* 设置TIM7的中断优先级并使能 */
	HAL_NVIC_SetPriority(TIM7_IRQn, 5 ,0U);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	/* 使能TIM7时钟 */
	__HAL_RCC_TIM7_CLK_ENABLE();

	/*-----------------------------------------------------------------------
        System Clock source       = PLL (HSE)
        SYSCLK(Hz)                = 400000000 (CPU Clock)
        HCLK(Hz)                  = 200000000 (AXI and AHBs Clock)
        AHB Prescaler             = 2
        D1 APB3 Prescaler         = 2 (APB3 Clock  100MHz)
        D2 APB1 Prescaler         = 2 (APB1 Clock  100MHz)
        D2 APB2 Prescaler         = 2 (APB2 Clock  100MHz)
        D3 APB4 Prescaler         = 2 (APB4 Clock  100MHz)

        因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = APB1 x 2 = 200MHz;
        因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = APB2 x 2 = 200MHz;
        APB4上面的TIMxCLK没有分频，所以就是100MHz;

        APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13, TIM14，LPTIM1
        APB2 定时器有 TIM1, TIM8 , TIM15, TIM16，TIM17

        APB4 定时器有 LPTIM2，LPTIM3，LPTIM4，LPTIM5
	----------------------------------------------------------------------- */
	/* 获取时钟配置 */
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	/* 获取APB1时钟 */
	uwAPB1Prescaler = clkconfig.APB1CLKDivider;

	/* 获得TIM7时钟 */
	if (uwAPB1Prescaler == RCC_HCLK_DIV1) 
	{
		uwTimclock = HAL_RCC_GetPCLK1Freq();
	}
	else
	{
		uwTimclock = 2*HAL_RCC_GetPCLK1Freq();
	}

	/* TIM7分频到1MHz */
	uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

	/* TIM7CLK = uwTimclock / (Period + 1) / (Prescaler + 1) = 1KHz */
	TimHandle.Init.Period = (1000000U / 1000U) - 1U;
	TimHandle.Init.Prescaler = uwPrescalerValue;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&TimHandle) == HAL_OK)
	{
		/* 启动TIM7 */
		return HAL_TIM_Base_Start_IT(&TimHandle);
	}

	/* 返回错误 */
	return HAL_ERROR;
}

/*
*********************************************************************************************************
*	函 数 名: HAL_SuspendTick
*	功能说明: 关闭TIM7
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_SuspendTick(void)
{
	__HAL_TIM_DISABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/*
*********************************************************************************************************
*	函 数 名: HAL_ResumeTick
*	功能说明: 使能TIM7
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_ResumeTick(void)
{
	__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);
}

/*
*********************************************************************************************************
*	函 数 名: TIM7_IRQHandler
*	功能说明: TIM7定时器中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM7_IRQHandler(void)
{
	if((TIM7->SR & TIM_FLAG_UPDATE) != RESET)
	{
		 TIM7->SR = ~ TIM_FLAG_UPDATE;
		 HAL_IncTick();
	}
}

#else

void HAL_Delay(uint32_t Delay)
{
	bsp_DelayMS(Delay);
}

HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
	return HAL_OK;
}

uint32_t HAL_GetTick (void) 
{
	static uint32_t ticks = 0U;
	uint32_t i;

	if (osKernelGetState () == osKernelRunning)
	{
		return ((uint32_t)osKernelGetTickCount ());
	}

	/* 如果RTX5还没有运行，采用下面方式 */
	for (i = (SystemCoreClock >> 14U); i > 0U; i--) 
	{
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	}
	
	return ++ticks;
}
#endif

