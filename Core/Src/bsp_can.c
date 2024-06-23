
#include "bsp.h"

CAN_RxHeaderTypeDef	RxHeader = {0};     

CAN_TxHeaderTypeDef CanTxHeadType = {
	0x001,			
	0x00,			
	CAN_ID_STD,	
	CAN_RTR_DATA,	
	8,				
	DISABLE,		
};

CAN_InitTypeDef Can1_InitTypeDef = {
	CAN_PRE_4,				
	CAN_MODE_NORMAL,	
	CAN_SJW_1TQ,			
	CAN_BS1_16TQ,			
	CAN_BS2_4TQ,			
	
	DISABLE,
	ENABLE,
	DISABLE,
	DISABLE,
	DISABLE,
	DISABLE
};

CAN_InitTypeDef Can2_InitTypeDef = {
	CAN_PRE_4,
	CAN_MODE_NORMAL,
	CAN_SJW_1TQ,
	CAN_BS1_16TQ,
	CAN_BS2_4TQ,
	
	DISABLE,
	ENABLE,
	DISABLE,
	DISABLE,
	DISABLE,
	DISABLE
};



CAN_FilterTypeDef  CAN1_sFilterConfig = {
//	(((unsigned int)0x07FF<<21)&0xffff0000)>>16,			//FilterIdHigh
//	(((unsigned int)0x07FF<<21)|CAN_ID_STD)&0xffff,		//FilterIdLow
	0x0000,
	0x0000,
//	0xFFFF,														//FilterMaskIdHigh
//	0xFFFC,														//FilterMaskIdLow	
	0x0000,
	0x0000,	
	CAN_RX_FIFO0,											//FilterFIFOAssignment
	14,																//FilterBank 指定要初始化的筛选器组
	CAN_FILTERMODE_IDMASK,						//FilterMode
	CAN_FILTERSCALE_32BIT,						//FilterScale
	ENABLE,														//FilterActivation
	0																	//SlaveStartFilterBank 从CAN实例选择启动过滤器组
};


CAN_FilterTypeDef  CAN2_sFilterConfig = {
//	(((unsigned int)0x07FF<<21)&0xffff0000)>>16,			//FilterIdHigh
//	(((unsigned int)0x07FF<<21)|CAN_ID_STD)&0xffff,		//FilterIdLow
	0x0000,
	0x0000,
//	0xFFFF,											//FilterMaskIdHigh
//	0xFFFC,											//FilterMaskIdLow	
	0x0000,
	0x0000,
	CAN_RX_FIFO0,								//FilterFIFOAssignment
//		CAN_RX_FIFO1,								//FilterFIFOAssignment
	14,													//FilterBank
	CAN_FILTERMODE_IDMASK,			//FilterMode
	CAN_FILTERSCALE_32BIT,			//FilterScale
	ENABLE,											//FilterActivation
	14													//SlaveStartFilterBank 
};


void ConfigStm32HalCan(void)
{
	Init_HardCanGpioConfig();
	
	if(CAN_Mode_Init(&Can1_InitTypeDef , &CAN1_Handler , CAN1) != HAL_OK){
		Error_Handler();
	}
	else{
	}
	if(CAN_Filter_Config(&CAN1_Handler , &CAN1_sFilterConfig) != HAL_OK){
		Error_Handler();
	}
	else{
	}
	
	if (HAL_CAN_ConfigFilter(&CAN1_Handler, &CAN1_sFilterConfig) != HAL_OK){
		Error_Handler();	/* Filter configuration Error */
	}else{
	}
	
	if (HAL_CAN_Start(&CAN1_Handler) != HAL_OK){
		Error_Handler();	/* Start Error */
	}
	else{
	}
	/* Activate CAN RX notification */
//	if (HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
	if(HAL_CAN_ActivateNotification(&CAN1_Handler, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		/* Notification Error */
		Error_Handler();
	}
	else{
	}
	
	
	/*can2*/
	if(CAN_Mode_Init(&Can2_InitTypeDef , &CAN2_Handler , CAN2) != HAL_OK){
		Error_Handler();
	}	
	/*这点在f1上需要注意下 ， f4 不用管 Refer to Section 2.3: Memory map for the register boundary addresses. In connectivity line
	devices, the registers from offset 0x200 to 31C are present only in CAN1. !!!!!!!*/
	if(CAN_Filter_Config(&CAN2_Handler , &CAN1_sFilterConfig) != HAL_OK){
		Error_Handler();
	}
	else{
	}	
	if (HAL_CAN_ConfigFilter(&CAN2_Handler, &CAN1_sFilterConfig) != HAL_OK){
		Error_Handler();
	}
	else{
	}
	if (HAL_CAN_Start(&CAN2_Handler) != HAL_OK){
		Error_Handler();
	}
	else{
	}
//	if(HAL_CAN_ActivateNotification(&CAN2_Handler, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
	if(HAL_CAN_ActivateNotification(&CAN2_Handler, CAN_IT_RX_FIFO0_MSG_PENDING|CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK){
		/* Notification Error */
		Error_Handler();
	}
	else{
	}

}

/*
*********************************************************************************************************
*	函 数 名: Init_HardCanGpioConfig
*	功能说明:
*	形    参:  
*	返 回 值: 
*********************************************************************************************************
*/
void Init_HardCanGpioConfig(void)
{
	/* CAN1 interrupt Init */
	GPIO_InitTypeDef GPIO_Initure = {0};
	
	__HAL_RCC_CAN1_CLK_ENABLE();  
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_Initure.Pin = GPIO_PIN_11 | GPIO_PIN_12;   			
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;     
	GPIO_Initure.Pull = GPIO_NOPULL;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 
	GPIO_Initure.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);  
	        	
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0 , 7);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
		
	/* CAN2 interrupt Init */
	__HAL_RCC_CAN2_CLK_ENABLE();   
	__HAL_RCC_GPIOB_CLK_ENABLE();		

	GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_5;   			
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;     
	GPIO_Initure.Pull = GPIO_NOPULL;  
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  
	GPIO_Initure.Alternate = GPIO_AF9_CAN2;  
	HAL_GPIO_Init(GPIOB , &GPIO_Initure); 
	 
	HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0 , 8);
	HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
}



/*
*********************************************************************************************************
*	函 数 名: CAN_Mode_Init
*	功能说明:
*	形    参:  
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char CAN_Mode_Init(CAN_InitTypeDef* can_Init , CAN_HandleTypeDef* canx_handler , CAN_TypeDef* CanChanNum)
{
	unsigned char can_lnit = 0;
	CAN_InitTypeDef	CAN_InitConf;
	canx_handler->Instance = (CAN_TypeDef *)CanChanNum;
	canx_handler->Init = CAN_InitConf;
	canx_handler->Init.Prescaler = can_Init->Prescaler;	
	canx_handler->Init.Mode = can_Init->Mode;
	canx_handler->Init.SyncJumpWidth = can_Init->SyncJumpWidth;		
	canx_handler->Init.TimeSeg1 =  can_Init->TimeSeg1;	
	canx_handler->Init.TimeSeg2 =  can_Init->TimeSeg2;	
	canx_handler->Init.TimeTriggeredMode = can_Init->TimeTriggeredMode;	
	canx_handler->Init.AutoBusOff = can_Init->AutoBusOff;
	canx_handler->Init.AutoWakeUp = can_Init->AutoWakeUp;	
	canx_handler->Init.AutoRetransmission = can_Init->AutoRetransmission;
	canx_handler->Init.ReceiveFifoLocked = can_Init->ReceiveFifoLocked;
	canx_handler->Init.TransmitFifoPriority = can_Init->TransmitFifoPriority;
  if(HAL_CAN_Init(canx_handler) != HAL_OK){
		can_lnit = HAL_ERROR;
	}
	else{
		can_lnit = HAL_OK;
	}
	return can_lnit;
}

/*
*********************************************************************************************************
*	函 数 名: CAN_Filter_Config
*	功能说明:
*	形    参:  
*	返 回 值: 
*********************************************************************************************************
*/
unsigned char CAN_Filter_Config(CAN_HandleTypeDef* canx_handler , CAN_FilterTypeDef* canx_filter)
{
	unsigned char can_filter = 0;
  CAN_FilterTypeDef  sFilterConfig;
  /*配置CAN过滤器*/
  sFilterConfig.FilterBank = canx_filter->FilterBank;                     //过滤器0
  sFilterConfig.FilterMode = canx_filter->FilterMode;
  sFilterConfig.FilterScale = canx_filter->FilterScale;
  sFilterConfig.FilterIdHigh = canx_filter->FilterIdHigh;              //32位ID
  sFilterConfig.FilterIdLow = canx_filter->FilterIdLow;
  sFilterConfig.FilterMaskIdHigh = canx_filter->FilterMaskIdHigh;          //32位MASK
  sFilterConfig.FilterMaskIdLow = canx_filter->FilterMaskIdLow;
  sFilterConfig.FilterFIFOAssignment = canx_filter->FilterFIFOAssignment;//过滤器0关联到FIFO0
  sFilterConfig.FilterActivation = canx_filter->FilterActivation;          //激活滤波器0
  sFilterConfig.SlaveStartFilterBank = canx_filter->SlaveStartFilterBank;
	if(HAL_CAN_ConfigFilter(canx_handler, &sFilterConfig) != HAL_OK){
		can_filter = HAL_ERROR;
	}
	else{
		can_filter = HAL_OK;
	}
	return can_filter;
}   

/*
*********************************************************************************************************
*	函 数 名: CAN_Send_Mesage
*	功能说明:
*	形    参:  
				can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
				len:数据长度(最大为8)				     
				msg:数据指针,最大为8个字节.
*	返 回 值: /int (*p)[n]
*********************************************************************************************************
*/
unsigned char CAN_Send_Mesage(CAN_HandleTypeDef can_hand , CAN_TxHeaderTypeDef* cse ,unsigned char (*ucp)[8])
{	
	__IO unsigned int p=0;
	unsigned char stacan = 0;
	CAN_TxHeaderTypeDef	TxHeader = {0};//send
	uint32_t TxMailbox;
	unsigned char message[8];
	TxHeader.StdId=cse->StdId;    	
	TxHeader.ExtId=cse->ExtId;       
	TxHeader.IDE=cse->IDE;  
	TxHeader.RTR=cse->RTR; 
	TxHeader.DLC=cse->DLC; 
	TxHeader.TransmitGlobalTime = cse->TransmitGlobalTime;	
	for(unsigned char i = 0 ; i < cse->DLC ; i++){
		*(message + i) = *(*ucp + i) ;	
	}
	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand) == 0);
	/* Start the Transmission process */
	if (HAL_CAN_AddTxMessage(&can_hand, &TxHeader, message, &TxMailbox) != HAL_OK){
		/* Transmission request Error */
		Error_Handler();
		stacan = 1;
	}else{
		stacan = 0;
	}
	return stacan;
	
//	/* 该函数将返回用于存储Tx消息的TxMailbox。该参数可以是@arg CAN_Tx_Mailboxes的值 */
//	if(HAL_CAN_AddTxMessage(&can_hand, &TxHeader, message, &TxMailbox) != HAL_OK)
//		return 1;
//	/*对邮箱进行判断,一共有3个*/
	//	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand) != 2){
	//	}
	
//	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand) == 2){
//		/* 该函数将返回用于存储Tx消息的TxMailbox。该参数可以是@arg CAN_Tx_Mailboxes的值 */
//		HAL_CAN_AddTxMessage(&can_hand, &TxHeader, message, &TxMailbox);
//	}
	
//	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand)==0);
//	if(HAL_CAN_AddTxMessage(&can_hand, &TxHeader, message, &TxMailbox) != HAL_OK)
//	{
//		return 0;
//	}
//		return 1;
   
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	struct CAN_RESDATA can1resdata = {0};
	struct CAN_RESDATA can2resdata = {0};
	CAN_RxHeaderTypeDef	RxHeader = {0};      //接收
	if(hcan->Instance == CAN1){
		HAL_CAN_GetRxMessage(&CAN1_Handler , CAN_FILTER_FIFO0 , &RxHeader, can1resdata.can_resbuf);//获取数据'
		can1resdata.resStdID = RxHeader.StdId;
		can1resdata.can_resbuflen = RxHeader.DLC;
		
		/*将CAN报文放入消息队列*/										
		osMessageQueuePut(msgQueue_Can1ResData,  		/* 消息队列的名字 */
											&can1resdata, 				/* 放入消息队列的变量（消息） */
											NULL,              			/* 放入消息的优先级 */
											NULL); 
	}
 	else if(hcan->Instance == CAN2){
		HAL_CAN_GetRxMessage(&CAN2_Handler , CAN_FILTER_FIFO0 , &RxHeader, can2resdata.can_resbuf);//获取数据
		can2resdata.resStdID = RxHeader.StdId;
		
//		/*将CAN报文放入消息队列*/
//		osMessageQueuePut(msgQueue_Can2ResData,  /* 消息队列的名字 */
//											&can2resdata, 				/* 放入消息队列的变量（消息） */
//											NULL,              		/* 放入消息的优先级 */
//											NULL               		/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
//											); 		
	}
}

void CAN1_RX0_IRQHandler(void){
  HAL_CAN_IRQHandler(&CAN1_Handler);
}

void CAN2_RX0_IRQHandler(void){
	HAL_CAN_IRQHandler(&CAN2_Handler);
}
