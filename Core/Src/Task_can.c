#include "bsp.h"

unsigned char getHeartbeat(unsigned char getdata);

/*
*********************************************************************************************************
*	函 数 名: AppTaskCan1_answer
*	功能说明: CAN1应答解包函数
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
void AppTaskCan1_answer(void *argument)
{	
	uint16_t usFrequency = 50; /* 延迟周期 */
	uint32_t tick;
	/* 获取当前时间 */
	tick = osKernelGetTickCount();
  osStatus_t os_Status; 
	struct CAN_RESDATA can1SED = {0};
	struct SendUDPData sendudpdata = {{0} , 0};
//	struct SendUDPData sendudpdata_num2 = {{0} , 0};
	sendudpdata.ucSendUdPData[0] = 0x55;
	sendudpdata.ucSendUdPData[1] = 0xAA;
	sendudpdata.ucSendUdPData[2] = 0x32; //50字节
	sendudpdata.ucSendUdpLen = 3;
	struct MOTORDATA getmotordata;
  while(1)
  {		
		os_Status = osMessageQueueGet(msgQueue_Can1ResData,  	/* 消息队列ID 					*/
									  &can1SED,          										/* 存放消息的内存 			*/
									  NULL,				 													/* 消息优先级 					*/
									  osWaitForever);    										/* 超时时间（一直等待） */
	    if(osOK == os_Status)
			{
					sendudpdata.ucSendUdPData[3] = 0x01; //1号电机
					sendudpdata.ucSendUdpLen++;
					if(can1SED.resStdID == 0x0702 && can1SED.can_resbuflen == 1){//1号节点 NMT节点保护值
						sendudpdata.ucSendUdPData[4] = getHeartbeat(can1SED.can_resbuf[0]);
						sendudpdata.ucSendUdpLen++;
					}
					else if(can1SED.resStdID == 0x0704 && can1SED.can_resbuflen == 1){//2号节点 NMT节点保护值
						sendudpdata.ucSendUdPData[27] = getHeartbeat(can1SED.can_resbuf[0]);
						sendudpdata.ucSendUdpLen++;
					}
					/*上臂电机*/
					else if(can1SED.resStdID == 0x0582 && can1SED.can_resbuflen == 8){//头一个字节为数据长度
						switch(can1SED.can_resbuf[0]){
							case 0x4F: //1字节
								if(can1SED.can_resbuf[1] == 0x98 && can1SED.can_resbuf[2] == 0x60){//回零
									sendudpdata.ucSendUdPData[5] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdpLen++;
								}
								else if(can1SED.can_resbuf[1] == 0x60 && can1SED.can_resbuf[2] == 0x60){//操作模式
									sendudpdata.ucSendUdPData[6] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdpLen++;
								}
								break;
							
							case 0x4B: //2字节
								if(can1SED.can_resbuf[1] == 0x3F && can1SED.can_resbuf[2] == 0x60){//2字节,错误码查询
									sendudpdata.ucSendUdPData[7] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[8] = can1SED.can_resbuf[5];
									sendudpdata.ucSendUdpLen += 2;
								}
								else if(can1SED.can_resbuf[1] == 0x41 && can1SED.can_resbuf[2] == 0x60){//状态字查询
									sendudpdata.ucSendUdPData[25] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[26] = can1SED.can_resbuf[5];	
									sendudpdata.ucSendUdpLen += 2;
									unsigned char bit[8] = {0};									
									//状态字解析
									for (int i = 7; i >= 0; i--) {
										bit[i] = (can1SED.can_resbuf[5] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}
									if(bit[0] == 0x01){//非正常启停
									}
									if(bit[1] == 0x01){//远程
									}
									if(bit[2] == 0x01){//位置到达
									}
									if(bit[3] == 0x01){
									}
									if(bit[4] == 0x01){
									}
									if(bit[5] == 0x01){//误差过大
									}
									if(bit[6] == 0x01){
									}
									if(bit[7] == 0x01){
									}							
								}
								break;
							
							case 0x47: //3字节，暂无
								break;
							
							case 0x43: //4字节
								if(can1SED.can_resbuf[1] == 0x62 && can1SED.can_resbuf[2] == 0x60){//位置指令
									sendudpdata.ucSendUdPData[9]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[10] = can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[11] = can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[12] = can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;
								}
								else if(can1SED.can_resbuf[1] == 0x64 && can1SED.can_resbuf[2] == 0x60){//实际位置
									sendudpdata.ucSendUdPData[13]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[14] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[15] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[16] =  can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;
								}
								else if(can1SED.can_resbuf[1] == 0x6B && can1SED.can_resbuf[2] == 0x60){//速度指令
									sendudpdata.ucSendUdPData[17]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[18] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[19] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[20] =  can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;
								}
								else if(can1SED.can_resbuf[1] == 0x6C && can1SED.can_resbuf[2] == 0x60){//速度实际						
									sendudpdata.ucSendUdPData[21]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[22] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[23] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[24] =  can1SED.can_resbuf[7];	
									sendudpdata.ucSendUdpLen += 4;
									if((sendudpdata.ucSendUdPData[21] + sendudpdata.ucSendUdPData[22] + sendudpdata.ucSendUdPData[23] + sendudpdata.ucSendUdPData[24]) <= 0x02){//证明到了 电机已经停止了
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 6);//用于速度扫描标志判定
									}
								}
								else if(can1SED.can_resbuf[1] == 0xFD && can1SED.can_resbuf[2] == 0x60){
									//分三种情况，左触发，右触发，未触发
									unsigned char getSBit[8] = {0};
									for (int i = 7; i >= 0; i--) {
										getSBit[i] = (can1SED.can_resbuf[4] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}
									unsigned char getSBit_1[8] = {0};
									for (int i = 7; i >= 0; i--){
										getSBit_1[i] = (can1SED.can_resbuf[7] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}									
									if(getSBit[4] == 1 && getSBit[5] == 1){//未触发
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 2);//用于速度扫描标志判定
									}
									else if(getSBit[4] == 0 && getSBit[5] == 1){//左触发
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 0);//用于速度扫描标志判定
									}
									else if(getSBit[4] == 1 && getSBit[5] == 0){//右触发				
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 1);//用于速度扫描标志判定
									}
									else{
									}
									if(getSBit_1[0] == 1 && getSBit_1[1] == 1){//停止
										//osEventFlagsSet(event_negativelimit_ID , 0x01U << 2);
										osEventFlagsSet(event_SppedScanning_ID , 0x01U << 4);
									}
									else if(getSBit_1[0] == 0 && getSBit_1[1] == 1){//运动中
										//osEventFlagsSet(event_SppedScanning_ID , 0x01U << 3);
									}
									else if(getSBit_1[0] == 0 && getSBit_1[1] == 0){//未初始化
									}
								}
								break;
							
							default:
								break;
						}
					}
					/* 下壁电机 */
					else if(can1SED.resStdID == 0x0584 && can1SED.can_resbuflen == 8){
						switch(can1SED.can_resbuf[0]){
							case 0x4F: //1字节
							if(can1SED.can_resbuf[1] == 0x98 && can1SED.can_resbuf[2] == 0x60){	
								sendudpdata.ucSendUdPData[28] = can1SED.can_resbuf[4];//回零
								sendudpdata.ucSendUdpLen++;
							}
							else if(can1SED.can_resbuf[1] == 0x60 && can1SED.can_resbuf[2] == 0x60){
								//操作模式
								sendudpdata.ucSendUdPData[29] = can1SED.can_resbuf[4];
								sendudpdata.ucSendUdpLen++;
							}
							break;
							
							case 0x4B: //2字节,错误码查询
								if(can1SED.can_resbuf[1] == 0x3F && can1SED.can_resbuf[2] == 0x60)
								{
									sendudpdata.ucSendUdPData[30] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[31] = can1SED.can_resbuf[5];
									sendudpdata.ucSendUdpLen += 2;
								}
								else if(can1SED.can_resbuf[1] == 0x41 && can1SED.can_resbuf[2] == 0x60)
								{//状态字查询
									sendudpdata.ucSendUdPData[48] = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[49] = can1SED.can_resbuf[5];
									sendudpdata.ucSendUdpLen += 2;									
									unsigned char bit[8] = {0};									
									//状态字解析
									for (int i = 7; i >= 0; i--) {
										bit[i] = (can1SED.can_resbuf[5] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}
									if(bit[0] == 0x01){//非正常启停
									}
									if(bit[1] == 0x01){//远程
									}
									if(bit[2] == 0x01){//位置到达
									}
									if(bit[3] == 0x01){
									}
									if(bit[4] == 0x01){
									}
									if(bit[5] == 0x01){//误差过大
									}
									if(bit[6] == 0x01){
									}
									if(bit[7] == 0x01){
									}							
								}
								break;
								
								case 0x47: //3字节
									break;
								
								case 0x43: //4字节
								if(can1SED.can_resbuf[1] == 0x62 && can1SED.can_resbuf[2] == 0x60){//位置指令
									sendudpdata.ucSendUdPData[32]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[33] = can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[34] = can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[35] = can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;	
								}
								else if(can1SED.can_resbuf[1] == 0x64 && can1SED.can_resbuf[2] == 0x60){//实际位置
									sendudpdata.ucSendUdPData[36]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[37] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[38] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[39] =  can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;	
								}
								else if(can1SED.can_resbuf[1] == 0x6B && can1SED.can_resbuf[2] == 0x60){//速度指令
									sendudpdata.ucSendUdPData[40]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[41] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[42] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[43] =  can1SED.can_resbuf[7];
									sendudpdata.ucSendUdpLen += 4;	
								}
								else if(can1SED.can_resbuf[1] == 0x6C && can1SED.can_resbuf[2] == 0x60){
									//速度实际									
									sendudpdata.ucSendUdPData[44]  = can1SED.can_resbuf[4];
									sendudpdata.ucSendUdPData[45] =  can1SED.can_resbuf[5];
									sendudpdata.ucSendUdPData[46] =  can1SED.can_resbuf[6];
									sendudpdata.ucSendUdPData[47] =  can1SED.can_resbuf[7];	
									sendudpdata.ucSendUdpLen += 4;	
									if((sendudpdata.ucSendUdPData[21] + sendudpdata.ucSendUdPData[22] + sendudpdata.ucSendUdPData[23] + sendudpdata.ucSendUdPData[24]) <= 0x01)
									{										
										//用于速度扫描标志判定
										osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 6);
									}
								}
								else if(can1SED.can_resbuf[1] == 0xFD && can1SED.can_resbuf[2] == 0x60)
								{
									//分三种情况，左触发，右触发，未触发
									unsigned char getSBit[8] = {0};
									for (int i = 7; i >= 0; i--) 
									{
										getSBit[i] = (can1SED.can_resbuf[4] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}
									unsigned char getSBit_1[8] = {0};
									for (int i = 7; i >= 0; i--) 
									{
										getSBit_1[i] = (can1SED.can_resbuf[7] >> i) & 0x01; // 右移并与1进行位运算提取比特
									}									
									
									if(getSBit[4] == 1 && getSBit[5] == 1){
										//未触发
										//用于速度扫描标志判定
										osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 2);
									}
									else if(getSBit[4] == 0 && getSBit[5] == 1){
										//左触发
										//用于寻零判定
										//osEventFlagsSet(event_belownegativelimit_ID , 0x01U << 0);
										//用于速度扫描标志判定
										osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 0);
									}
									else if(getSBit[4] == 1 && getSBit[5] == 0){
											//右触发
											//用于寻零判定
											//osEventFlagsSet(event_belownegativelimit_ID , 0x01U << 1);
											//用于速度扫描标志判定
											osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 1);
									}
									else{
									}
									if(getSBit_1[0] == 1 && getSBit_1[1] == 1){//停止
										//osEventFlagsSet(event_SppedScanning_ID , 0x01U << 4);
										//用于速度扫描标志判定
										osEventFlagsSet(event_belowSppedScanning_ID , 0x01U << 4);
									}
									else if(getSBit_1[0] == 0 && getSBit_1[1] == 1){//运动中
										//osEventFlagsSet(event_SppedScanning_ID , 0x01U << 3);
									}
									else if(getSBit_1[0] == 0 && getSBit_1[1] == 0){//未初始化
									}
								}
								break;
						}
					}
					udp_echoclient_send(sendudpdata.ucSendUdPData , 50);
			}
  }	
}


unsigned char getHeartbeat(unsigned char getdata)
{
	unsigned char getHear = 0;
	switch(getdata)
	{
		case 0x0:
		//printf("Boot-up");
		getHear = 0x00;
		break;

		case 0x4:
		//printf("停止");
		getHear = 0x04;
		break;

		case 0x5:
		//printf("操作");
		getHear = 0x05;
		break;

		case 0x7f:
		//printf("预操做");
		getHear = 0x7f;
		break;

		default:
		break;
	}		
	return getHear;
}



/*
*********************************************************************************************************
*	函 数 名: AppTaskCan2_answer
*	功能说明: CAN2应答解包函数
*	形    参: 无
*	返 回 值: 无
* 优 先 级:   
*********************************************************************************************************
*/
//void AppTaskCan2_answer(void *argument)
//{	
//	uint16_t usFrequency = 50; /* 延迟周期 */
//	uint32_t tick;
//	/* 获取当前时间 */
//	tick = osKernelGetTickCount();
//  osStatus_t os_Status; 
//	struct CAN_RESDATA can2SED = {0};
////  static _CANMSG can1_Msg;
//	static CAN_TxHeaderTypeDef CanTxHeadType = { 0x7FF , 0x00 , CAN_ID_STD , CAN_RTR_DATA , 8 , DISABLE , };
//	unsigned char buff[8] = {1,2,3,4,5,6,7,8};	
//	
//	struct Lm1266Coll GetLm1266Coll = {0,{0}};
//  while(1)
//  {
//		os_Status = osMessageQueueGet(msgQueue_Can2ResData,  /* 消息队列ID */
//									  &can2SED,          /* 存放消息的内存 */
//									  NULL,				 /* 消息优先级 */
//									  osWaitForever);    /* 超时时间（一直等待） */
////		if(osOK == os_Status){
////			CanTxHeadType.StdId = can2SED.resStdID;
////			CAN_Send_Mesage(CAN2_Handler , &CanTxHeadType , &can2SED.can_resbuf);
////		}

////		/* 相对延迟 */
////		tick += usFrequency;                          
////		osDelayUntil(tick);			
//  }	
//}
