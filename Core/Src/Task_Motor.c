#include "bsp.h"

int calculateTwosComplement(int input) 
{
    // 计算输入参数的补码形式的负数
    int twosComplement = -input;
    return twosComplement;
}



static unsigned char Positiondata[11][8] = { 
 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //速度
 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //加速度
 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //位置
 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}
};




/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorSppedScanning
*	功能说明: 电机 速度扫描模式
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskMotorSppedScanning(void *argument)
{
	unsigned char clockwiseSpped[8]   = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//顺时针速度
	unsigned char Counterclockwise[8] = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//逆时针速度
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
	const uint16_t usFrequency = 150; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount();	
	uint32_t flags = 0;
	static uint32_t msg_Number = 0;
	
	/*获取消息队列数据*/
	msg_Number = osMessageQueueGetCount(msgQueue_SppedScanning);
	if(msg_Number >= 1)
	{
		os_Status = osMessageQueueGet(msgQueue_SppedScanning, &getMotorsppedscanning , NULL ,NULL);
		if(os_Status == osOK)
		{
			/*速度，加速度，数据拷贝*/
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
	/*第一遍运动 ，其实无所谓正方向还是反方向，反正都会触发限位*/
	for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
	
	/*开查询*/
	osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);			

	while(1)
	{
//	os_Status = osEventFlagsWait(event_SppedScanning_ID , 0x00000017U , osFlagsWaitAny , osWaitForever);
		os_Status = osEventFlagsWait(event_SppedScanning_ID , 0x00000077U , osFlagsWaitAny , osWaitForever);
		//if(os_Status == 0x12 || os_Status == 0x02)//第三种状态，停止，右触发  0x01U << 4   0x01U << 1
		if(os_Status == 0x02)//第三种状态，停止，右触发  0x01U << 4   0x01U << 1
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
//		else if(os_Status == 0x11 || os_Status == 0x01)//第二种状态，停止，左触发  0x01U << 4   0x01U << 0  11;
		else if(os_Status == 0x01)
		{
			printf("this is 2");
			for(unsigned char uci = 4; uci <= 7 ; uci++)//右运动
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
		else if(os_Status == 0x04)//第一种状态，运动，未触发 ， 0x01U << 2 C
		{
			printf("this is 1");
		}
		else if(os_Status == 0x52 || os_Status == 0x42)//卡在运动，但速度0
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
		else if(os_Status == 0x51 || os_Status == 0x41)//卡在运动，但速度0
		{
			//无法判别方向
			for(unsigned char uci = 4; uci <= 7 ; uci++)//右运动
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
		/*任何状态下收到停止指令都停止*/
		else if(os_Status == 0x22 || os_Status == 0x21 || os_Status == 0x24 || os_Status == 0x20 ||
						os_Status == 0x72 || os_Status == 0x62 || os_Status == 0x71 || os_Status == 0x61 )
		{
			//ThreadIdTaskMotorStop = osThreadNew(AppTaskMotorStop, NULL, &TaskMotorSppedScanning_Attr);	
			/*若收到停止指令，则结束任务*/
			osThreadExit();		
		}
//		else if(os_Status == 0x24 || os_Status == 0x34)
//		{
//			/*若收到停止指令，则结束任务*/
//			osThreadExit();
//		}
//		tick += usFrequency;                 
//		osDelayUntil(tick);
	}
}




/*
*********************************************************************************************************
*	函 数 名: AppTaskUnderMotorSppedScanning
*	功能说明: 下壁电机 速度扫描模式
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskUnderMotorSppedScanning(void *argument)
{
	unsigned char clockwiseSpped[8]   = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//顺时针速度
	unsigned char Counterclockwise[8] = {0x23 , 0xFF , 0x60 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00};	//逆时针速度
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
	const uint16_t usFrequency = 150; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount();	
	uint32_t flags = 0;
	static uint32_t msg_Number = 0;
	/*获取消息队列数据*/
	msg_Number = osMessageQueueGetCount(msgQueue_UnderSppedScanning);
	if(msg_Number >= 1)
	{
		os_Status = osMessageQueueGet(msgQueue_UnderSppedScanning, &getMotorsppedscanning , NULL ,NULL);
		if(os_Status == osOK)
		{
			/*速度，加速度，数据拷贝*/
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
	/*第一遍运动 ，其实无所谓正方向还是反方向，反正都会触发限位*/
	for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
	osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*开查询*/
	
	while(1)
	{
		os_Status = osEventFlagsWait(event_belowSppedScanning_ID , 0x00000077U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x12 || os_Status == 0x02)//第三种状态，停止，右触发  0x01U << 4   0x01U << 1
		if(os_Status == 0x2)//第三种状态，停止，右触发  0x01U << 4   0x01U << 1
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
//		else if(os_Status == 0x11 || os_Status == 0x01)//第二种状态，停止，左触发  0x01U << 4   0x01U << 0  11;
		else if(os_Status == 0x1)//第二种状态，停止，左触发  0x01U << 4   0x01U << 0  11;
		{
			printf("this is 2");
			for(unsigned char uci = 4; uci <= 7 ; uci++)//右运动
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
		else if(os_Status == 0x04)//第一种状态，运动，未触发 ， 0x01U << 2 C
		{
			//printf("this is 1");
		}
		else if(os_Status == 0x52 || os_Status == 0x42)//卡在运动，但速度0
		{
			for(unsigned char uci = 4; uci <= 7 ; uci++)//左运动
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
			//无法判别方向
			for(unsigned char uci = 4; uci <= 7 ; uci++)//右运动
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
		/*任何状态下收到停止指令都停止*/
		/*任何状态下收到停止指令都停止*/
		else if(os_Status == 0x22 || os_Status == 0x21 || os_Status == 0x24 || os_Status == 0x20 ||
						os_Status == 0x72 || os_Status == 0x62 || os_Status == 0x71 || os_Status == 0x61 )
		{
			//ThreadIdTaskUnderMotorStop = osThreadNew(AppTaskUnderMotorStop, NULL, &TaskUnderMotorSppedScanning_Attr);	
			/*若收到停止指令，则结束任务*/
			osThreadExit();		
		}
		
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorFindZero
*	功能说明: 电机数据查询任务，轮询发送 CANOPEN 电机参数查询数据
*	形    参: 
*	返 回 值: 无
* 优 先 级:  
*********************************************************************************************************
*/
void AppTaskMotorDataquery(void *argument)
{
//	osStatus_t os_Status;
//	uint32_t flags = 0;
//	const uint16_t usFrequency = 10; /* 延迟周期 */
//	uint32_t tick;	
//	tick = osKernelGetTickCount();
//	/*数据查询任务*/
//	
//	/*1.回零方式*/
//	unsigned char Zerofindway[8] = {0x40, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*2.位置指令*/
//	unsigned char PositionCommand[8] = {0x40, 0x62, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*3.实际位置*/
//	unsigned char PositionReal[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*4.操作模式*/
//	unsigned char OperateMode[8] = {0x40, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*5.错误码查询*/
//	unsigned char ErrorCode[8] = {0x40, 0x3F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*6.速度指令*/
//	unsigned char SppedCommand[8] = {0x40, 0x6B, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*7.实际速度*/		
//	unsigned char SppedReal[8] = {0x40, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*8.状态字*/
//	unsigned char StateWord[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*S1-S17检查*/
//	unsigned char ClickS1_S17[8] = {0x40, 0xFD, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	
//	unsigned char ucloop = 0;
//	unsigned char ucloop_2 = 0;
//	/*开始轮旋发送数据*/	
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
//	while(1)
//	{
//		switch(ucloop)
//		{
//			case 0:
//				ucloop = 1;	//回零方式
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
//				break;

//			case 1:
//				ucloop = 2; //位置指令
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
//				break;

//			case 2:
//				ucloop = 3; //实际位置
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
//				break;

//			case 3:
//				ucloop = 4;	//操作模式
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
//				break;

//			case 4:
//				ucloop = 5; //错误代码
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
//				break;

//			case 5:
//				ucloop = 6;	//速度指令
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
//				break;

//			case 6:
//				ucloop = 7;	//实际速度
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
//				break;		

//			case 7:
//				ucloop = 8;//状态字
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
//				break;
//			
//			case 8:
//				ucloop = 0;//S1-S14状态查询
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
//				break;			

//			default:
//				break;
//		}
//		
//		/*如果用户需要自己回零则需要在这里关闭这个线程，以防止限位开关干扰*/
////		flags = osEventFlagsGet(event_closurefinddata_ID);
////		if(flags == 0x01)
////		{
////			osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
////			/*线程退出*/
////			osThreadExit();
////		}
//		
//		tick += usFrequency; //大约5s后到达负极限点                  
//		osDelayUntil(tick);
//	}		
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUnderMotorDataquery
*	功能说明: 下壁电机数据查询任务，轮询发送 CANOPEN 电机参数查询数据
*	形    参: 
*	返 回 值: 无
* 优 先 级:  
*********************************************************************************************************
*/
void AppTaskUnderMotorDataquery(void *argument)
{
//	osStatus_t os_Status;
//	uint32_t flags = 0;
//	const uint16_t usFrequency = 10; /* 延迟周期 */
//	uint32_t tick;	
//	tick = osKernelGetTickCount();
//	/*数据查询任务*/
//	
//	/*1.回零方式*/
//	unsigned char Zerofindway[8] = {0x40, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*2.位置指令*/
//	unsigned char PositionCommand[8] = {0x40, 0x62, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*3.实际位置*/
//	unsigned char PositionReal[8] = {0x40, 0x64, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*4.操作模式*/
//	unsigned char OperateMode[8] = {0x40, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*5.错误码查询*/
//	unsigned char ErrorCode[8] = {0x40, 0x3F, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*6.速度指令*/
//	unsigned char SppedCommand[8] = {0x40, 0x6B, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*7.实际速度*/		
//	unsigned char SppedReal[8] = {0x40, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*8.状态字*/
//	unsigned char StateWord[8] = {0x40, 0x41, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	/*S1-S17检查*/
//	unsigned char ClickS1_S17[8] = {0x40, 0xFD, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
//	
//	unsigned char ucloop = 0;
//	/*开始轮旋发送数据*/	
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };

//	while(1)
//	{
//		switch(ucloop)
//		{
//			case 0:
//				ucloop = 1;	//回零方式
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &Zerofindway); 
//				break;

//			case 1:
//				ucloop = 2; //位置指令
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionCommand);
//				break;

//			case 2:
//				ucloop = 3; //实际位置
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &PositionReal);
//				break;

//			case 3:
//				ucloop = 4;	//操作模式
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &OperateMode);
//				break;

//			case 4:
//				ucloop = 5; //错误代码
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ErrorCode);
//				break;

//			case 5:
//				ucloop = 6;	//速度指令
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedCommand);
//				break;

//			case 6:
//				ucloop = 7;	//实际速度
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &SppedReal);
//				break;		

//			case 7:
//				ucloop = 8;//状态字
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &StateWord);
//				break;
//			
//			case 8:
//				ucloop = 0;//S1-S14状态查询
//				CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &ClickS1_S17);
//				break;			

//			default:
//				break;
//		}		
//		/*如果用户需要自己回零则需要在这里关闭这个线程，以防止限位开关干扰*/
////		flags = osEventFlagsGet(event_belowclosurefinddata_ID);
////		if(flags == 0x01)
////		{
////			osEventFlagsClear(event_belowclosurefinddata_ID, 0x00000001U);
////			/*线程退出*/
////			osThreadExit();
////		}
//		tick += usFrequency; //大约5s后到达负极限点                  
//		osDelayUntil(tick);
//	}		
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorPosition
*	功能说明: 电机 定位模式
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskMotorPosition(void *argument)
{
	osStatus_t os_Status; 
	const uint16_t usFrequency = 100; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount();
	struct MOTORPOSITION getMotorPosition = {0 , 0 , 0};
	
//	unsigned char Positiondata[11][8] = { 
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
//																		 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //速度
//																		 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //加速度
//																		 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //位置
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x602 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	
	while(1)
	{
			os_Status = osMessageQueueGet(msgQueue_PositionData, /* 消息队列ID */
																		&getMotorPosition,     /* 存放消息的内存 */
																		NULL,				 					 /* 消息优先级 */
																		osWaitForever);    		 /* 超时时间（一直等待） */
			if(osOK == os_Status)
			{
					//首先赋值速度
					Positiondata[4][4] = getMotorPosition.spped;			//最低位
					Positiondata[4][5] = getMotorPosition.spped >> 8;
					Positiondata[4][6] = getMotorPosition.spped >> 16;
					Positiondata[4][7] = getMotorPosition.spped >> 24;//最高位
				
					//赋值加速度
					Positiondata[5][4] = getMotorPosition.acceleration;			//最低位
					Positiondata[5][5] = getMotorPosition.acceleration >> 8;
					Positiondata[5][6] = getMotorPosition.acceleration >> 16;
					Positiondata[5][7] = getMotorPosition.acceleration >> 24;//最高位			

					//赋值位置
					Positiondata[6][4] = getMotorPosition.position;			//最低位
					Positiondata[6][5] = getMotorPosition.position >> 8;
					Positiondata[6][6] = getMotorPosition.position >> 16;
					Positiondata[6][7] = getMotorPosition.position >> 24;//最高位		
					/*发送中值指令*/
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
					osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);		//为2		
					/*退出线程*/
					osThreadExit();					
			}
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorPosition
*	功能说明: 下壁电机 定位模式
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskUnderMotorPosition(void *argument)
{
	osStatus_t os_Status; 
	const uint16_t usFrequency = 50; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount();
	struct MOTORPOSITION getMotorPosition = {0 , 0 , 0};
	
//	unsigned char Positiondata[11][8] = { 
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2F , 0x60 , 0x60 , 0x00 , 0x01 , 0x00 , 0x00 , 0x00} ,
//																		 {0x23 , 0x81 , 0x60 , 0x00 , 0x88 , 0x13 , 0x00 , 0x00} ,  //速度
//																		 {0x23 , 0x83 , 0x60 , 0x00 , 0x90 , 0xD3 , 0x03 , 0x00} ,  //加速度
//																		 {0x23 , 0x7A , 0x60 , 0x00 , 0x55 , 0x5A , 0x00 , 0x00} ,  //位置
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
//																		 {0x2B , 0x40 , 0x60 , 0x00 , 0x5F , 0x00 , 0x00 , 0x00}};
	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
	
	while(1)
	{
			os_Status = osMessageQueueGet(msgQueue_UnderPositionData, /* 消息队列ID */
																		&getMotorPosition,     /* 存放消息的内存 */
																		NULL,				 					 /* 消息优先级 */
																		osWaitForever);    		 /* 超时时间（一直等待） */
			if(osOK == os_Status)
			{
					//首先赋值速度
					Positiondata[4][4] = getMotorPosition.spped;			//最低位
					Positiondata[4][5] = getMotorPosition.spped >> 8;
					Positiondata[4][6] = getMotorPosition.spped >> 16;
					Positiondata[4][7] = getMotorPosition.spped >> 24;//最高位
				
					//赋值加速度
					Positiondata[5][4] = getMotorPosition.acceleration;			//最低位
					Positiondata[5][5] = getMotorPosition.acceleration >> 8;
					Positiondata[5][6] = getMotorPosition.acceleration >> 16;
					Positiondata[5][7] = getMotorPosition.acceleration >> 24;//最高位			

					//赋值位置
					Positiondata[6][4] = getMotorPosition.position;			//最低位
					Positiondata[6][5] = getMotorPosition.position >> 8;
					Positiondata[6][6] = getMotorPosition.position >> 16;
					Positiondata[6][7] = getMotorPosition.position >> 24;//最高位		
					/*发送中值指令*/
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
					osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*开查询*/
					/*退出线程*/
					osThreadExit();					
			}
	}
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorFindZero
*	功能说明: 电机回零任务，我一直在思考回零这个需不需要让用户设置参数
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskMotorFindZero(void *argument)
{
	osStatus_t os_Status;
	osStatus_t status; 
	const uint16_t usFrequency = 50; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount(); 
	static unsigned char findzerodata[11][8] = {
	 {0x81 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x01 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
	 {0x2F , 0x60 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x23 , 0x99 , 0x60 , 0x01 , 0x88 , 0x13 , 0x00 , 0x00} , 	//回原点速度 高速 5000
	 {0x23 , 0x99 , 0x60 , 0x02 , 0xE8 , 0x03 , 0x00 , 0x00} ,	//回原点速度 低速 1000
	 {0x23 , 0x9A , 0x60 , 0x00 , 0x30 , 0x75 , 0x00 , 0x00} ,	//回原点加速度 30,000
	 {0x2F , 0x98 , 0x60 , 0x00 , 0x11 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x1F , 0x00 , 0x00 , 0x00}
	};
//	unsigned int findzeroH = 0X00002710; //10000
//	unsigned int findzeroL = 0X000003E8; //5000L
	findzerodata[0][1] = 0x02;
	findzerodata[1][1] = 0x02;		
	/*经过验证这个速度不可以取过大，若过大会影响回零性能*/
	unsigned int findzeroH = 0X00001388; 
	unsigned int findzeroL = 0X000003E8; 
	
	findzerodata[6][4] = findzeroH;				//最低位
	findzerodata[6][5] = findzeroH >> 8;
	findzerodata[6][6] = findzeroH >> 16;
	findzerodata[6][7] = findzeroH >> 24;	//最高位
	
	findzerodata[7][4] = findzeroL;				//最低位
	findzerodata[7][5] = findzeroL >> 8;
	findzerodata[7][6] = findzeroL >> 16;
	findzerodata[7][7] = findzeroL >> 24;	//最高位	
	
	/*适当延时才可以启动回零，否则发送回零无法相应*/
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
	
	/*开查询*/					
	osEventFlagsClear(event_closurefinddata_ID, 0x00000001U);
	osEventFlagsSet(event_closurefinddata_ID ,  0x01U << 1);	
	
	osThreadExit();//自己结束

	/*让电机运动2S后再去发送查询信息，以防止电机卡在正极限点，触发限位标志 ，不能放到启动任务种会影响回零*/	
	/*电机数据查询任务*/						
//	ThreadIdTaskMotorDataquery = osThreadNew(AppTaskMotorDataquery, NULL, &TaskMotorDataquery_Attr);	
//	while(1)
//	{
		/*线程同步*/
//		os_Status = osEventFlagsWait(event_negativelimit_ID , 0x00000003U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x01)//左
//		{			
			/*启动消息队列 				位置信息 23125  0x00005A55 ,
														速度		 10000  0X00002710 ,
													  加速度   250000 0x0003D090 ,
			*/
//		struct MOTORPOSITION setMotorPosition = {0x00005A55 , 0X00002710 , 0x0003D090};	
//		osMessageQueuePut(msgQueue_PositionData,  /* 消息队列的名字 */
//											&setMotorPosition, 			/* 放入消息队列的变量（消息） */
//											NULL,              			/* 放入消息的优先级 */
//											NULL               			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
//											); 	
//		osEventFlagsClear(event_negativelimit_ID, 0x0000000FU);
//		ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);		
			/*退出线程*/
//			osThreadExit();
//		}
//		else if(os_Status == 0x02)//右
//		{
//			printf("this is right");
			
//			/*启动消息队列 				位置信息 23125  0x00005A55 ,
//															速度		 10000  0X00002710 ,
//															加速度   250000 0x0003D090 ,
//			*/
//			struct MOTORPOSITION setMotorPosition = {0x00005A55 , 0X00002710 , 0x0003D090};	
//			osMessageQueuePut(msgQueue_PositionData,  /* 消息队列的名字 */
//												&setMotorPosition, 			/* 放入消息队列的变量（消息） */
//												NULL,              			/* 放入消息的优先级 */
//												NULL               			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
//												); 	
//			osEventFlagsClear(event_negativelimit_ID, 0x0000000FU);
//			ThreadIdTaskMotorPosition = osThreadNew(AppTaskMotorPosition, NULL, &TaskMotorPosition_Attr);	
//			/*退出线程*/
//				osThreadExit();			
//		}
//	}
}



/*
*********************************************************************************************************
*	函 数 名: AppTaskUnderMotorFindZero
*	功能说明: 下壁电机寻零任务
*	形    参: 
*	返 回 值: 无
*   优 先 级:  
*********************************************************************************************************
*/
void AppTaskUnderMotorFindZero(void *argument)
{
	osStatus_t os_Status;
	const uint16_t usFrequency = 100; /* 延迟周期 */
	uint32_t tick;	
	tick = osKernelGetTickCount(); 
	/*下壁电机编号为4*/
	unsigned char findzerodata[11][8] = {
	 {0x81 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x01 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x07 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x0F , 0x00 , 0x00 , 0x00} ,
	 {0x2F , 0x60 , 0x60 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00} ,
	 {0x23 , 0x99 , 0x60 , 0x01 , 0x88 , 0x13 , 0x00 , 0x00} , 	//回原点速度 高速 5000
	 {0x23 , 0x99 , 0x60 , 0x02 , 0xE8 , 0x03 , 0x00 , 0x00} ,	//回原点速度 低速 1000
	 {0x23 , 0x9A , 0x60 , 0x00 , 0x30 , 0x75 , 0x00 , 0x00} ,	//回原点加速度 30,000
	 {0x2F , 0x98 , 0x60 , 0x00 , 0x11 , 0x00 , 0x00 , 0x00} ,
	 {0x2B , 0x40 , 0x60 , 0x00 , 0x1F , 0x00 , 0x00 , 0x00}
	};
//	unsigned int findzeroH = 0X00002710; //10000
//	unsigned int findzeroL = 0X000003E8; //5000L
	
	findzerodata[0][1] = 0x04;
	findzerodata[1][1] = 0x04;	
	
	/*经过验证这个速度不可以取过大，若过大会影响回零性能*/
	unsigned int findzeroH = 0X00001388; 
	unsigned int findzeroL = 0X000003E8; 
	
	findzerodata[6][4] = findzeroH;				//最低位
	findzerodata[6][5] = findzeroH >> 8;
	findzerodata[6][6] = findzeroH >> 16;
	findzerodata[6][7] = findzeroH >> 24;	//最高位
	
	findzerodata[7][4] = findzeroL;				//最低位
	findzerodata[7][5] = findzeroL >> 8;
	findzerodata[7][6] = findzeroL >> 16;
	findzerodata[7][7] = findzeroL >> 24;	//最高位	
	
	/*适当延时才可以启动回零，否则发送回零无法相应*/
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
	osEventFlagsSet(event_belowclosurefinddata_ID ,  0x01U << 1);		/*开查询*/
	
	osThreadExit();//自己结束

	/*让电机运动2S后再去发送查询信息，以防止电机卡在正极限点，触发限位标志 ，不能放到启动任务种会影响回零*/	
	/*电机数据查询任务*/						
	//ThreadIdTaskUnderMotorDataquery = osThreadNew(AppTaskUnderMotorDataquery, NULL, &TaskUnderMotorDataquery_Attr);	
//	while(1)
//	{
		/*线程同步*/
		//os_Status = osEventFlagsWait(event_belownegativelimit_ID , 0x00000003U , osFlagsWaitAny , osWaitForever);
//		if(os_Status == 0x01)//左
//		{			
			/*退出线程*/
//			osThreadExit();
//		}
//		else if(os_Status == 0x02)//右
//		{
//			printf("this is right");
			
				/*启动消息队列 				位置信息 23125  0x00005A55 ,
															速度		 10000  0X00002710 ,
															加速度   250000 0x0003D090 ,
				*/
//			struct MOTORPOSITION setMotorPosition = {0xFFFFA5AB , 0X00002710 , 0x0003D090};	
//			osMessageQueuePut(msgQueue_UnderPositionData,  /* 消息队列的名字 */
//												&setMotorPosition, 			/* 放入消息队列的变量（消息） */
//												NULL,              			/* 放入消息的优先级 */
//												NULL               			/* 超时时间为0，如果osMessageQueuePut被中断回调函数调用时，此参数必须是NULL */
//												); 	
//			osEventFlagsClear(event_belownegativelimit_ID, 0x0000000FU);
			
			/*退出线程*/
//			osThreadExit();
			
//		}
//	}	
}


/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorSpped
*	功能说明: 电机速度模式任务,暂时好像用不上，先不写了
*	形    参: 
*	返 回 值: 无
* 优 先 级:  
*********************************************************************************************************
*/
void AppTaskMotorSpped(void *argument)
{}

/*
*********************************************************************************************************
*	函 数 名: AppTaskMotorStop
*	功能说明: 电机停止任务
*	形    参: 
*	返 回 值: 无
* 优 先 级:  
*********************************************************************************************************
*/
//void AppTaskMotorStop(void *argument)
//{
//	osStatus_t os_Status;
//	const uint16_t usFrequency = 100; /* 延迟周期 */
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
//	const uint16_t usFrequency = 100; /* 延迟周期 */
//	uint32_t tick;	
//	tick = osKernelGetTickCount(); 
//	unsigned char stopdata[] = {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00};
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x604 , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };	
//	CAN_Send_Mesage(CAN1_Handler , &CanTxHeadType , &stopdata);	
//	osThreadExit();
//}
