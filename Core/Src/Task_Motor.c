#include "bsp.h"

int calculateTwosComplement(int input) 
{
    // ������������Ĳ�����ʽ�ĸ���
    int twosComplement = -input;
    return twosComplement;
}



static unsigned char Positiondata[11][8] = { 
 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //�ٶ�
 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //���ٶ�
 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //λ��
 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}
};




/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorSppedScanning
*	����˵��: ��� �ٶ�ɨ��ģʽ
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskMotorSppedScanning(void *argument)
{
	unsigned char clockwiseSpped[8]   = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//˳ʱ���ٶ�
	unsigned char Counterclockwise[8] = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//��ʱ���ٶ�
	unsigned char acceleration[8]     = {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD0 , 0x03 , 0x00};
	unsigned char SppedScanning[6][8] = { 
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
																	 {0x2F , 0x60 , 0x60 , 0x00 , 0x03 , 0x00 , 0x00 , 0x00} ,
																	 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD0 , 0x03 , 0x00} , 
																	 {0x23 , 0xFF , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	struct MOTORSPPEDSCANNING getMotorsppedscanning = {0 , 0};
	osStatus_t os_Status; 
	const uint16_t usFrequency = 150; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount();	
	uint32_t flags = 0;
	static uint32_t msg_Number = 0;
	
	/*��ȡ��Ϣ��������*/
	msg_Number = osMessageQueueGetCount(msgQueue_SppedScanning);
	if(msg_Number >= 1)
	{
		os_Status = osMessageQueueGet(msgQueue_SppedScanning, &getMotorsppedscanning , NULL ,NULL);
		if(os_Status == osOK)
		{
			/*�ٶȣ����ٶȣ����ݿ���*/
			clockwiseSpped[4] = getMotorsppedscanning.spped;
			clockwiseSpped[5] = getMotorsppedscanning.spped >> 8;
			clockwiseSpped[6] = getMotorsppedscanning.spped >> 16;
			clockwiseSpped[7] = getMotorsppedscanning.spped >> 24;
			
			Counterclockwise[4] = calculateTwosComplement(getMotorsppedscanning.spped);
			Counterclockwise[5] = calculateTwosComplement(getMotorsppedscanning.spped) >> 8;
			Counterclockwise[6] = calculateTwosComplement(getMotorsppedscanning.spped) >> 16;
			Counterclockwise[7] = calculateTwosComplement(getMotorsppedscanning.spped) >> 24;
			
			acceleration[4] = getMotorsppedscanning.acceleration;
			acceleration[5] = getMotorsppedscanning.acceleration >> 8;
			acceleration[6] = getMotorsppedscanning.acceleration >> 16;
			acceleration[7] = getMotorsppedscanning.acceleration >> 24;
			
			printf("this is data");
		}
	}
	/*��һ���˶� ����ʵ����ν�������Ƿ����򣬷������ᴥ����λ*/
	for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
	{
		SppedScanning[5][uci] = Counterclockwise[uci];
	}
	for(int i = 0; i <= 5 ; i++)
	{
		CanTxHeadType.StdId = 0x602;
		CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
		tick += usFrequency;                  
		osDelayUntil(tick);
	}	
	tick += usFrequency + 100;              
	osDelayUntil(tick);	
	
	/*����ѯ*/
	osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);			

	while(1)
	{
//	os_Status = osEventFlagsWait(event_SppedScanning_ID , 0x00000017U , osFlagsWaitAny , osWaitForever);
		os_Status = osEventFlagsWait(event_SppedScanning_ID , 0x00000077U , osFlagsWaitAny , osWaitForever);
		//if(os_Status == 0x12 || os_Status == 0x02)//������״̬��ֹͣ���Ҵ���  0x01U << 4   0x01U << 1
		if(os_Status == 0x02)//������״̬��ֹͣ���Ҵ���  0x01U << 4   0x01U << 1
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = Counterclockwise[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x602;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);
			}	
			tick += usFrequency + 50;              
			osDelayUntil(tick);		
		}	
//		else if(os_Status == 0x11 || os_Status == 0x01)//�ڶ���״̬��ֹͣ���󴥷�  0x01U << 4   0x01U << 0  11;
		else if(os_Status == 0x01)
		{
			printf("this is 2");
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = clockwiseSpped[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x602;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);		
			}
			tick += usFrequency + 50;                
			osDelayUntil(tick);		
		}
		else if(os_Status == 0x04)//��һ��״̬���˶���δ���� �� 0x01U << 2 C
		{
			printf("this is 1");
		}
		else if(os_Status == 0x52 || os_Status == 0x42)//�����˶������ٶ�0
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = Counterclockwise[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x602;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);
			}	
			tick += usFrequency + 50;              
			osDelayUntil(tick);					
		}
		else if(os_Status == 0x51 || os_Status == 0x41)//�����˶������ٶ�0
		{
			//�޷��б���
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = clockwiseSpped[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x602;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);		
			}
			tick += usFrequency + 50;                
			osDelayUntil(tick);				
		}
		/*�κ�״̬���յ�ָֹͣ�ֹͣ*/
		else if(os_Status == 0x22 || os_Status == 0x21 || os_Status == 0x24 || os_Status == 0x20 ||
						os_Status == 0x72 || os_Status == 0x62 || os_Status == 0x71 || os_Status == 0x61 )
		{
			//ThreadIdTaskMotorStop = osThreadNew(AppTaskMotorStop, NULL, &TaskMotorSppedScanning_Attr);	
			/*���յ�ָֹͣ����������*/
			osThreadExit();		
		}
//		else if(os_Status == 0x24 || os_Status == 0x34)
//		{
//			/*���յ�ָֹͣ����������*/
//			osThreadExit();
//		}
//		tick += usFrequency;                 
//		osDelayUntil(tick);
	}
}




/*
*********************************************************************************************************
*	�� �� ��: AppTaskUnderMotorSppedScanning
*	����˵��: �±ڵ�� �ٶ�ɨ��ģʽ
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskUnderMotorSppedScanning(void *argument)
{
	unsigned char clockwiseSpped[8]   = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//˳ʱ���ٶ�
	unsigned char Counterclockwise[8] = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//��ʱ���ٶ�
	unsigned char acceleration[8]     = {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD0 , 0x03 , 0x00};
	unsigned char SppedScanning[6][8] = { 
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
																	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
																	 {0x2F , 0x60 , 0x60 , 0x00 , 0x03 , 0x00 , 0x00 , 0x00} ,
																	 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD0 , 0x03 , 0x00} , 
																	 {0x23 , 0xFF , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	struct MOTORSPPEDSCANNING getMotorsppedscanning = {0 , 0};
	osStatus_t os_Status; 
	const uint16_t usFrequency = 150; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount();	
	uint32_t flags = 0;
	static uint32_t msg_Number = 0;
	/*��ȡ��Ϣ��������*/
	msg_Number = osMessageQueueGetCount(msgQueue_UnderSppedScanning);
	if(msg_Number >= 1)
	{
		os_Status = osMessageQueueGet(msgQueue_UnderSppedScanning, &getMotorsppedscanning , NULL ,NULL);
		if(os_Status == osOK)
		{
			/*�ٶȣ����ٶȣ����ݿ���*/
			clockwiseSpped[4] = getMotorsppedscanning.spped;
			clockwiseSpped[5] = getMotorsppedscanning.spped >> 8;
			clockwiseSpped[6] = getMotorsppedscanning.spped >> 16;
			clockwiseSpped[7] = getMotorsppedscanning.spped >> 24;
			
			Counterclockwise[4] = calculateTwosComplement(getMotorsppedscanning.spped);
			Counterclockwise[5] = calculateTwosComplement(getMotorsppedscanning.spped) >> 8;
			Counterclockwise[6] = calculateTwosComplement(getMotorsppedscanning.spped) >> 16;
			Counterclockwise[7] = calculateTwosComplement(getMotorsppedscanning.spped) >> 24;
			
			acceleration[4] = getMotorsppedscanning.acceleration;
			acceleration[5] = getMotorsppedscanning.acceleration >> 8;
			acceleration[6] = getMotorsppedscanning.acceleration >> 16;
			acceleration[7] = getMotorsppedscanning.acceleration >> 24;
			
			printf("this is data");
		}
	}
	/*��һ���˶� ����ʵ����ν�������Ƿ����򣬷������ᴥ����λ*/
	for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
	{
		SppedScanning[5][uci] = Counterclockwise[uci];
	}
	for(int i = 0; i <= 5 ; i++)
	{
		CanTxHeadType.StdId = 0x604;
		CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
		tick += usFrequency;                  
		osDelayUntil(tick);
	}	
	tick += usFrequency + 100;              
	osDelayUntil(tick);	
	
	osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*����ѯ*/
	
	while(1)
	{
		os_Status = osEventFlagsWait(event_belowSppedScanning_ID , 0x00000077U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x12 || os_Status == 0x02)//������״̬��ֹͣ���Ҵ���  0x01U << 4   0x01U << 1
		if(os_Status == 0x2)//������״̬��ֹͣ���Ҵ���  0x01U << 4   0x01U << 1
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = Counterclockwise[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x604;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);
			}	
			tick += usFrequency + 50;              
			osDelayUntil(tick);		
		}	
//		else if(os_Status == 0x11 || os_Status == 0x01)//�ڶ���״̬��ֹͣ���󴥷�  0x01U << 4   0x01U << 0  11;
		else if(os_Status == 0x1)//�ڶ���״̬��ֹͣ���󴥷�  0x01U << 4   0x01U << 0  11;
		{
			printf("this is 2");
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = clockwiseSpped[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x604;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);		
			}
			tick += usFrequency + 50;                
			osDelayUntil(tick);		
		}
		else if(os_Status == 0x04)//��һ��״̬���˶���δ���� �� 0x01U << 2 C
		{
			//printf("this is 1");
		}
		else if(os_Status == 0x52 || os_Status == 0x42)//�����˶������ٶ�0
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = Counterclockwise[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x604;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);
			}	
			tick += usFrequency + 50;              
			osDelayUntil(tick);				
		}
		else if(os_Status == 0x51 || os_Status == 0x41)
		{
			//�޷��б���
			for(unsigned char uci = 4; uci <= 7 ; uci++)//���˶�
			{
				SppedScanning[5][uci] = clockwiseSpped[uci];
			}
			for(int i = 3; i <= 5 ; i++)
			{
				CanTxHeadType.StdId = 0x604;
				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedScanning[i]);
				tick += usFrequency;                  
				osDelayUntil(tick);		
			}
			tick += usFrequency + 50;                
			osDelayUntil(tick);					
		}
		/*�κ�״̬���յ�ָֹͣ�ֹͣ*/
		/*�κ�״̬���յ�ָֹͣ�ֹͣ*/
		else if(os_Status == 0x22 || os_Status == 0x21 || os_Status == 0x24 || os_Status == 0x20 ||
						os_Status == 0x72 || os_Status == 0x62 || os_Status == 0x71 || os_Status == 0x61 )
		{
			//ThreadIdTaskUnderMotorStop = osThreadNew(AppTaskUnderMotorStop, NULL, &TaskUnderMotorSppedScanning_Attr);	
			/*���յ�ָֹͣ����������*/
			osThreadExit();		
		}
		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorFindZero
*	����˵��: ������ݲ�ѯ������ѯ���� CANOPEN ���������ѯ����
*	��    ��: 
*	�� �� ֵ: ��
* �� �� ��:  
*********************************************************************************************************
*/
void AppTaskMotorDataquery(void *argument)
{
//	osStatus_t os_Status;
//	uint32_t flags = 0;
//	const uint16_t usFrequency = 10; /* �ӳ����� */
//	uint32_t tick;	
//	tick = osKernelGetTickCount();
//	/*���ݲ�ѯ����*/
//	
//	/*1.���㷽ʽ*/
//	unsigned char Zerofindway[8] = {0x40, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*2.λ��ָ��*/
//	unsigned char PositionCommand[8] = {0x40, 0x62, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*3.ʵ��λ��*/
//	unsigned char PositionReal[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*4.����ģʽ*/
//	unsigned char OperateMode[8] = {0x40, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*5.�������ѯ*/
//	unsigned char ErrorCode[8] = {0x40, 0x3F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*6.�ٶ�ָ��*/
//	unsigned char SppedCommand[8] = {0x40, 0x6B, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*7.ʵ���ٶ�*/		
//	unsigned char SppedReal[8] = {0x40, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*8.״̬��*/
//	unsigned char StateWord[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*S1-S17���*/
//	unsigned char ClickS1_S17[8] = {0x40, 0xFD, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	
//	unsigned char ucloop = 0;
//	unsigned char ucloop_2 = 0;
//	/*��ʼ������������*/	
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
//	while(1)
//	{
//		switch(ucloop)
//		{
//			case 0:
//				ucloop = 1;	//���㷽ʽ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
//				break;

//			case 1:
//				ucloop = 2; //λ��ָ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
//				break;

//			case 2:
//				ucloop = 3; //ʵ��λ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
//				break;

//			case 3:
//				ucloop = 4;	//����ģʽ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
//				break;

//			case 4:
//				ucloop = 5; //�������
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
//				break;

//			case 5:
//				ucloop = 6;	//�ٶ�ָ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
//				break;

//			case 6:
//				ucloop = 7;	//ʵ���ٶ�
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
//				break;		

//			case 7:
//				ucloop = 8;//״̬��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
//				break;
//			
//			case 8:
//				ucloop = 0;//S1-S14״̬��ѯ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
//				break;			

//			default:
//				break;
//		}
//		
//		/*����û���Ҫ�Լ���������Ҫ������ر�����̣߳��Է�ֹ��λ���ظ���*/
////		flags = osEventFlagsGet(event_closurefinddata_ID);
////		if(flags == 0x01)
////		{
////			osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
////			/*�߳��˳�*/
////			osThreadExit();
////		}
//		
//		tick += usFrequency; //��Լ5s�󵽴︺���޵�                  
//		osDelayUntil(tick);
//	}		
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUnderMotorDataquery
*	����˵��: �±ڵ�����ݲ�ѯ������ѯ���� CANOPEN ���������ѯ����
*	��    ��: 
*	�� �� ֵ: ��
* �� �� ��:  
*********************************************************************************************************
*/
void AppTaskUnderMotorDataquery(void *argument)
{
//	osStatus_t os_Status;
//	uint32_t flags = 0;
//	const uint16_t usFrequency = 10; /* �ӳ����� */
//	uint32_t tick;	
//	tick = osKernelGetTickCount();
//	/*���ݲ�ѯ����*/
//	
//	/*1.���㷽ʽ*/
//	unsigned char Zerofindway[8] = {0x40, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*2.λ��ָ��*/
//	unsigned char PositionCommand[8] = {0x40, 0x62, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*3.ʵ��λ��*/
//	unsigned char PositionReal[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*4.����ģʽ*/
//	unsigned char OperateMode[8] = {0x40, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*5.�������ѯ*/
//	unsigned char ErrorCode[8] = {0x40, 0x3F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*6.�ٶ�ָ��*/
//	unsigned char SppedCommand[8] = {0x40, 0x6B, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*7.ʵ���ٶ�*/		
//	unsigned char SppedReal[8] = {0x40, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*8.״̬��*/
//	unsigned char StateWord[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*S1-S17���*/
//	unsigned char ClickS1_S17[8] = {0x40, 0xFD, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	
//	unsigned char ucloop = 0;
//	/*��ʼ������������*/	
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };

//	while(1)
//	{
//		switch(ucloop)
//		{
//			case 0:
//				ucloop = 1;	//���㷽ʽ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
//				break;

//			case 1:
//				ucloop = 2; //λ��ָ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
//				break;

//			case 2:
//				ucloop = 3; //ʵ��λ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
//				break;

//			case 3:
//				ucloop = 4;	//����ģʽ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
//				break;

//			case 4:
//				ucloop = 5; //�������
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
//				break;

//			case 5:
//				ucloop = 6;	//�ٶ�ָ��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
//				break;

//			case 6:
//				ucloop = 7;	//ʵ���ٶ�
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
//				break;		

//			case 7:
//				ucloop = 8;//״̬��
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
//				break;
//			
//			case 8:
//				ucloop = 0;//S1-S14״̬��ѯ
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
//				break;			

//			default:
//				break;
//		}		
//		/*����û���Ҫ�Լ���������Ҫ������ر�����̣߳��Է�ֹ��λ���ظ���*/
////		flags = osEventFlagsGet(event_belowclosurefinddata_ID);
////		if(flags == 0x01)
////		{
////			osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000001U);
////			/*�߳��˳�*/
////			osThreadExit();
////		}
//		tick += usFrequency; //��Լ5s�󵽴︺���޵�                  
//		osDelayUntil(tick);
//	}		
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorPosition
*	����˵��: ��� ��λģʽ
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskMotorPosition(void *argument)
{
	osStatus_t os_Status; 
	const uint16_t usFrequency = 100; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount();
	struct MOTORPOSITION getMotorPosition = {0 , 0 , 0};
	
//	unsigned char Positiondata[11][8] = { 
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
//																		 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //�ٶ�
//																		 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //���ٶ�
//																		 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //λ��
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	
	while(1)
	{
			os_Status = osMessageQueueGet(msgQueue_PositionData, /* ��Ϣ����ID */
																		&getMotorPosition,     /* �����Ϣ���ڴ� */
																		NULL,				 					 /* ��Ϣ���ȼ� */
																		osWaitForever);    		 /* ��ʱʱ�䣨һֱ�ȴ��� */
			if(osOK == os_Status)
			{
					//���ȸ�ֵ�ٶ�
					Positiondata[4][4] = getMotorPosition.spped;			//���λ
					Positiondata[4][5] = getMotorPosition.spped >> 8;
					Positiondata[4][6] = getMotorPosition.spped >> 16;
					Positiondata[4][7] = getMotorPosition.spped >> 24;//���λ
				
					//��ֵ���ٶ�
					Positiondata[5][4] = getMotorPosition.acceleration;			//���λ
					Positiondata[5][5] = getMotorPosition.acceleration >> 8;
					Positiondata[5][6] = getMotorPosition.acceleration >> 16;
					Positiondata[5][7] = getMotorPosition.acceleration >> 24;//���λ			

					//��ֵλ��
					Positiondata[6][4] = getMotorPosition.position;			//���λ
					Positiondata[6][5] = getMotorPosition.position >> 8;
					Positiondata[6][6] = getMotorPosition.position >> 16;
					Positiondata[6][7] = getMotorPosition.position >> 24;//���λ		
					/*������ֵָ��*/
					for(int i = 0; i <= 8 ; i++)
					{
						CanTxHeadType.StdId = 0x602;
						CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Positiondata[i]);
						tick += usFrequency;                  
						osDelayUntil(tick);		
					}
					tick += usFrequency;                  
					osDelayUntil(tick);		
						
					osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
					osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);		//Ϊ2		
					/*�˳��߳�*/
					osThreadExit();					
			}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorPosition
*	����˵��: �±ڵ�� ��λģʽ
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskUnderMotorPosition(void *argument)
{
	osStatus_t os_Status; 
	const uint16_t usFrequency = 50; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount();
	struct MOTORPOSITION getMotorPosition = {0 , 0 , 0};
	
//	unsigned char Positiondata[11][8] = { 
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
//																		 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //�ٶ�
//																		 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //���ٶ�
//																		 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //λ��
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	
	while(1)
	{
			os_Status = osMessageQueueGet(msgQueue_UnderPositionData, /* ��Ϣ����ID */
																		&getMotorPosition,     /* �����Ϣ���ڴ� */
																		NULL,				 					 /* ��Ϣ���ȼ� */
																		osWaitForever);    		 /* ��ʱʱ�䣨һֱ�ȴ��� */
			if(osOK == os_Status)
			{
					//���ȸ�ֵ�ٶ�
					Positiondata[4][4] = getMotorPosition.spped;			//���λ
					Positiondata[4][5] = getMotorPosition.spped >> 8;
					Positiondata[4][6] = getMotorPosition.spped >> 16;
					Positiondata[4][7] = getMotorPosition.spped >> 24;//���λ
				
					//��ֵ���ٶ�
					Positiondata[5][4] = getMotorPosition.acceleration;			//���λ
					Positiondata[5][5] = getMotorPosition.acceleration >> 8;
					Positiondata[5][6] = getMotorPosition.acceleration >> 16;
					Positiondata[5][7] = getMotorPosition.acceleration >> 24;//���λ			

					//��ֵλ��
					Positiondata[6][4] = getMotorPosition.position;			//���λ
					Positiondata[6][5] = getMotorPosition.position >> 8;
					Positiondata[6][6] = getMotorPosition.position >> 16;
					Positiondata[6][7] = getMotorPosition.position >> 24;//���λ		
					/*������ֵָ��*/
					for(int i = 0; i <= 8 ; i++)
					{
						CanTxHeadType.StdId = 0x604;
						CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Positiondata[i]);
						tick += usFrequency;                  
						osDelayUntil(tick);		
					}
					tick += usFrequency;                  
					osDelayUntil(tick);	
					
					osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000001U);
					osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*����ѯ*/
					/*�˳��߳�*/
					osThreadExit();					
			}
	}
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorFindZero
*	����˵��: �������������һֱ��˼����������費��Ҫ���û����ò���
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskMotorFindZero(void *argument)
{
	osStatus_t os_Status;
	osStatus_t status; 
	const uint16_t usFrequency = 50; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount(); 
	static unsigned char findzerodata[11][8] = {
	 {0x81 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x01 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
	 {0x2F , 0x60 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x23 , 0x99 , 0x60 , 0x01 , 0x88 , 0x13 , 0x00 , 0x00} , 	//��ԭ���ٶ� ���� 5000
	 {0x23 , 0x99 , 0x60 , 0x02 , 0xE8 , 0x03 , 0x00 , 0x00} ,	//��ԭ���ٶ� ���� 1000
	 {0x23 , 0x9A , 0x60 , 0x00 , 0x30 , 0x75 , 0x00 , 0x00} ,	//��ԭ����ٶ� 30,000
	 {0x2F , 0x98 , 0x60 , 0x00 , 0x11 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x1F , 0x00 , 0x00 , 0x00}
	};
//	unsigned int findzeroH = 0X00002710; //10000
//	unsigned int findzeroL = 0X000003E8; //5000L
	findzerodata[0][1] = 0x02;
	findzerodata[1][1] = 0x02;		
	/*������֤����ٶȲ�����ȡ�����������Ӱ���������*/
	unsigned int findzeroH = 0X00001388; 
	unsigned int findzeroL = 0X000003E8; 
	
	findzerodata[6][4] = findzeroH;				//���λ
	findzerodata[6][5] = findzeroH >> 8;
	findzerodata[6][6] = findzeroH >> 16;
	findzerodata[6][7] = findzeroH >> 24;	//���λ
	
	findzerodata[7][4] = findzeroL;				//���λ
	findzerodata[7][5] = findzeroL >> 8;
	findzerodata[7][6] = findzeroL >> 16;
	findzerodata[7][7] = findzeroL >> 24;	//���λ	
	
	/*�ʵ���ʱ�ſ����������㣬�����ͻ����޷���Ӧ*/
	tick += usFrequency + 1000; 
//	tick += usFrequency + 300;	
	osDelayUntil(tick);	
	
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x000 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };	
	for(int i = 0; i <= 10 ; i++)
	{
		if(i < 2)
		{
			CanTxHeadType.StdId = 0x000;
			CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &findzerodata[i]);
		}
		else
		{
			CanTxHeadType.StdId = 0x602;
			CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &findzerodata[i]);
		}
		tick += usFrequency;                  
		osDelayUntil(tick);		
	}
//	tick += usFrequency + 250;                  
//	osDelayUntil(tick);		
	
	/*����ѯ*/					
	osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);	
	
	osThreadExit();//�Լ�����

	/*�õ���˶�2S����ȥ���Ͳ�ѯ��Ϣ���Է�ֹ������������޵㣬������λ��־ �����ܷŵ����������ֻ�Ӱ�����*/	
	/*������ݲ�ѯ����*/						
//	ThreadIdTaskMotorDataquery = osThreadNew(AppTaskMotorDataquery, NULL, &TaskMotorDataquery_Attr);	
//	while(1)
//	{
		/*�߳�ͬ��*/
//		os_Status = osEventFlagsWait(event_negativelimit_ID , 0x00000003U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x01)//��
//		{			
			/*������Ϣ���� 				λ����Ϣ 23125  0x00005A55 ,
														�ٶ�		 10000  0X00002710 ,
													  ���ٶ�   250000 0x0003D090 ,
			*/
//		struct MOTORPOSITION setMotorPosition = {0x00005A55 , 0X00002710 , 0x0003D090};	
//		osMessageQueuePut(msgQueue_PositionData,  /* ��Ϣ���е����� */
//											&setMotorPosition, 			/* ������Ϣ���еı�������Ϣ�� */
//											NULL,              			/* ������Ϣ�����ȼ� */
//											NULL               			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
//											); 	
//		osEventFlagsClear(event_negativelimit_ID, 0x0000000FU);
//		ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);		
			/*�˳��߳�*/
//			osThreadExit();
//		}
//		else if(os_Status == 0x02)//��
//		{
//			printf("this is right");
			
//			/*������Ϣ���� 				λ����Ϣ 23125  0x00005A55 ,
//															�ٶ�		 10000  0X00002710 ,
//															���ٶ�   250000 0x0003D090 ,
//			*/
//			struct MOTORPOSITION setMotorPosition = {0x00005A55 , 0X00002710 , 0x0003D090};	
//			osMessageQueuePut(msgQueue_PositionData,  /* ��Ϣ���е����� */
//												&setMotorPosition, 			/* ������Ϣ���еı�������Ϣ�� */
//												NULL,              			/* ������Ϣ�����ȼ� */
//												NULL               			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
//												); 	
//			osEventFlagsClear(event_negativelimit_ID, 0x0000000FU);
//			ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);	
//			/*�˳��߳�*/
//				osThreadExit();			
//		}
//	}
}



/*
*********************************************************************************************************
*	�� �� ��: AppTaskUnderMotorFindZero
*	����˵��: �±ڵ��Ѱ������
*	��    ��: 
*	�� �� ֵ: ��
*   �� �� ��:  
*********************************************************************************************************
*/
void AppTaskUnderMotorFindZero(void *argument)
{
	osStatus_t os_Status;
	const uint16_t usFrequency = 100; /* �ӳ����� */
	uint32_t tick;	
	tick = osKernelGetTickCount(); 
	/*�±ڵ�����Ϊ4*/
	unsigned char findzerodata[11][8] = {
	 {0x81 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x01 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
	 {0x2F , 0x60 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x23 , 0x99 , 0x60 , 0x01 , 0x88 , 0x13 , 0x00 , 0x00} , 	//��ԭ���ٶ� ���� 5000
	 {0x23 , 0x99 , 0x60 , 0x02 , 0xE8 , 0x03 , 0x00 , 0x00} ,	//��ԭ���ٶ� ���� 1000
	 {0x23 , 0x9A , 0x60 , 0x00 , 0x30 , 0x75 , 0x00 , 0x00} ,	//��ԭ����ٶ� 30,000
	 {0x2F , 0x98 , 0x60 , 0x00 , 0x11 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x1F , 0x00 , 0x00 , 0x00}
	};
//	unsigned int findzeroH = 0X00002710; //10000
//	unsigned int findzeroL = 0X000003E8; //5000L
	
	findzerodata[0][1] = 0x04;
	findzerodata[1][1] = 0x04;	
	
	/*������֤����ٶȲ�����ȡ�����������Ӱ���������*/
	unsigned int findzeroH = 0X00001388; 
	unsigned int findzeroL = 0X000003E8; 
	
	findzerodata[6][4] = findzeroH;				//���λ
	findzerodata[6][5] = findzeroH >> 8;
	findzerodata[6][6] = findzeroH >> 16;
	findzerodata[6][7] = findzeroH >> 24;	//���λ
	
	findzerodata[7][4] = findzeroL;				//���λ
	findzerodata[7][5] = findzeroL >> 8;
	findzerodata[7][6] = findzeroL >> 16;
	findzerodata[7][7] = findzeroL >> 24;	//���λ	
	
	/*�ʵ���ʱ�ſ����������㣬�����ͻ����޷���Ӧ*/
	tick += usFrequency + 1000;
//	tick += usFrequency + 300;   
	osDelayUntil(tick);	
	
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };	
	for(int i = 0; i <= 10 ; i++)
	{
		if(i < 2)
		{
			CanTxHeadType.StdId = 0x000;
			CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &findzerodata[i]);
		}
		else
		{
			CanTxHeadType.StdId = 0x604;
			CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &findzerodata[i]);
		}
		tick += usFrequency;                  
		osDelayUntil(tick);		
	}
	tick += usFrequency + 250;                  
	osDelayUntil(tick);

	osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*����ѯ*/
	
	osThreadExit();//�Լ�����

	/*�õ���˶�2S����ȥ���Ͳ�ѯ��Ϣ���Է�ֹ������������޵㣬������λ��־ �����ܷŵ����������ֻ�Ӱ�����*/	
	/*������ݲ�ѯ����*/						
	//ThreadIdTaskUnderMotorDataquery = osThreadNew(AppTaskUnderMotorDataquery, NULL, &TaskUnderMotorDataquery_Attr);	
//	while(1)
//	{
		/*�߳�ͬ��*/
		//os_Status = osEventFlagsWait(event_belownegativelimit_ID , 0x00000003U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x01)//��
//		{			
			/*�˳��߳�*/
//			osThreadExit();
//		}
//		else if(os_Status == 0x02)//��
//		{
//			printf("this is right");
			
				/*������Ϣ���� 				λ����Ϣ 23125  0x00005A55 ,
															�ٶ�		 10000  0X00002710 ,
															���ٶ�   250000 0x0003D090 ,
				*/
//			struct MOTORPOSITION setMotorPosition = {0xFFFFA5AB , 0X00002710 , 0x0003D090};	
//			osMessageQueuePut(msgQueue_UnderPositionData,  /* ��Ϣ���е����� */
//												&setMotorPosition, 			/* ������Ϣ���еı�������Ϣ�� */
//												NULL,              			/* ������Ϣ�����ȼ� */
//												NULL               			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
//												); 	
//			osEventFlagsClear(event_belownegativelimit_ID, 0x0000000FU);
			
			/*�˳��߳�*/
//			osThreadExit();
			
//		}
//	}	
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorSpped
*	����˵��: ����ٶ�ģʽ����,��ʱ�����ò��ϣ��Ȳ�д��
*	��    ��: 
*	�� �� ֵ: ��
* �� �� ��:  
*********************************************************************************************************
*/
void AppTaskMotorSpped(void *argument)
{}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMotorStop
*	����˵��: ���ֹͣ����
*	��    ��: 
*	�� �� ֵ: ��
* �� �� ��:  
*********************************************************************************************************
*/
//void AppTaskMotorStop(void *argument)
//{
//	osStatus_t os_Status;
//	const uint16_t usFrequency = 100; /* �ӳ����� */
//	uint32_t tick;	
//	tick = osKernelGetTickCount(); 
//	unsigned char stopdata[] = {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00};
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };	
//	CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &stopdata);	
//	osThreadExit();		
//}

//void AppTaskUnderMotorStop(void *argument)
//{
//	osStatus_t os_Status;
//	const uint16_t usFrequency = 100; /* �ӳ����� */
//	uint32_t tick;	
//	tick = osKernelGetTickCount(); 
//	unsigned char stopdata[] = {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00};
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };	
//	CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &stopdata);	
//	osThreadExit();
//}
