
#ifndef __BSP_H
#define __BSP_H

#include "EventRecorder.h"
#include "EventRecorderConf.h"
#include <stdio.h>
#include <string.h>
#include "includes.h"
#include "bsp_dwt.h"
#include "stm32f4xx_hal.h"


//#include "udp_echoclient.h"
#include "lwip/udp.h"
#include "bsp_can.h"
#include "bsp_gpio.h"
#include "bsp_uart_fifo.h"
#include "bsp_dma.h"
#include "bsp_uart_dma.h"

#include "udp_echoserver.h"

#include "Task_can.h"
#include "Task_led.h"
#include "Task_lwip.h"
//#include "Task_start.h"
/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

void SystemClock_Config(void);

enum CAN_Prescaler{
	CAN_PRE_1=1,
	CAN_PRE_2,
	CAN_PRE_3,
	CAN_PRE_4,
	CAN_PRE_5,
	CAN_PRE_6,
	CAN_PRE_7,
	CAN_PRE_8,
	CAN_PRE_9,
	CAN_PRE_10
};

extern CAN_HandleTypeDef   CAN1_Handler;  /*CAN 句柄结构定义*/
extern CAN_HandleTypeDef   CAN2_Handler;  

/*采集到的电机数据*/
struct MOTORDATA
{
	unsigned char num1_NMT;					//NMT
	unsigned char num1_findzero;		//寻零
	unsigned char num1_opermode;		//操作模式
	unsigned short num1_errorcode;	//错误代码
	int num1_placecom;	//位置指令
	int num1_placeact;	//实际位置
	int num1_sppedcom;	//速度指令
	int num1_sppedact;	//实际速度
	unsigned short num1_state;		//状态字
	
	unsigned char num2_NMT;					//NMT
	unsigned char num2_findzero;		//寻零
	unsigned char num2_opermode;		//操作模式
	unsigned short num2_errorcode;	//错误代码
	int num2_placecom;							//位置指令
	int num2_placeact;							//实际位置
	int num2_sppedcom;							//速度指令
	int num2_sppedact;							//实际速度
	unsigned short num2_state;			//状态字	
};

/*DYT数据*/
struct DYTDATA
{
	unsigned char ucgetBuffer[32];
	unsigned char getlen;
};


struct Lm1266Coll
{
	unsigned char datalen;
	unsigned char databuffer[32];
};


/*LM1266采集的数据*/
struct LM1266{
	unsigned int voltage;
	unsigned int current;
	unsigned int power;
	unsigned int electrical_energy;
	unsigned int Power_Factor;
	unsigned int carbon_dioxide;
	unsigned int frequency;
	unsigned char number;
};

/*检测模块标记 结构体*/
struct LM1266DATA
{
  unsigned char lm1266_flag_single;
	unsigned char lm1266_setup_len; 		//LM1266设置数据接收长度
	unsigned char lm1266_real_len; 			//LM1266设置实际接收长度
	unsigned char lm1266_resdata[38];		//LM1266缓存数组
	
	unsigned char lm1266_senddata[8];
	unsigned char lm1266_senddatalen;
	
	unsigned int lm1266_voltage;
	unsigned int lm1266_current;
	unsigned int lm1266_power;
};
extern struct LM1266DATA Lm1266Data_12v;
extern struct LM1266DATA Lm1266Data_48v;

//#pragma pack(2)
struct UARTState_Machine{
	unsigned char buf[48];	/*数据存储数组*/
	unsigned char zero[128];
	unsigned char read;		/*读取的FIFO数据*/
	unsigned char ucStatus;  /* 状态机标志 */
	unsigned char ucCount;	/**/
	unsigned char UMflag;
	unsigned char Canflag;
	unsigned char datalen;
};

/*用于接受CAN数据*/
struct CAN_RESDATA{
	unsigned char can_resbuf[8];
	unsigned char can_resbuflen;
	unsigned int  resStdID;	//标准帧
	unsigned int  resExtID; //拓展帧
};
extern struct CAN_RESDATA can1resdata;
extern struct CAN_RESDATA can2resdata;

struct UDPRESDATA{
	unsigned char getbuffer[128];
	unsigned char gerlen;
};

/*电机位置模式结构体*/
struct MOTORPOSITION
{
	unsigned int position;				//位置
	unsigned int spped;						//速度
	unsigned int acceleration;		//加速度
};


/*电机回原点模式结构体*/
struct MOTORFINDZERO
{
	unsigned int sppedhight;			//回原点高速
	unsigned int sppedlow;				//回原点低速
	unsigned int acceleration; 		//回原点加速度
	unsigned char findzeromode;		//回原点模式
};

/*电机速度扫描模式结构体*/
struct MOTORSPPEDSCANNING
{
	int spped;						//速度
	unsigned int acceleration;		//加速度
};

struct GETUDPDATA
{
	unsigned char  ucGetUdpData[32];
	unsigned short ucGetUdpLen;
	unsigned char  ucGetUdpFlag;
};


struct SendUDPData
{
	unsigned char ucSendUdPData[64];
	unsigned short ucSendUdpLen;
};

typedef struct
{
	unsigned char *pRxBuf;
	unsigned char rxFlag;
	unsigned char rxLen;
	const unsigned char rxSize;	/*const 必须在结构体变量定义时赋值 */
}BspUart_t;

extern BspUart_t g_tBspUsart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUart5;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart3;
extern BspUart_t g_tBspUsart6;

extern struct GETUDPDATA SetUdpData;

void Error_Handler(void);
/**********	RTX5引用	***********/

/************************* 消息队列 *******************************/

/*CAN1数据接收 . 消息队列*/
extern osMessageQueueId_t msgQueue_Can1ResData;
extern const osMessageQueueAttr_t Can1ResData_Attr; 

/*CAN2数据接收 . 消息队列*/
extern osMessageQueueId_t msgQueue_Can2ResData;
extern const osMessageQueueAttr_t Can2ResData_Attr;

/* 用于接收位置信息 */
extern osMessageQueueId_t msgQueue_PositionData; 
extern const osMessageQueueAttr_t PositionData_Attr;

/* 用于接收速度扫描模式信息 */
extern osMessageQueueId_t msgQueue_SppedScanning; 
extern const osMessageQueueAttr_t SppedScanning_Attr;
/*下壁电机速度扫描值*/
extern osMessageQueueId_t msgQueue_UnderSppedScanning; 
extern const osMessageQueueAttr_t UnderSppedScanning_Attr;


/*用于接收网络UDP数据*/
extern osMessageQueueId_t msgQueue_UdpResData; 			
extern const osMessageQueueAttr_t UdpResData_Attr;


/*用于发送网络UDP数据*/
extern osMessageQueueId_t msgQueue_UdpSendData; 			
extern const osMessageQueueAttr_t UdpSendData_Attr;


/*用于接收DYT数据*/
extern osMessageQueueId_t msgQueue_DytData; 			
extern const osMessageQueueAttr_t DytData_Attr;

/* 用于接收 下壁电机位置信息 */
extern osMessageQueueId_t msgQueue_UnderPositionData; 
extern const osMessageQueueAttr_t UnderPositionData_Attr;

/*lm1266数据读取*/
//extern osMessageQueueId_t msgQueue_lm1266Data; 
//extern const osMessageQueueAttr_t lm1266Data_Attr;

extern osMessageQueueId_t msgQueue_lm1266Num1Data; 
extern const osMessageQueueAttr_t lm1266Num1Data_Attr;

extern osMessageQueueId_t msgQueue_lm1266Num2Data; 
extern const osMessageQueueAttr_t lm1266Num2Data_Attr;

extern osMessageQueueId_t msgQueue_MotorData; 			
extern const osMessageQueueAttr_t MotorData_Attr;


/************************* 事件标志组 *******************************/

/*网线插拔标志 . 事件标志组*/
extern osEventFlagsId_t event_cable_ID;
extern const osEventFlagsAttr_t event_cable_Attr;

/*LM1266 . 事件标志组*/
extern const osEventFlagsAttr_t event_lm1266_Attr;
extern osEventFlagsId_t event_lm1266_ID;

/* 速度0 事件标志组 */
extern const osEventFlagsAttr_t event_sppedzero_Attr;
extern osEventFlagsId_t event_sppedzero_ID;

/* 位置0 事件标志组 */
extern const osEventFlagsAttr_t event_positionzero_Attr;
extern osEventFlagsId_t event_positionzero_ID;

/* 位置到达 事件标志组 */
extern const osEventFlagsAttr_t event_positionarrive_Attr;
extern osEventFlagsId_t event_positionarrive_ID;

/*负极限位 事件标志组*/
extern const osEventFlagsAttr_t event_negativelimit_Attr;
extern osEventFlagsId_t event_negativelimit_ID;
/*下壁电机负极限点*/
extern const osEventFlagsAttr_t event_belownegativelimit_Attr;
extern osEventFlagsId_t event_belownegativelimit_ID; /*标志组ID */


/*关闭查询电机参数 事件标志组*/
extern const osEventFlagsAttr_t event_closurefinddata_Attr;
extern osEventFlagsId_t event_closurefinddata_ID;
/*下壁电机关闭查询电机参数 事件标志组*/
extern const osEventFlagsAttr_t event_belowclosurefinddata_Attr;
extern osEventFlagsId_t event_belowclosurefinddata_ID; /*标志组ID */


/*电机速度扫描模式 事件标志组*/
extern const osEventFlagsAttr_t event_SppedScanning_Attr;
extern osEventFlagsId_t event_SppedScanning_ID; /*标志组ID */

extern const osEventFlagsAttr_t event_belowSppedScanning_Attr;
extern osEventFlagsId_t event_belowSppedScanning_ID; /*标志组ID */


/* 串口1 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart1_Attr;
extern osEventFlagsId_t event_uart1_ID;

/* 串口2 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart2_Attr;
extern osEventFlagsId_t event_uart2_ID;

/* 串口3 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart3_Attr;
extern osEventFlagsId_t event_uart3_ID;

/* 串口4 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart4_Attr;
extern osEventFlagsId_t event_uart4_ID;

/* 串口5 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart5_Attr;
extern osEventFlagsId_t event_uart5_ID;

/* 串口6 事件标志组属性 */
extern const osEventFlagsAttr_t event_uart6_Attr;
extern osEventFlagsId_t event_uart6_ID;

/* 网口获取数据接 事件标志组属性 */
extern const osEventFlagsAttr_t event_UdpGetData_Attr;
extern osEventFlagsId_t event_UdpGetData_ID;

/* 线程同步标志位 用于发送网口数据 */
extern const osEventFlagsAttr_t event_UdpSendData_Attr;
extern osEventFlagsId_t event_UdpSendData_ID;

/************************* 任务 *******************************/

/*启动 . 任务*/
extern osThreadId_t ThreadIdStart;

extern void AppTaskUDPSend(void *argument);
extern const osThreadAttr_t TaskUDPSend_Attr;
extern osThreadId_t ThreadIdTaskUDPSend;

/*杂货 . 任务*/
extern void AppTaskGroce(void *argument);
extern const osThreadAttr_t ThreadGroce_Attr;
extern osThreadId_t ThreadIdTaskGroce;


/*DYT数据解析任务*/
extern void AppTaskDYTData(void *argument);
extern const osThreadAttr_t ThreadDYTData_Attr;
extern osThreadId_t ThreadIdDYTData;

/*LM1266 . 任务*/
//extern void AppTaskIM1266(void *argument);
//extern const osThreadAttr_t ThreadIM1266_Attr;
//extern osThreadId_t ThreadIdTaskIM1266;

/*LM1266 . 任务*/
extern void AppTaskIM1266Num1(void *argument);
extern const osThreadAttr_t ThreadIM1266Num1_Attr;
extern osThreadId_t ThreadIdTaskIM1266Num1;


/*第二个 LM1266*/
extern void AppTaskIM1266Num2(void *argument);
extern const osThreadAttr_t ThreadIM1266Num2_Attr;
extern osThreadId_t ThreadIdTaskIM1266Num2;


/*LWIP网络轮询 . 任务*/
extern osThreadId_t ThreadIdTaskLwIPPro;
extern const osThreadAttr_t ThreadLwIPPro_Attr;
extern void AppTaskLwIPPro(void *argument);

/*网线插拔 . 任务*/
extern osThreadId_t ThreadIdTaskEthCheck;
extern void AppTaskEthCheck(void *argument);
extern const osThreadAttr_t ThreadEthCheck_Attr;

/*CAN1中断接收 . 任务*/
extern osThreadId_t ThreadIdTaskcan1_anwser;
extern void AppTaskCan1_answer(void *argument);
extern const osThreadAttr_t ThreadCan1answer_Attr;

/*CAN2中断接收 . 任务*/
extern osThreadId_t ThreadIdTaskcan2_anwser;
extern void AppTaskCan2_answer(void *argument);
extern const osThreadAttr_t ThreadCan2answer_Attr;

/*LED . 任务*/
extern osThreadId_t ThreadIdTaskLED;
extern void AppTaskLED(void *argument);
extern const osThreadAttr_t ThreadLED_Attr;

/*UDP网络任务解析*/
extern void AppTaskUdpNetWork(void *argument);
extern const osThreadAttr_t ThreadUdpNetWork_Attr;
extern osThreadId_t ThreadIdTaskUdpNetWork;

/*电机寻零任务*/
extern void AppTaskMotorFindZero(void *argument);
extern const osThreadAttr_t ThreadMotorFindZero_Attr;
extern osThreadId_t ThreadIdTaskMotorFindZero;
/*下壁电机 - 寻零任务*/
extern void AppTaskUnderMotorFindZero(void *argument);
extern const osThreadAttr_t ThreadUnderMotorFindZero_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorFindZero;



/*电机位置任务*/
extern void AppTaskMotorPosition(void *argument);
extern const osThreadAttr_t TaskMotorPosition_Attr;
extern osThreadId_t ThreadIdTaskMotorPosition;
/*下壁电机 - 位置任务*/
extern void AppTaskUnderMotorPosition(void *argument);
extern const osThreadAttr_t TaskUnderMotorPosition_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorPosition;


/*电机查询数据任务*/
extern void AppTaskMotorDataquery(void *argument);
extern const osThreadAttr_t TaskMotorDataquery_Attr;
extern osThreadId_t ThreadIdTaskMotorDataquery;
/*下壁电机数据 - 查询任务*/
extern void AppTaskUnderMotorDataquery(void *argument);
extern const osThreadAttr_t TaskUnderMotorDataquery_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorDataquery;


/*电机速度扫描任务*/
extern void AppTaskMotorSppedScanning(void *argument);
extern const osThreadAttr_t TaskMotorSppedScanning_Attr;
extern osThreadId_t ThreadIdTaskMotorSppedScanning;
/*下壁电机速度扫描任务*/
extern void AppTaskUnderMotorSppedScanning(void *argument);
extern const osThreadAttr_t TaskUnderMotorSppedScanning_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorSppedScanning;

/*电机速度任务*/
extern void AppTaskMotorSpped(void *argument);
extern const osThreadAttr_t TaskMotorSpped_Attr;
extern osThreadId_t ThreadIdTaskMotorSpped;



/* 电机停止任务 */
extern void AppTaskMotorStop(void *argument);
extern const osThreadAttr_t TaskMotorStop_Attr;
extern osThreadId_t ThreadIdTaskMotorStop;


/* 下壁电机停止任务 */
extern void AppTaskUnderMotorStop(void *argument);
extern const osThreadAttr_t TaskUnderMotorStop_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorStop;


#endif /* */