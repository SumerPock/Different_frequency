
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
	14,																//FilterBank ָ��Ҫ��ʼ����ɸѡ����
	CAN_FILTERMODE_IDMASK,						//FilterMode
	CAN_FILTERSCALE_32BIT,						//FilterScale
	ENABLE,														//FilterActivation
	0																	//SlaveStartFilterBank ��CANʵ��ѡ��������������
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
	/*�����f1����Ҫע���� �� f4 ���ù� Refer to Section 2.3: Memory map for the register boundary addresses. In connectivity line
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
*	�� �� ��: Init_HardCanGpioConfig
*	����˵��:
*	��    ��:  
*	�� �� ֵ: 
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
*	�� �� ��: CAN_Mode_Init
*	����˵��:
*	��    ��:  
*	�� �� ֵ: 
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
*	�� �� ��: CAN_Filter_Config
*	����˵��:
*	��    ��:  
*	�� �� ֵ: 
*********************************************************************************************************
*/
unsigned char CAN_Filter_Config(CAN_HandleTypeDef* canx_handler , CAN_FilterTypeDef* canx_filter)
{
	unsigned char can_filter = 0;
  CAN_FilterTypeDef  sFilterConfig;
  /*����CAN������*/
  sFilterConfig.FilterBank = canx_filter->FilterBank;                     //������0
  sFilterConfig.FilterMode = canx_filter->FilterMode;
  sFilterConfig.FilterScale = canx_filter->FilterScale;
  sFilterConfig.FilterIdHigh = canx_filter->FilterIdHigh;              //32λID
  sFilterConfig.FilterIdLow = canx_filter->FilterIdLow;
  sFilterConfig.FilterMaskIdHigh = canx_filter->FilterMaskIdHigh;          //32λMASK
  sFilterConfig.FilterMaskIdLow = canx_filter->FilterMaskIdLow;
  sFilterConfig.FilterFIFOAssignment = canx_filter->FilterFIFOAssignment;//������0������FIFO0
  sFilterConfig.FilterActivation = canx_filter->FilterActivation;          //�����˲���0
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
*	�� �� ��: CAN_Send_Mesage
*	����˵��:
*	��    ��:  
				can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
				len:���ݳ���(���Ϊ8)				     
				msg:����ָ��,���Ϊ8���ֽ�.
*	�� �� ֵ: /int (*p)[n]
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
	
//	/* �ú������������ڴ洢Tx��Ϣ��TxMailbox���ò���������@arg CAN_Tx_Mailboxes��ֵ */
//	if(HAL_CAN_AddTxMessage(&can_hand, &TxHeader, message, &TxMailbox) != HAL_OK)
//		return 1;
//	/*����������ж�,һ����3��*/
	//	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand) != 2){
	//	}
	
//	while(HAL_CAN_GetTxMailboxesFreeLevel(&can_hand) == 2){
//		/* �ú������������ڴ洢Tx��Ϣ��TxMailbox���ò���������@arg CAN_Tx_Mailboxes��ֵ */
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
	CAN_RxHeaderTypeDef	RxHeader = {0};      //����
	if(hcan->Instance == CAN1){
		HAL_CAN_GetRxMessage(&CAN1_Handler , CAN_FILTER_FIFO0 , &RxHeader, can1resdata.can_resbuf);//��ȡ����'
		can1resdata.resStdID = RxHeader.StdId;
		can1resdata.can_resbuflen = RxHeader.DLC;
		
		/*��CAN���ķ�����Ϣ����*/										
		osMessageQueuePut(msgQueue_Can1ResData,  		/* ��Ϣ���е����� */
											&can1resdata, 				/* ������Ϣ���еı�������Ϣ�� */
											NULL,              			/* ������Ϣ�����ȼ� */
											NULL); 
	}
 	else if(hcan->Instance == CAN2){
		HAL_CAN_GetRxMessage(&CAN2_Handler , CAN_FILTER_FIFO0 , &RxHeader, can2resdata.can_resbuf);//��ȡ����
		can2resdata.resStdID = RxHeader.StdId;
		
//		/*��CAN���ķ�����Ϣ����*/
//		osMessageQueuePut(msgQueue_Can2ResData,  /* ��Ϣ���е����� */
//											&can2resdata, 				/* ������Ϣ���еı�������Ϣ�� */
//											NULL,              		/* ������Ϣ�����ȼ� */
//											NULL               		/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
//											); 		
	}
}

void CAN1_RX0_IRQHandler(void){
  HAL_CAN_IRQHandler(&CAN1_Handler);
}

void CAN2_RX0_IRQHandler(void){
	HAL_CAN_IRQHandler(&CAN2_Handler);
}
