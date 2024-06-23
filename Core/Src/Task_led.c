#include "bsp.h"


void copyArray(unsigned char source[], unsigned char destination[], unsigned char length) 
{
    memcpy(destination, source, length * sizeof(int));
}

void AppTaskGroce(void *argument)
{
	osStatus_t os_Status; 
	uint16_t usFrequency = 50; /* �ӳ����� */
	uint32_t tick;
	/* ��ȡ��ǰʱ�� */
	tick = osKernelGetTickCount();	
	while(1)
	{
		printf("this is AppTaskGroce");
		
		/* ����ӳ� */
		tick += usFrequency;                          
		osDelayUntil(tick);	
	}
}


/*1.���㷽ʽ*/
unsigned char Zerofindway[8] = {0x40, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*2.λ��ָ��*/
unsigned char PositionCommand[8] = {0x40, 0x62, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*3.ʵ��λ��*/
unsigned char PositionReal[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*4.����ģʽ*/
unsigned char OperateMode[8] = {0x40, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*5.�������ѯ*/
unsigned char ErrorCode[8] = {0x40, 0x3F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*6.�ٶ�ָ��*/
unsigned char SppedCommand[8] = {0x40, 0x6B, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*7.ʵ���ٶ�*/		
unsigned char SppedReal[8] = {0x40, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*8.״̬��*/
unsigned char StateWord[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*S1-S17���*/
unsigned char ClickS1_S17[8] = {0x40, 0xFD, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
/*LM1266���������ݽṹ��*/
struct LM1266DATA Lm1266Data_12v = {0 , 40 , 0 , {0} , {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xc4 , 0x1a} , 8 , 0 , 0 , 0};
struct LM1266DATA Lm1266Data_48v = {0 , 40 , 0 , {0} , {0x01 , 0x03 , 0x00 , 0x48 , 0x00 , 0x08 , 0xc4 , 0x1a} , 8 , 0 , 0 , 0};	
	
void AppTaskLED(void *argument)
{
	osStatus_t os_Status1; 
	osStatus_t os_Status2; 
	osStatus_t os_Status3; 
	osStatus_t os_Status4; 
	osStatus_t os_Status5;
	
	osStatus_t os_Status6;//�ϱ۵�����ݲ�ѯ
	osStatus_t os_Status7;//�±ڵ�����ݲ�ѯ
	
	osStatus_t os_StatusQueue; 
	uint16_t usFrequency = 20; /* �ӳ����� */
	uint32_t tick;
	/* ��ȡ��ǰʱ�� */
	tick = osKernelGetTickCount();
	unsigned char ucloop[2] = {0};			/*���1��2���ݲ�ѯ״̬��*/
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	unsigned char timerloop = 0;
	while(1)
	{
		os_Status3 = osEventFlagsGet(event_uart3_ID);	//LM1266���ݽ���
		os_Status4 = osEventFlagsGet(event_uart4_ID); //DYT���ݽ���
		os_Status5 = osEventFlagsGet(event_uart5_ID);	//��Դ���ݽ���
		os_Status2 = osEventFlagsGet(event_uart2_ID);	//��Դ���ݽ���	
		
		os_Status6 = osEventFlagsGet(event_closurefinddata_ID);				//�ϱڵ����ѯֹͣ
		os_Status7 = osEventFlagsGet(event_belowclosurefinddata_ID);	//�±ڵ����ѯֹͣ
	
		
		if(timerloop >= 20)
		{
			timerloop = 0;
			Lm1266Data_12v.lm1266_senddata[0] = 0x01;
			Lm1266Data_12v.lm1266_senddata[1] = 0x03;
			Lm1266Data_12v.lm1266_senddata[2] = 0x00;
			Lm1266Data_12v.lm1266_senddata[3] = 0x48;
			Lm1266Data_12v.lm1266_senddata[4] = 0x00;
			Lm1266Data_12v.lm1266_senddata[5] = 0x08;
			Lm1266Data_12v.lm1266_senddata[6] = 0xc4;
			Lm1266Data_12v.lm1266_senddata[7] = 0x1a;			
			BSP_UART_Transmit_DMA(&husart1, 	Lm1266Data_48v.lm1266_senddata , 8);
			BSP_UART_Transmit_DMA(&husart3, 	Lm1266Data_12v.lm1266_senddata , 8);
			if(os_Status2 == 1)
			{
				unsigned char buffer[] = {0x55, 0xAA, //����֡ͷ
																	0x0D,	//���ݳ���
																	0x04, //ID
																	0x01,	//������ID
																	0x00, 0x00, 0x00, 0x00,//�����ͻ��������λ
																	0x00,	//�����͸���
																	0x00,	//����״̬			
																	0x00,	//���״̬
																	0x00,	//�������
																	};
				g_tBspUsart2.rxFlag = 0;
				osEventFlagsClear(event_uart2_ID, 0x00000000U);
				if(g_tBspUsart2.pRxBuf[0] == 0x55 && g_tBspUsart2.pRxBuf[1] == 0x0C && g_tBspUart5.pRxBuf[3] == 0x81)
				{
					//3 4 5 6������ 
					buffer[5] = g_tBspUsart2.pRxBuf[4];
					buffer[6] = g_tBspUsart2.pRxBuf[5];
					buffer[7] = g_tBspUsart2.pRxBuf[6];
					buffer[8] = g_tBspUsart2.pRxBuf[7];
					//8  �����͸���
					buffer[9] = g_tBspUsart2.pRxBuf[8];
					//9  ����״̬
					buffer[10] = g_tBspUsart2.pRxBuf[9];				
					//10  ���״̬
					buffer[11] = g_tBspUsart2.pRxBuf[10];
					//11 �������
					buffer[12] = g_tBspUsart2.pRxBuf[11];
					udp_echoclient_send(buffer , 13);
				}
			}
			if(os_Status5 == 1)
			{
				unsigned char buffer[] = {0x55, 0xAA, //����֡ͷ
														0x0D,	//���ݳ���
														0x04, //ID
														0x02,	//������ID
														0x00, 0x00, 0x00, 0x00,//�����ͻ��������λ
														0x00,	//�����͸���
														0x00,	//����״̬			
														0x00,	//���״̬
														0x00,	//�������
														};
				g_tBspUart5.rxFlag = 0;
				osEventFlagsClear(event_uart5_ID, 0x00000000U);
				if(g_tBspUart5.pRxBuf[0] == 0x55 && g_tBspUart5.pRxBuf[1] == 0x0C && g_tBspUart5.pRxBuf[3] == 0x81)
				{
					//3 4 5 6������ 
					buffer[5] = g_tBspUart5.pRxBuf[4];
					buffer[6] = g_tBspUart5.pRxBuf[5];
					buffer[7] = g_tBspUart5.pRxBuf[6];
					buffer[8] = g_tBspUart5.pRxBuf[7];
					//8  �����͸���
					buffer[9] = g_tBspUart5.pRxBuf[8];
					//9  ����״̬
					buffer[10] = g_tBspUart5.pRxBuf[9];				
					//10  ���״̬
					buffer[11] = g_tBspUart5.pRxBuf[10];
					//11 �������
					buffer[12] = g_tBspUart5.pRxBuf[11];
					udp_echoclient_send(buffer , 13);
				}
			}
			if(os_Status3 == 1)
			{
				g_tBspUsart3.rxFlag = 0;
				osEventFlagsClear(event_uart3_ID, 0x00000001U);
				if(g_tBspUsart3.pRxBuf[0] == 0x01 && g_tBspUsart3.pRxBuf[2] == 0x20)
				{
									copyArray(g_tBspUsart3.pRxBuf , Lm1266Data_12v.lm1266_resdata ,g_tBspUsart3.rxLen);
					//printf("this is lm1266");
					
					Lm1266Data_12v.lm1266_voltage = Lm1266Data_12v.lm1266_resdata[3] << 24 | \
																	Lm1266Data_12v.lm1266_resdata[4] << 16 | \
																	Lm1266Data_12v.lm1266_resdata[5] << 8  | \
																	Lm1266Data_12v.lm1266_resdata[6];
					//����Ӳ�����ǻ�⵽10v���µĽ�����ѹ����������� 0000
					if(Lm1266Data_12v.lm1266_voltage <0x186A0)
					{
						Lm1266Data_12v.lm1266_voltage = 0;
					}
					unsigned char udpsendlm1266[16] = {0x55, 0xAA, 0x10, 0x03,
																						 0x00, 0x00, 0x00, 0x00,
																						 0x00, 0x00, 0x00, 0x00,
																						 0x00, 0x00, 0x00, 0x00,
					};
					udpsendlm1266[4] =  Lm1266Data_12v.lm1266_resdata[3];
					udpsendlm1266[5] =  Lm1266Data_12v.lm1266_resdata[4];
					udpsendlm1266[6] =  Lm1266Data_12v.lm1266_resdata[5];
					udpsendlm1266[7] =  Lm1266Data_12v.lm1266_resdata[6];
					
					udpsendlm1266[8] =  Lm1266Data_12v.lm1266_resdata[7];
					udpsendlm1266[9] =  Lm1266Data_12v.lm1266_resdata[8];
					udpsendlm1266[10] =  Lm1266Data_12v.lm1266_resdata[9];
					udpsendlm1266[11] =  Lm1266Data_12v.lm1266_resdata[10];		

					udpsendlm1266[12] =  Lm1266Data_12v.lm1266_resdata[11];
					udpsendlm1266[13] =  Lm1266Data_12v.lm1266_resdata[12];
					udpsendlm1266[14] =  Lm1266Data_12v.lm1266_resdata[13];
					udpsendlm1266[15] =  Lm1266Data_12v.lm1266_resdata[14];
					udp_echoclient_send(udpsendlm1266 , 16);
				}
			}
		}
		else
		{
			timerloop++;
		}
		

		
		if(os_Status4 == 1)
		{
			g_tBspUart4.rxFlag = 0;
			osEventFlagsClear(event_uart4_ID, 0x00000000U);
			if(g_tBspUart4.pRxBuf[0] == 0xCC && g_tBspUart4.pRxBuf[1] == 0xAA && g_tBspUart4.pRxBuf[2] == 0x08)
			{
				unsigned char udpsenddyt[19] = {0x55, 0xaa, 0x12, 0x02};
				//1.״̬�֣�δ��˵��
				udpsenddyt[4] = g_tBspUart4.pRxBuf[3];
				//2.�������߽��ٶȣ����������ȵͺ��Ϊ��
				udpsenddyt[5] = g_tBspUart4.pRxBuf[4];
				udpsenddyt[6] = g_tBspUart4.pRxBuf[5];
				//3.�������߽��ٶ�
				udpsenddyt[7] = g_tBspUart4.pRxBuf[6];
				udpsenddyt[8] = g_tBspUart4.pRxBuf[7];
				//4.����ƫ��
				udpsenddyt[9] = g_tBspUart4.pRxBuf[8];
				udpsenddyt[10] = g_tBspUart4.pRxBuf[9];
				//5.����ƫ��
				udpsenddyt[11] = g_tBspUart4.pRxBuf[10];
				udpsenddyt[12] = g_tBspUart4.pRxBuf[11];
				//6.��������
				udpsenddyt[13] = g_tBspUart4.pRxBuf[12];
				udpsenddyt[14] = g_tBspUart4.pRxBuf[13];
				//7.����״̬
				udpsenddyt[15] = g_tBspUart4.pRxBuf[14];
				//8.��������
				udpsenddyt[16] = g_tBspUart4.pRxBuf[15];
				udpsenddyt[17] = g_tBspUart4.pRxBuf[16];
				/*У����*/
				udpsenddyt[18] = g_tBspUart4.pRxBuf[17];	
				udp_echoclient_send(udpsenddyt , 19);
				// ʹ��memset��������
//				memset(g_tBspUart4.pRxBuf, 0, sizeof(g_tBspUart4.pRxBuf));
			}
		}
		
		/*Test*/
		CanTxHeadType.StdId = 0x0602;
		CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 		
		CAN_Send_Mesage(CAN2_Handler , &CanTxHeadType , &Zerofindway);
		
		//��ѯ�����Ϣ,��
		if(os_Status6 == 2)
		{
			/*���������ѯ*/
			switch(ucloop[0])
			{
				case 0:
					ucloop[0] = 1;	//���㷽ʽ
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
					break;
				case 1:
					ucloop[0] = 2; //λ��ָ��
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
					break;
				case 2:
					ucloop[0] = 3; //ʵ��λ��
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
					break;
				case 3:
					ucloop[0] = 4;	//����ģʽ
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
					break;
				case 4:
					ucloop[0] = 5; //�������
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
					break;
				case 5:
					ucloop[0] = 6;	//�ٶ�ָ��
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
					break;
				case 6:
					ucloop[0] = 7;	//ʵ���ٶ�
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
					break;		
				case 7:
					ucloop[0] = 8;	//״̬��
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
					break;	
				case 8:
					ucloop[0] = 0;	//S1-S14״̬��ѯ
					CanTxHeadType.StdId = 0x0602;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
					break;			
				default:
					break;
			}	
		}
		
		if(os_Status7 == 2)
		{
			switch(ucloop[1])
			{
				case 0:
					ucloop[1] = 1;	//���㷽ʽ
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
					break;
				case 1:
					ucloop[1] = 2; //λ��ָ��
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
					break;
				case 2:
					ucloop[1] = 3; //ʵ��λ��
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
					break;
				case 3:
					ucloop[1] = 4;	//����ģʽ
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
					break;
				case 4:
					ucloop[1] = 5; //�������
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
					break;
				case 5:
					ucloop[1] = 6;	//�ٶ�ָ��
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
					break;
				case 6:
					ucloop[1] = 7;	//ʵ���ٶ�
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
					break;		
				case 7:
					ucloop[1] = 8;	//״̬��
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
					break;
				case 8:
					ucloop[1] = 0;	//S1-S14״̬��ѯ
					CanTxHeadType.StdId = 0x0604;
					CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
					break;	
				default:
					break;
			}
			

		}


		/* ����ӳ� */
		tick += usFrequency;                          
		osDelayUntil(tick);		
	}
}



//void AppTaskDYTData(void *argument)
//{
//	uint16_t usFrequency = 500; /* �ӳ����� */
//	uint32_t tick;
//	/* ��ȡ��ǰʱ�� */
//	tick = osKernelGetTickCount();
//	osStatus_t os_Status; 
//	struct DYTDATA GetDytData;
//	while(1)
//	{
//		os_Status = osMessageQueueGet(msgQueue_DytData, 	/* ��Ϣ����ID */
//																	&GetDytData,     		/* �����Ϣ���ڴ� */
//																	NULL,				 					/* ��Ϣ���ȼ� */
//																	osWaitForever);    		/* ��ʱʱ�䣨һֱ�ȴ��� */
//		if(osOK == os_Status)
//		{
//			
//		}	
//	}
//}



