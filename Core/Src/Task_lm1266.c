#include "bsp.h"

//void AppTaskIM1266Num1(void *argument)
//{
//	osStatus_t os_Status; 
//	/*LM1266���������ݽṹ��*/
////	struct LM1266DATA GetLm1266Data = {0 , 40 , 0 , {0} , {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xc4 , 0x1a} , 8 , 0 , 0 , 0};
////	struct Lm1266Coll GetLm1266Coll = {0,{0}};
//	while(1)
//	{	
//		/*�߳�ͬ��*/
//		os_Status = osEventFlagsWait(event_uart1_ID , 0x00000001U , osFlagsWaitAll , osWaitForever);
//		if(os_Status == 0x01)
//		{
////			for(unsigned short usi = 0; usi < g_tBspUsart1.rxLen ; usi++)
////			{
////				printf("g_tBspUsart1.pRxBuf[%d] = %x \r\n", usi , g_tBspUsart1.pRxBuf[usi]);
////			}			
//		}
//	}
//}


void AppTaskIM1266(void *argument)
{
//	struct UARTState_Machine uartstate_machine = {0,0,0,0,0,0};/*����״̬��*/
//	const uint16_t usFrequency = 100; /* �ӳ����� */
//	uint32_t tick;
	osStatus_t os_Status; 
	/*LM1266���������ݽṹ��*/
	struct LM1266DATA GetLm1266Data = {0 , 40 , 0 , {0} , {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xc4 , 0x1a} , 8 , 0 , 0 , 0};
	while(1)
	{
//			os_Status = osMessageQueueGet(msgQueue_lm1266Data, 	/* ��Ϣ����ID */
//																		&GetLm1266Data,     	/* �����Ϣ���ڴ� */
//																		NULL,				 					/* ��Ϣ���ȼ� */
//																		osWaitForever);    		/* ��ʱʱ�䣨һֱ�ȴ��� */

		
//				os_Status = osMessageQueueGet(msgQueue_UdpResData,  /* ��Ϣ����ID */
//										&GetUdpData,          							/* �����Ϣ���ڴ� */
//										NULL,						 										/* ��Ϣ���ȼ� */
//										osWaitForever);    									/* ��ʱʱ�䣨һֱ�ȴ��� */

		
//			if(osOK == os_Status)
//			{
//				for(unsigned short usi = 0; usi < GetLm1266Data.lm1266_real_len ; usi++)
//				{
//					printf("GetLm1266Data.lm1266_resdata[%d] = %d \r\n", usi , GetLm1266Data.lm1266_resdata[usi]);
//				}
//				
//			}	
	}
}

//void AppTaskIM1266Num2(void *argument)
//{
////	struct UARTState_Machine uartstate_machine = {0,0,0,0,0,0};/*����״̬��*/
//	const uint16_t usFrequency = 100; /* �ӳ����� */
//	uint32_t tick;
//	osStatus_t os_Status; 
////	struct Lm1266Coll GetLm1266Coll = {0,{0}};
//	/*LM1266���������ݽṹ��*/
//	//struct LM1266DATA GetLm1266Data = {0 , 40 , 0 , {0} , {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xc4 , 0x1a} , 8 , 0 , 0 , 0};
//	while(1)
//	{
//		/*�߳�ͬ��*/
//		os_Status = osEventFlagsWait(event_uart3_ID , 0x00000001U , osFlagsWaitAll , osWaitForever);
//		if(os_Status == 0x01)
//		{
////			for(unsigned short usi = 0; usi < g_tBspUsart3.rxLen ; usi++)
////			{
////				printf("g_tBspUsart3.pRxBuf[%d] = %x \r\n", usi , g_tBspUsart3.pRxBuf[usi]);
////			}			
//		}
//	}
//}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskIM1266
*	����˵��: IM1266�������
*	��    ��: ��
*	�� �� ֵ: ��
* �� �� ��: 
*********************************************************************************************************
*/
//void AppTaskIM1266(void *argument)
//{
//	struct UARTState_Machine uartstate_machine = {0,0,0,0,0,0};/*����״̬��*/
//	const uint16_t usFrequency = 100; /* �ӳ����� */
//	uint32_t tick;
//	unsigned char cilck[8] = {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xC4 , 0x1A};
//	/* ��ȡ��ǰʱ�� */
//	tick = osKernelGetTickCount(); 
//	struct LM1266 mylm1266 = {0};
//	while(1){
//		/* ����ӳ� */
//		comSendBuf(COM1, cilck, 8);
//		if(comGetChar(COM1, &uartstate_machine.read))
//		{   
////			Uart_State_Machine(&uartstate_machine , mylm1266);
//		}	
//		if(comGetChar(COM4, &uartstate_machine.read))
//		{   
//			//Uart_State_Machine(&uartstate_machine , mylm1266);
////			comSendBuf(COM4, "this is uart4", 12);		
////			comSendBuf(COM4, "this is uart4", 12);		
//		}	
//				
//		if(osEventFlagsGet(event_lm1266_ID) == 0x01U){
//				osEventFlagsClear(event_lm1266_ID , 0x01U);
////				comSendBuf(COM5,"thi2 is ok\r\n", 10);
////				comSendBuf(COM2,"thi1 is ok\r\n", 10);
//		}
//		tick += usFrequency;                          
//		osDelayUntil(tick);
//	}
//}

