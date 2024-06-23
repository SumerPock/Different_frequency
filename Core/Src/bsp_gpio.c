#include "bsp.h"

/* 控制端口对应的RCC时钟 */
#define GPIO_GPIO_CLK_ALLENABLE() {	  \
		__HAL_RCC_GPIOC_CLK_ENABLE(); \
		__HAL_RCC_GPIOA_CLK_ENABLE(); \
	};
/*创建GPIO回调控制函数*/
//1 . 定义俩个函数分别实现GPIO拉高拉低功能


/*SIG P1*/
#define GPIO_PORT_SIG1  GPIOA
#define GPIO_PIN_SIG1		GPIO_PIN_8
/*SIG P2*/
#define GPIO_PORT_SIG2  GPIOC
#define GPIO_PIN_SIG2		GPIO_PIN_8
/*SIG P3*/
#define GPIO_PORT_SIG3  GPIOC
#define GPIO_PIN_SIG3		GPIO_PIN_9
	
///*SIG P4*/
//#define GPIO_PORT_SIG4  	GPIOC
//#define GPIO_PIN_SIG4		GPIO_PIN_8
///*SIG P5*/
//#define GPIO_PORT_SIG5  	GPIOA
//#define GPIO_PIN_SIG5		GPIO_PIN_8
///*SIG_CTRL3*/
//#define GPIO_PORT_SIG_CTRL3   GPIOE
//#define GPIO_PIN_SIG_CTRL3	  GPIO_PIN_4
///*SIG_POW*/
//#define GPIO_PORT_SIG_POW   GPIOE
//#define GPIO_PIN_SIG_POW	GPIO_PIN_10

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitGpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/* 打开GPIO时钟 */
	GPIO_GPIO_CLK_ALLENABLE();

	/*
	配置所有的LED指示灯GPIO为推挽输出模式
	由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
	这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
//	bsp_GpioOff(1);
//	bsp_GpioOff(2);
//	bsp_GpioOff(3);
//	bsp_GpioOff(4);
//	bsp_GpioOff(5);
//	bsp_GpioOff(6);
	
	/* 配置GPIO */
	GPIO_InitStruct.Pin = GPIO_PIN_SIG1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   		/* 设置推挽输出 */
	GPIO_InitStruct.Pull = GPIO_PULLUP;                 /* 上下拉电阻不使能 */
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		/* GPIO速度等级 */
	HAL_GPIO_Init(GPIO_PORT_SIG1 , &GPIO_InitStruct);


	GPIO_InitStruct.Pin = GPIO_PIN_SIG2 | GPIO_PIN_SIG3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   		
	GPIO_InitStruct.Pull = GPIO_PULLUP;               	
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  		
	HAL_GPIO_Init(GPIO_PORT_SIG2 , &GPIO_InitStruct);
}


void GPIO_LEDInitialization(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_10|GPIO_PIN_11;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void GPIO_OUTPUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin 		= GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
}


void bsp_GpioOn(unsigned char _no)
{
	_no--;
	if (_no == 0)
	{
		GPIO_PORT_SIG1->BSRR = GPIO_PIN_SIG1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_SIG2->BSRR = GPIO_PIN_SIG2;
	}	
	else if (_no == 2)
	{
		GPIO_PORT_SIG3->BSRR = GPIO_PIN_SIG3;
	}
	
//	else if (_no == 3)
//	{
//		GPIO_PORT_SIG4->BSRR = GPIO_PIN_SIG4;
//	}
//	else if (_no == 4)
//	{
//		GPIO_PORT_SIG5->BSRR = GPIO_PIN_SIG5;
//	}
//	else if (_no == 5)
//	{
//		GPIO_PORT_SIG_CTRL3->BSRR = GPIO_PIN_SIG_CTRL3;
//	}
//	else if(_no == 6)
//	{
//		GPIO_PORT_SIG_POW->BSRR = GPIO_PIN_SIG_POW;
//	}
}


void bsp_GpioOff(unsigned char _no)
{
	_no--;
	if (_no == 0)
	{
		GPIO_PORT_SIG1->BSRR = (uint32_t)GPIO_PIN_SIG1 << 16U;
	}
	else if (_no == 1)
	{
		GPIO_PORT_SIG2->BSRR = (uint32_t)GPIO_PIN_SIG2 << 16U;
	}	
	else if (_no == 2)
	{
		GPIO_PORT_SIG3->BSRR = (uint32_t)GPIO_PIN_SIG3 << 16U;
	}
//	else if (_no == 3)
//	{
//		GPIO_PORT_SIG4->BSRR = (uint32_t)GPIO_PIN_SIG4 << 16U;
//	}
//	else if (_no == 4)
//	{
//		GPIO_PORT_SIG5->BSRR = (uint32_t)GPIO_PIN_SIG5 << 16U;
//	}
//	else if (_no == 5)
//	{
//		GPIO_PORT_SIG_CTRL3->BSRR = (uint32_t)GPIO_PIN_SIG_CTRL3 << 16U;
//	}	
//	else if(_no == 6)
//	{
//		GPIO_PORT_SIG_POW->BSRR = (uint32_t)GPIO_PIN_SIG_POW << 16U;
//	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_GpioToggle(unsigned char _no)
{
	if (_no == 1)
	{
		GPIO_PORT_SIG1->ODR ^= GPIO_PIN_SIG1;
	}
	else if (_no == 2)
	{
		GPIO_PORT_SIG2->ODR ^= GPIO_PIN_SIG2;
	}	
	else if (_no == 3)
	{
		GPIO_PORT_SIG3->ODR ^= GPIO_PIN_SIG3;
	}
//	else if (_no == 4)
//	{
//		GPIO_PORT_SIG4->ODR ^= GPIO_PIN_SIG4;
//	}
//	else if (_no == 5)
//	{
//		GPIO_PORT_SIG5->ODR ^= GPIO_PIN_SIG5;
//	}
//	else if (_no == 6)
//	{
//		GPIO_PORT_SIG_CTRL3->ODR ^= GPIO_PIN_SIG_CTRL3;
//	}
//	else if (_no == 7)
//	{
//		GPIO_PORT_SIG_POW->ODR ^= GPIO_PIN_SIG_POW;
//	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
*********************************************************************************************************
*/
unsigned char bsp_IsGpioOn(unsigned char _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_SIG1->ODR & GPIO_PIN_SIG1) == 1)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_SIG2->ODR & GPIO_PIN_SIG2) == 1)
		{
			return 1;
		}
		return 0;
	}	
	else if (_no == 3)
	{
		if ((GPIO_PORT_SIG3->ODR & GPIO_PIN_SIG3) == 1)
		{
			return 1;
		}
		return 0;
	}
//	else if (_no == 4)
//	{
//		if ((GPIO_PORT_SIG4->ODR & GPIO_PIN_SIG4) == 1)
//		{
//			return 1;
//		}
//		return 0;
//	}
//	else if (_no == 5)
//	{
//		if ((GPIO_PORT_SIG5->ODR & GPIO_PIN_SIG5) == 1)
//		{
//			return 1;
//		}
//		return 0;
//	}
//	else if (_no == 6)
//	{
//		if ((GPIO_PORT_SIG_CTRL3->ODR & GPIO_PIN_SIG_CTRL3) == 1)
//		{
//			return 1;
//		}
//		return 0;
//	}
//	else if (_no == 7)
//	{
//		if ((GPIO_PORT_SIG_CTRL3->ODR & GPIO_PIN_SIG_CTRL3) == 1)
//		{
//			return 1;
//		}
//		return 0;
//	}
//	else if (_no == 6)
//	{
//		if ((GPIO_PORT_SIG_POW->ODR & GPIO_PIN_SIG_POW) == 1)
//		{
//			return 1;
//		}
//		return 0;
//	}	
	return 0;
}




