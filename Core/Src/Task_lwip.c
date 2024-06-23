#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: AppTaskEthCheck
*	功能说明: 检查网线插拔状态。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 
*********************************************************************************************************
*/
void AppTaskEthCheck(void *argument)
{
	const uint16_t usFrequency = 250;	/* 延迟周期 */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* 获取当前时间 */
	osStatus_t os_Status;
	unsigned char strct = 0;
	while(1)
	{
		/*DUBUG模式下可以正常识别，但断电后不进入DUBUG模式，无法识别*/
		ethernetif_check_link(&netif);
		/*只有当网线插上后才建立UDP链接*/
		if(netif_is_link_up(&netif)) 
		{
			osEventFlagsClear(event_cable_ID , 0x02U);
			osEventFlagsSet(event_cable_ID , 0x01U);
			//g_ucEthLinkStatus = 1;	
			udp_echoserver_init();
			//关闭此线程
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
		tick += usFrequency;    /* 相对延迟 */                      
		osDelayUntil(tick);	
	}	
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskLwIPPro
*	功能说明: LwIP数据接收处理
*	形    参: 无
*	返 回 值: 无
*   优 先 级:   
*********************************************************************************************************
*/
void AppTaskLwIPPro(void *argument)
{
	const uint16_t usFrequency = 20;	/* 延迟周期 */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* 获取当前时间 */
	osStatus_t os_Status;
	while(1)
	{
		ethernetif_poll(&netif);
		tick += usFrequency;    /* 相对延迟 */                      
		osDelayUntil(tick);	
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUdpNetWork
*	功能说明: LwIP数据接收处理
*	形    参: 无
*	返 回 值: 无
*   优 先 级:   
*********************************************************************************************************
*/
void AppTaskUdpNetWork(void *argument)
{
	const uint16_t usFrequency = 100;	/* 延迟周期 */
	unsigned int tick = 0;
	tick = osKernelGetTickCount(); 	/* 获取当前时间 */
	osStatus_t os_Status;
	struct GETUDPDATA GetUdpData = {0};
	struct MOTORPOSITION setMotorPosition = {0X00002710 , 0x0003D090 , 0x00005A55}; //位置，速度，加速度
	struct MOTORFINDZERO setMotorFindZero = {0x1388 , 0x2710 , 0x7530 , 0x11};			//高速回零，低速回零，回零加速度，回零方式
	struct MOTORSPPEDSCANNING setMotorSppedScanning = {0x0003D090 , 0x00005A55};		//速度，加速度
	
	struct MOTORPOSITION belowsetMotorPosition = {0X00002710 , 0x0003D090 , 0x00005A55}; //位置，速度，加速度
	struct MOTORFINDZERO belowsetMotorFindZero = {0x1388 , 0x2710 , 0x7530 , 0x11};			//高速回零，低速回零，回零加速度，回零方式
	struct MOTORSPPEDSCANNING belowsetMotorSppedScanning = {0x0003D090 , 0x00005A55};		//速度，加速度	
	
	while(1)
	{
		os_Status = osMessageQueueGet(msgQueue_UdpResData,  /* 消息队列ID */
										&GetUdpData,          							/* 存放消息的内存 */
										NULL,						 										/* 消息优先级 */
										osWaitForever);    									/* 超时时间（一直等待） */		
		if(osOK == os_Status)
		{
				if(GetUdpData.ucGetUdpData[0] == 0xAA && GetUdpData.ucGetUdpData[1] == 0xBB && GetUdpData.ucGetUdpData[2] == 0xCC)
				{
					//4 为数据ID
					if(GetUdpData.ucGetUdpData[4] == 0x01 && 
						(GetUdpData.ucGetUdpData[3] == 0x14 || GetUdpData.ucGetUdpData[3] == 0x13 ||
						 GetUdpData.ucGetUdpData[3] == 0x0F))
					{
						switch(GetUdpData.ucGetUdpData[5]) 			//电机编码
						{
							case 1:	//1号电机，即上臂
								switch(GetUdpData.ucGetUdpData[6]) 	//运动模式
								{
									case 1:	//位置模式
										//位置
										setMotorPosition.position = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);
										//速度
										setMotorPosition.spped = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																					| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
										//加速度
										setMotorPosition.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																									| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
										//消息队列
										osMessageQueuePut(msgQueue_PositionData,  /* 消息队列的名字 */
																			&setMotorPosition, 			/* 放入消息队列的变量（消息） */
																			NULL,              			/* 放入消息的优先级 */
																			NULL               			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
																			); 

										osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
										osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //为1	/*关闭电机查询任务*/
										ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);	
										break;
									
									case 2://速度扫描模式
											//速度
											setMotorSppedScanning.spped = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//加速度
											setMotorSppedScanning.acceleration = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);		
											/*消息队列*/
											osMessageQueuePut(msgQueue_SppedScanning,  /* 消息队列的名字 */
																				&setMotorSppedScanning, 			/* 放入消息队列的变量（消息） */
																				NULL,              						/* 放入消息的优先级 */
																				NULL               						/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
																				);
											osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //为1	/*关闭电机查询任务*/
											//启动速度扫描标志位
											event_SppedScanning_ID = osEventFlagsNew(&event_SppedScanning_Attr);									
											//启动速度扫描任务 
											ThreadIdTaskMotorSppedScanning = osThreadNew(AppTaskMotorSppedScanning, NULL, &TaskMotorSppedScanning_Attr);								

											//启动停止模式标志位
										break;
									
									case 3:	//原点模式
											//高速回原点
											setMotorFindZero.sppedhight = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//低速回原点
											setMotorFindZero.sppedlow = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
											//回原点加速度
											setMotorFindZero.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																						| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
											//回原点模式
											setMotorFindZero.findzeromode = GetUdpData.ucGetUdpData[19];
											//查询电机参数标志组
//											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0);
									
											osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); /*关闭电机查询任务*/
											//回零任务 
											ThreadIdTaskMotorFindZero = osThreadNew(AppTaskMotorFindZero, NULL, &ThreadMotorFindZero_Attr);
										break;
									
									case 4: //扫描停止
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 5);
										break;
									
									default:
										break;
								}
								break;
							
							case 2:	//2号电机，即下壁
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://位置模式
											//位置
										belowsetMotorPosition.position = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);
										//速度
										belowsetMotorPosition.spped = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																					| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
										//加速度
										belowsetMotorPosition.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																									| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
										//消息队列
										osMessageQueuePut(msgQueue_UnderPositionData,  /* 消息队列的名字 */
																			&belowsetMotorPosition, 			/* 放入消息队列的变量（消息） */
																			NULL,              			/* 放入消息的优先级 */
																			NULL               			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
																			); 	
										osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
										osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*关查询*/
										ThreadIdTaskUnderMotorPosition = osThreadNew(AppTaskUnderMotorPosition, NULL, &TaskUnderMotorPosition_Attr);

										break;
									
									case 2://速度扫描模式
											//速度
											belowsetMotorSppedScanning.spped = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//加速度
											belowsetMotorSppedScanning.acceleration = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);		
											/*消息队列*/
											osMessageQueuePut(msgQueue_UnderSppedScanning,  /* 消息队列的名字 */
																				&belowsetMotorSppedScanning, 	/* 放入消息队列的变量（消息） */
																				NULL,              						/* 放入消息的优先级 */
																				NULL               						/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
																				);
									
											osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*关查询*/
											//启动速度扫描标志位
											event_belowSppedScanning_ID = osEventFlagsNew(&event_belowSppedScanning_Attr);
											//启动速度扫描任务 
											ThreadIdTaskUnderMotorSppedScanning = osThreadNew(AppTaskUnderMotorSppedScanning, NULL, &TaskUnderMotorSppedScanning_Attr);								

											//启动停止模式标志位									
										break;
								
									case 3://原点模式
											//高速回原点
											belowsetMotorFindZero.sppedhight = GetUdpData.ucGetUdpData[7] | (GetUdpData.ucGetUdpData[8] << 8) 
																						 | (GetUdpData.ucGetUdpData[9] << 16)| (GetUdpData.ucGetUdpData[10] << 24);							
											//低速回原点
											belowsetMotorFindZero.sppedlow = GetUdpData.ucGetUdpData[11] | (GetUdpData.ucGetUdpData[12] << 8) 
																						| (GetUdpData.ucGetUdpData[13] << 16)| (GetUdpData.ucGetUdpData[14] << 24);
											//回原点加速度
											belowsetMotorFindZero.acceleration = GetUdpData.ucGetUdpData[15] | (GetUdpData.ucGetUdpData[16] << 8) 
																						| (GetUdpData.ucGetUdpData[17] << 16)| (GetUdpData.ucGetUdpData[18] << 24);
											//回原点模式
											belowsetMotorFindZero.findzeromode = GetUdpData.ucGetUdpData[19];
											//查询电机参数标志组
//											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0);
											osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
											osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*关查询*/									
											//回零任务 
											ThreadIdTaskUnderMotorFindZero = osThreadNew(AppTaskUnderMotorFindZero, NULL, &ThreadUnderMotorFindZero_Attr);
										break;
									
									case 4: //扫描停止
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
					
					//导引头策略搜索
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
					
					//导引头预置角
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
					{//10长度为精确频率码
//						unsigned char ligner_data[7] = {0xAA, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00};
						unsigned char ligner_data[8] = {0xAA, 0x08, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00};
						switch(GetUdpData.ucGetUdpData[5])
						{//1号
							case 1:
								//精确频率码
								ligner_data[4] = GetUdpData.ucGetUdpData[6];
								ligner_data[5] = GetUdpData.ucGetUdpData[7];
								ligner_data[6] = GetUdpData.ucGetUdpData[8];
								ligner_data[7] = GetUdpData.ucGetUdpData[9];
								BSP_UART_Transmit_IT(&husart2,  ligner_data , 8);	
								//BSP_UART_Transmit_DMA(&husart2, 	ligner_data , 8);
								break;
							case 2://2号
								//精确频率码
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
					{//7长度为激光或红光
						unsigned char ligner_data[4] = {0xAA, 0x04, 0x00, 0x01};
						switch(GetUdpData.ucGetUdpData[5])
						{//1号
							case 1:
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://红光开
										ligner_data[3] = 0x03;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);
										break;
									case 2:	//红光关
										ligner_data[3] = 0x04;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);									
										break;
									case 3:	//激光开
										ligner_data[3] = 0x01;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);	
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);
										break;
									case 4://激光关
										ligner_data[3] = 0x02;
										BSP_UART_Transmit_IT(&husart2,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&husart2,  ligner_data , 4);									
										break;
									default:
										break;
								}
								break;
							//2号
							case 2:
								switch(GetUdpData.ucGetUdpData[6])
								{
									case 1://红光开
										ligner_data[3] = 0x03;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);									
										break;
									case 2://红光关
										ligner_data[3] = 0x04;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);
										break;
									case 3://激光开
										ligner_data[3] = 0x01;
										BSP_UART_Transmit_IT(&huart5,  ligner_data , 4);
										//BSP_UART_Transmit_DMA(&huart5,  ligner_data , 4);
										break;
									case 4://激光关
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
					//继电器控制
					else if(GetUdpData.ucGetUdpData[3] == 0X08 && GetUdpData.ucGetUdpData[4] == 0x05)
					{
						switch(GetUdpData.ucGetUdpData[5])
						{
							case 0x01:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//光源
								{
									bsp_GpioOn(2);
								}
								else
								{
									bsp_GpioOff(2);
								}
								break;
							case 0x02:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//导引头
								{
									bsp_GpioOn(3);
								}
								else
								{
									bsp_GpioOff(3);
								}
								break;
							case 0x03:
								if(GetUdpData.ucGetUdpData[6] == 0x01)//雷赛电机
								{
//									/*关查询*/
									osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000002U);
									osEventFlagsSet(event_belowclosurefinddata_ID , 0x01U << 0); /*关查询*/	
									osEventFlagsClear(event_closurefinddata_ID, 0x00000002U);
									osEventFlagsSet(event_closurefinddata_ID , 0x01U << 0); //为1	/*关闭电机查询任务*/
									bsp_GpioOn(1);	
//									
//									//tick += usFrequency + 5000;                          
//									//osDelayUntil(tick);	
//									osDelay(5000);
//									/*启动电机寻零任务*/
//									ThreadIdTaskMotorFindZero = osThreadNew(AppTaskMotorFindZero, NULL, &ThreadMotorFindZero_Attr);		
//									//tick += usFrequency + 1000;                          
//									//osDelayUntil(tick);							
//									/*启动下壁电机寻零任务*/
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
*	函 数 名: AppTaskUdpNetWork
*	功能说明: LwIP数据发送
*	形    参: 无
*	返 回 值: 无
*   优 先 级:   
*********************************************************************************************************
*/
//void AppTaskUDPSend(void *argument)
//{
//	const uint16_t usFrequency = 100;	/* 延迟周期 */
//	unsigned int tick = 0;
//	tick = osKernelGetTickCount(); 	/* 获取当前时间 */
//	osStatus_t os_Status;
//	struct MOTORDATA getmotordata;
//	unsigned char demo[8] = {1,2,3,4,5,6,7,8};

//	while(1)
//	{
////		os_Status = osMessageQueueGet(msgQueue_MotorData,  /* 消息队列ID 					*/
////								&getmotordata,          							/* 存放消息的内存 			*/
////								NULL,				 													/* 消息优先级 					*/
////								osWaitForever);    										/* 超时时间（一直等待） */
////		if(os_Status == osOK)
////		{
//////			udp_echoclient_send(sendudpdata.ucSendUdPData , 50);
////				
////			  printf("this is AppTaskUDPSend");
////		}
//		
////		udp_echoclient_send(demo , 8);
//				/* 相对延迟 */
//		tick += usFrequency;                          
//		osDelayUntil(tick);	
//	}
//}


