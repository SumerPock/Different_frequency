#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: AppTaskEthCheck
*	����˵��: ������߲��״̬��
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 
*********************************************************************************************************
*/
void AppTaskEthCheck(void *argument)
{
	const uint16_t usFrequency = 250;	/* �ӳ����� */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* ��ȡ��ǰʱ�� */
	osStatus_t os_Status;
	unsigned char strct = 0;
	while(1)
	{
		/*DUBUGģʽ�¿�������ʶ�𣬵��ϵ�󲻽���DUBUGģʽ���޷�ʶ��*/
		ethernetif_check_link(&netif);
		/*ֻ�е����߲��Ϻ�Ž���UDP����*/
		if(netif_is_link_up(&netif)) 
		{
			osEventFlagsClear(event_cable_ID , 0x02U);
			osEventFlagsSet(event_cable_ID , 0x01U);
			//g_ucEthLinkStatus = 1;	
			udp_echoserver_init();
			//�رմ��߳�
			osThreadExit(); 
		}
		else
		{
			osEventFlagsClear(event_cable_ID , 0x01U);
			osEventFlagsSet(event_cable_ID , 0x02U);
			//if(g_ucEthLinkStatus == 1)
			//{
			//	g_ucEthLinkStatus = 2;
			//}
		}	
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);	
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskLwIPPro
*	����˵��: LwIP���ݽ��մ���
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��:   
*********************************************************************************************************
*/
void AppTaskLwIPPro(void *argument)
{
	const uint16_t usFrequency = 20;	/* �ӳ����� */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* ��ȡ��ǰʱ�� */
	osStatus_t os_Status;
	while(1)
	{
		ethernetif_poll(&netif);
		tick += usFrequency;    /* ����ӳ� */                      
		osDelayUntil(tick);	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUdpNetWork
*	����˵��: LwIP���ݽ��մ���
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��:   
*********************************************************************************************************
*/
void AppTaskUdpNetWork(void *argument)
{
	const uint16_t usFrequency = 100;	/* �ӳ����� */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* ��ȡ��ǰʱ�� */
	osStatus_t os_Status;
	struct GETUDPDATA GetUdpData = {0};
	struct MOTORPOSITION setMotorPosition = {0X00002710 , 0x0003D090 , 0x00005A55}; //λ�ã��ٶȣ����ٶ�
	struct MOTORFINDZERO setMotorFindZero = {0x1388 , 0x2710 , 0x7530 , 0x11};			//���ٻ��㣬���ٻ��㣬������ٶȣ����㷽ʽ
	struct MOTORSPPEDSCANNING setMotorSppedScanning = {0x0003D090 , 0x00005A55};		//�ٶȣ����ٶ�
	
	struct MOTORPOSITION belowsetMotorPosition = {0X00002710 , 0x0003D090 , 0x00005A55}; //λ�ã��ٶȣ����ٶ�
	struct MOTORFINDZERO belowsetMotorFindZero = {0x1388 , 0x2710 , 0x7530 , 0x11};			//���ٻ��㣬���ٻ��㣬������ٶȣ����㷽ʽ
	struct MOTORSPPEDSCANNING belowsetMotorSppedScanning = {0x0003D090 , 0x00005A55};		//�ٶȣ����ٶ�	
	
	while(1)
	{
		os_Status = osMessageQueueGet(msgQueue_UdpResData,  /* ��Ϣ����ID */
										&GetUdpData,          							/* �����Ϣ���ڴ� */
										NULL,						 										/* ��Ϣ���ȼ� */
										osWaitForever);    									/* ��ʱʱ�䣨һֱ�ȴ��� */		
		if(osOK == os_Status)
		{
				if(GetUdpData.ucGetUdpData[0] == 0xAA && GetUdpData.ucGetUdpData[1] == 0xBB && GetUdpData.ucGetUdpData[2] == 0xCC)
				{
					//4 Ϊ����ID
					if(GetUdpData.ucGetUdpData[4] == 0x01 && 
						(GetUdpData.ucGetUdpData[3] == 0x14 || GetUdpData.ucGetUdpData[3] == 0x13 ||
						 GetUdpData.ucGetUdpData[3] == 0x0F))
					{
						switch(GetUdpData.ucGetUdpData[5]) 			//�������
						{
							case 1:	//1�ŵ�������ϱ�
								switch(GetUdpData.ucGetUdpData[6]) 	//�˶�ģʽ
								{
									case 1:	//λ��ģʽ
										//λ��
										setMotorPosition.position = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);
										//�ٶ�
										setMotorPosition.spped = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																					| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
										//���ٶ�
										setMotorPosition.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																									| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
										//��Ϣ����
										osMessageQueuePut(msgQueue_PositionData,  /* ��Ϣ���е����� */
																			&setMotorPosition, 			/* ������Ϣ���еı�������Ϣ�� */
																			NULL,              			/* ������Ϣ�����ȼ� */
																			NULL               			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
																			); 

										osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
										osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //Ϊ1	/*�رյ����ѯ����*/
										ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);	
										break;
									
									case 2://�ٶ�ɨ��ģʽ
											//�ٶ�
											setMotorSppedScanning.spped = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//���ٶ�
											setMotorSppedScanning.acceleration = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);		
											/*��Ϣ����*/
											osMessageQueuePut(msgQueue_SppedScanning,  /* ��Ϣ���е����� */
																				&setMotorSppedScanning, 			/* ������Ϣ���еı�������Ϣ�� */
																				NULL,              						/* ������Ϣ�����ȼ� */
																				NULL               						/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
																				);
											osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //Ϊ1	/*�رյ����ѯ����*/
											//�����ٶ�ɨ���־λ
											event_SppedScanning_ID = osEventFlagsNew(&event_SppedScanning_Attr);									
											//�����ٶ�ɨ������ 
											ThreadIdTaskMotorSppedScanning = osThreadNew(AppTaskMotorSppedScanning, NULL, &TaskMotorSppedScanning_Attr);								

											//����ֹͣģʽ��־λ
										break;
									
									case 3:	//ԭ��ģʽ
											//���ٻ�ԭ��
											setMotorFindZero.sppedhight = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//���ٻ�ԭ��
											setMotorFindZero.sppedlow = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
											//��ԭ����ٶ�
											setMotorFindZero.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																						| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
											//��ԭ��ģʽ
											setMotorFindZero.findzeromode = GetUdpData.ucGetUdpData[19];
											//��ѯ���������־��
//											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0);
									
											osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); /*�رյ����ѯ����*/
											//�������� 
											ThreadIdTaskMotorFindZero = osThreadNew(AppTaskMotorFindZero, NULL, &ThreadMotorFindZero_Attr);
										break;
									
									case 4: //ɨ��ֹͣ
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 5);
										break;
									
									default:
										break;
								}
								break;
							
							case 2:	//2�ŵ�������±�
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://λ��ģʽ
											//λ��
										belowsetMotorPosition.position = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);
										//�ٶ�
										belowsetMotorPosition.spped = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																					| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
										//���ٶ�
										belowsetMotorPosition.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																									| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
										//��Ϣ����
										osMessageQueuePut(msgQueue_UnderPositionData,  /* ��Ϣ���е����� */
																			&belowsetMotorPosition, 			/* ������Ϣ���еı�������Ϣ�� */
																			NULL,              			/* ������Ϣ�����ȼ� */
																			NULL               			/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
																			); 	
										osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
										osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*�ز�ѯ*/
										ThreadIdTaskUnderMotorPosition = osThreadNew(AppTaskUnderMotorPosition, NULL, &TaskUnderMotorPosition_Attr);

										break;
									
									case 2://�ٶ�ɨ��ģʽ
											//�ٶ�
											belowsetMotorSppedScanning.spped = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//���ٶ�
											belowsetMotorSppedScanning.acceleration = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);		
											/*��Ϣ����*/
											osMessageQueuePut(msgQueue_UnderSppedScanning,  /* ��Ϣ���е����� */
																				&belowsetMotorSppedScanning, 	/* ������Ϣ���еı�������Ϣ�� */
																				NULL,              						/* ������Ϣ�����ȼ� */
																				NULL               						/* ��ʱʱ��Ϊ0�����osMessageQueuePut���жϻص���������ʱ���˲���������NULL */
																				);
									
											osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*�ز�ѯ*/
											//�����ٶ�ɨ���־λ
											event_belowSppedScanning_ID = osEventFlagsNew(&event_belowSppedScanning_Attr);
											//�����ٶ�ɨ������ 
											ThreadIdTaskUnderMotorSppedScanning = osThreadNew(AppTaskUnderMotorSppedScanning, NULL, &TaskUnderMotorSppedScanning_Attr);								

											//����ֹͣģʽ��־λ									
										break;
								
									case 3://ԭ��ģʽ
											//���ٻ�ԭ��
											belowsetMotorFindZero.sppedhight = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//���ٻ�ԭ��
											belowsetMotorFindZero.sppedlow = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
											//��ԭ����ٶ�
											belowsetMotorFindZero.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																						| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
											//��ԭ��ģʽ
											belowsetMotorFindZero.findzeromode = GetUdpData.ucGetUdpData[19];
											//��ѯ���������־��
//											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0);
											osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*�ز�ѯ*/									
											//�������� 
											ThreadIdTaskUnderMotorFindZero = osThreadNew(AppTaskUnderMotorFindZero, NULL, &ThreadUnderMotorFindZero_Attr);
										break;
									
									case 4: //ɨ��ֹͣ
										osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 5);
										break;
									
									default:
										break;
								}							
								break;
							
							default:
								break;
						}
					}
					
					//����ͷ��������
					else if(GetUdpData.ucGetUdpData[3] == 0X0A && GetUdpData.ucGetUdpData[4] == 0x03)
					{
						unsigned char ligner_data[7] = {0XAA, 0XCC, 0X02, 0X50, 0XC3, 0X55, 0X68};
						if(GetUdpData.ucGetUdpData[5] == 0x02 && GetUdpData.ucGetUdpData[8] == 0x55)
						{
							ligner_data[2] = 0x02;
							ligner_data[3] = GetUdpData.ucGetUdpData[6];
							ligner_data[4] = GetUdpData.ucGetUdpData[7];
							ligner_data[5] = GetUdpData.ucGetUdpData[8];
							ligner_data[6] = GetUdpData.ucGetUdpData[9];
							BSP_UART_Transmit_IT(&huart4, 	ligner_data , 7);
							//BSP_UART_Transmit_DMA(&huart4, 	ligner_data , 7);
						}
						else if(GetUdpData.ucGetUdpData[5] == 0x02 && GetUdpData.ucGetUdpData[8] == 0xFF)
						{
							ligner_data[2] = 0x02;
							ligner_data[3] = GetUdpData.ucGetUdpData[6];
							ligner_data[4] = GetUdpData.ucGetUdpData[7];
							ligner_data[5] = GetUdpData.ucGetUdpData[8];
							ligner_data[6] = GetUdpData.ucGetUdpData[9];
							BSP_UART_Transmit_IT(&huart4, 	ligner_data , 7);
							//BSP_UART_Transmit_DMA(&huart4, 	ligner_data , 7);
						}
					}
					
					//����ͷԤ�ý�
					else if(GetUdpData.ucGetUdpData[3] == 0X0C && GetUdpData.ucGetUdpData[4] == 0x03)
					{
						unsigned char buffer[] = {0xAA, 0xCC, 0x03, 0xC4, 0x09, 0xC4, 0x09, 0x00, 0x00, 0x9A};
						if(GetUdpData.ucGetUdpData[5] == 0x03 && GetUdpData.ucGetUdpData[10] == 0x00 && GetUdpData.ucGetUdpData[11] == 0x00)
						{
							buffer[2] = GetUdpData.ucGetUdpData[5];
							
							buffer[3] = GetUdpData.ucGetUdpData[6];
							buffer[4] = GetUdpData.ucGetUdpData[7];
							
							buffer[5] = GetUdpData.ucGetUdpData[8];
							buffer[6] = GetUdpData.ucGetUdpData[9];	
							
							buffer[7] = GetUdpData.ucGetUdpData[10];
							buffer[8] = GetUdpData.ucGetUdpData[11];	
							buffer[9] = GetUdpData.ucGetUdpData[6] + GetUdpData.ucGetUdpData[7]  + GetUdpData.ucGetUdpData[8] + \
												  GetUdpData.ucGetUdpData[9] + GetUdpData.ucGetUdpData[10] + GetUdpData.ucGetUdpData[11];
							BSP_UART_Transmit_IT(&huart4, 	buffer , 10);
							//BSP_UART_Transmit_DMA(&huart4, 	buffer , 10);
						}
					}
					
					else if(GetUdpData.ucGetUdpData[3] == 0X0A && GetUdpData.ucGetUdpData[4] == 0x02)
					{//10����Ϊ��ȷƵ����
//						unsigned char ligner_data[7] = {0xAA, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00};
						unsigned char ligner_data[8] = {0xAA, 0x08, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00};
						switch(GetUdpData.ucGetUdpData[5])
						{//1��
							case 1:
								//��ȷƵ����
								ligner_data[4] = GetUdpData.ucGetUdpData[6];
								ligner_data[5] = GetUdpData.ucGetUdpData[7];
								ligner_data[6] = GetUdpData.ucGetUdpData[8];
								ligner_data[7] = GetUdpData.ucGetUdpData[9];
								BSP_UART_Transmit_IT(&husart2,  ligner_data , 8);	
								//BSP_UART_Transmit_DMA(&husart2, 	ligner_data , 8);
								break;
							case 2://2��
								//��ȷƵ����
								ligner_data[4] = GetUdpData.ucGetUdpData[6];
								ligner_data[5] = GetUdpData.ucGetUdpData[7];
								ligner_data[6] = GetUdpData.ucGetUdpData[8];
								ligner_data[7] = GetUdpData.ucGetUdpData[9];
								BSP_UART_Transmit_IT(&huart5,  ligner_data , 8);
								//BSP_UART_Transmit_DMA(&huart5, 	ligner_data , 8);							
								break;
							default:
								break;
						}
					}
					else if(GetUdpData.ucGetUdpData[3] == 0X07 && GetUdpData.ucGetUdpData[4] == 0x02)
					{//7����Ϊ�������
						unsigned char ligner_data[4] = {0xAA, 0x04, 0x00, 0x01};
						switch(GetUdpData.ucGetUdpData[5])
						{//1��
							case 1:
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://��⿪
										ligner_data[3] = 0x03;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);
										break;
									case 2:	//����
										ligner_data[3] = 0x04;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);									
										break;
									case 3:	//���⿪
										ligner_data[3] = 0x01;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);	
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);
										break;
									case 4://�����
										ligner_data[3] = 0x02;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);									
										break;
									default:
										break;
								}
								break;
							//2��
							case 2:
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://��⿪
										ligner_data[3] = 0x03;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);									
										break;
									case 2://����
										ligner_data[3] = 0x04;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);
										break;
									case 3://���⿪
										ligner_data[3] = 0x01;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);
										break;
									case 4://�����
										ligner_data[3] = 0x02;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);									
										break;
									default:
										break;
								}
								break;
							default:
								break;
						}
					}
					else if(GetUdpData.ucGetUdpData[3] == 0X25 && GetUdpData.ucGetUdpData[4] == 0x02)
					{
					}
					//�̵�������
					else if(GetUdpData.ucGetUdpData[3] == 0X08 && GetUdpData.ucGetUdpData[4] == 0x05)
					{
						switch(GetUdpData.ucGetUdpData[5])
						{
							case 0x01:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//��Դ
								{
									bsp_GpioOn(2);
								}
								else
								{
									bsp_GpioOff(2);
								}
								break;
							case 0x02:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//����ͷ
								{
									bsp_GpioOn(3);
								}
								else
								{
									bsp_GpioOff(3);
								}
								break;
							case 0x03:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//�������
								{
//									/*�ز�ѯ*/
									osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
									osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*�ز�ѯ*/	
									osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
									osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //Ϊ1	/*�رյ����ѯ����*/
									bsp_GpioOn(1);	
//									
//									//tick += usFrequency + 5000;                          
//									//osDelayUntil(tick);	
//									osDelay(5000);
//									/*�������Ѱ������*/
//									ThreadIdTaskMotorFindZero = osThreadNew(AppTaskMotorFindZero, NULL, &ThreadMotorFindZero_Attr);		
//									//tick += usFrequency + 1000;                          
//									//osDelayUntil(tick);							
//									/*�����±ڵ��Ѱ������*/
//									ThreadIdTaskUnderMotorFindZero = osThreadNew(AppTaskUnderMotorFindZero, NULL, &ThreadUnderMotorFindZero_Attr);	
									
								}
								else
								{
									bsp_GpioOff(1);
								}
								break;
							
							default:
								break;
						}
					}
					
				}
			}				
	}
}


/*
*********************************************************************************************************
*	�� �� ��: AppTaskUdpNetWork
*	����˵��: LwIP���ݷ���
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��:   
*********************************************************************************************************
*/
//void AppTaskUDPSend(void *argument)
//{
//	const uint16_t usFrequency = 100;	/* �ӳ����� */
//	unsigned int tick = 0;
//	tick = osKernelGetTickCount(); 	/* ��ȡ��ǰʱ�� */
//	osStatus_t os_Status;
//	struct MOTORDATA getmotordata;
//	unsigned char demo[8] = {1,2,3,4,5,6,7,8};

//	while(1)
//	{
////		os_Status = osMessageQueueGet(msgQueue_MotorData,  /* ��Ϣ����ID 					*/
////								&getmotordata,          							/* �����Ϣ���ڴ� 			*/
////								NULL,				 													/* ��Ϣ���ȼ� 					*/
////								osWaitForever);    										/* ��ʱʱ�䣨һֱ�ȴ��� */
////		if(os_Status == osOK)
////		{
//////			udp_echoclient_send(sendudpdata.ucSendUdPData , 50);
////				
////			  printf("this is AppTaskUDPSend");
////		}
//		
////		udp_echoclient_send(demo , 8);
//				/* ����ӳ� */
//		tick += usFrequency;                          
//		osDelayUntil(tick);	
//	}
//}


