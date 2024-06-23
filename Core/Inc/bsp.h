
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
/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

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

extern CAN_HandleTypeDef   CAN1_Handler;  /*CAN ����ṹ����*/
extern CAN_HandleTypeDef   CAN2_Handler;  

/*�ɼ����ĵ������*/
struct MOTORDATA
{
	unsigned char num1_NMT;					//NMT
	unsigned char num1_findzero;		//Ѱ��
	unsigned char num1_opermode;		//����ģʽ
	unsigned short num1_errorcode;	//�������
	int num1_placecom;	//λ��ָ��
	int num1_placeact;	//ʵ��λ��
	int num1_sppedcom;	//�ٶ�ָ��
	int num1_sppedact;	//ʵ���ٶ�
	unsigned short num1_state;		//״̬��
	
	unsigned char num2_NMT;					//NMT
	unsigned char num2_findzero;		//Ѱ��
	unsigned char num2_opermode;		//����ģʽ
	unsigned short num2_errorcode;	//�������
	int num2_placecom;							//λ��ָ��
	int num2_placeact;							//ʵ��λ��
	int num2_sppedcom;							//�ٶ�ָ��
	int num2_sppedact;							//ʵ���ٶ�
	unsigned short num2_state;			//״̬��	
};

/*DYT����*/
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


/*LM1266�ɼ�������*/
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

/*���ģ���� �ṹ��*/
struct LM1266DATA
{
  unsigned char lm1266_flag_single;
	unsigned char lm1266_setup_len; 		//LM1266�������ݽ��ճ���
	unsigned char lm1266_real_len; 			//LM1266����ʵ�ʽ��ճ���
	unsigned char lm1266_resdata[38];		//LM1266��������
	
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
	unsigned char buf[48];	/*���ݴ洢����*/
	unsigned char zero[128];
	unsigned char read;		/*��ȡ��FIFO����*/
	unsigned char ucStatus;  /* ״̬����־ */
	unsigned char ucCount;	/**/
	unsigned char UMflag;
	unsigned char Canflag;
	unsigned char datalen;
};

/*���ڽ���CAN����*/
struct CAN_RESDATA{
	unsigned char can_resbuf[8];
	unsigned char can_resbuflen;
	unsigned int  resStdID;	//��׼֡
	unsigned int  resExtID; //��չ֡
};
extern struct CAN_RESDATA can1resdata;
extern struct CAN_RESDATA can2resdata;

struct UDPRESDATA{
	unsigned char getbuffer[128];
	unsigned char gerlen;
};

/*���λ��ģʽ�ṹ��*/
struct MOTORPOSITION
{
	unsigned int position;				//λ��
	unsigned int spped;						//�ٶ�
	unsigned int acceleration;		//���ٶ�
};


/*�����ԭ��ģʽ�ṹ��*/
struct MOTORFINDZERO
{
	unsigned int sppedhight;			//��ԭ�����
	unsigned int sppedlow;				//��ԭ�����
	unsigned int acceleration; 		//��ԭ����ٶ�
	unsigned char findzeromode;		//��ԭ��ģʽ
};

/*����ٶ�ɨ��ģʽ�ṹ��*/
struct MOTORSPPEDSCANNING
{
	int spped;						//�ٶ�
	unsigned int acceleration;		//���ٶ�
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
	const unsigned char rxSize;	/*const �����ڽṹ���������ʱ��ֵ */
}BspUart_t;

extern BspUart_t g_tBspUsart1;
extern BspUart_t g_tBspUart4;
extern BspUart_t g_tBspUart5;
extern BspUart_t g_tBspUsart2;
extern BspUart_t g_tBspUsart3;
extern BspUart_t g_tBspUsart6;

extern struct GETUDPDATA SetUdpData;

void Error_Handler(void);
/**********	RTX5����	***********/

/************************* ��Ϣ���� *******************************/

/*CAN1���ݽ��� . ��Ϣ����*/
extern osMessageQueueId_t msgQueue_Can1ResData;
extern const osMessageQueueAttr_t Can1ResData_Attr; 

/*CAN2���ݽ��� . ��Ϣ����*/
extern osMessageQueueId_t msgQueue_Can2ResData;
extern const osMessageQueueAttr_t Can2ResData_Attr;

/* ���ڽ���λ����Ϣ */
extern osMessageQueueId_t msgQueue_PositionData; 
extern const osMessageQueueAttr_t PositionData_Attr;

/* ���ڽ����ٶ�ɨ��ģʽ��Ϣ */
extern osMessageQueueId_t msgQueue_SppedScanning; 
extern const osMessageQueueAttr_t SppedScanning_Attr;
/*�±ڵ���ٶ�ɨ��ֵ*/
extern osMessageQueueId_t msgQueue_UnderSppedScanning; 
extern const osMessageQueueAttr_t UnderSppedScanning_Attr;


/*���ڽ�������UDP����*/
extern osMessageQueueId_t msgQueue_UdpResData; 			
extern const osMessageQueueAttr_t UdpResData_Attr;


/*���ڷ�������UDP����*/
extern osMessageQueueId_t msgQueue_UdpSendData; 			
extern const osMessageQueueAttr_t UdpSendData_Attr;


/*���ڽ���DYT����*/
extern osMessageQueueId_t msgQueue_DytData; 			
extern const osMessageQueueAttr_t DytData_Attr;

/* ���ڽ��� �±ڵ��λ����Ϣ */
extern osMessageQueueId_t msgQueue_UnderPositionData; 
extern const osMessageQueueAttr_t UnderPositionData_Attr;

/*lm1266���ݶ�ȡ*/
//extern osMessageQueueId_t msgQueue_lm1266Data; 
//extern const osMessageQueueAttr_t lm1266Data_Attr;

extern osMessageQueueId_t msgQueue_lm1266Num1Data; 
extern const osMessageQueueAttr_t lm1266Num1Data_Attr;

extern osMessageQueueId_t msgQueue_lm1266Num2Data; 
extern const osMessageQueueAttr_t lm1266Num2Data_Attr;

extern osMessageQueueId_t msgQueue_MotorData; 			
extern const osMessageQueueAttr_t MotorData_Attr;


/************************* �¼���־�� *******************************/

/*���߲�α�־ . �¼���־��*/
extern osEventFlagsId_t event_cable_ID;
extern const osEventFlagsAttr_t event_cable_Attr;

/*LM1266 . �¼���־��*/
extern const osEventFlagsAttr_t event_lm1266_Attr;
extern osEventFlagsId_t event_lm1266_ID;

/* �ٶ�0 �¼���־�� */
extern const osEventFlagsAttr_t event_sppedzero_Attr;
extern osEventFlagsId_t event_sppedzero_ID;

/* λ��0 �¼���־�� */
extern const osEventFlagsAttr_t event_positionzero_Attr;
extern osEventFlagsId_t event_positionzero_ID;

/* λ�õ��� �¼���־�� */
extern const osEventFlagsAttr_t event_positionarrive_Attr;
extern osEventFlagsId_t event_positionarrive_ID;

/*������λ �¼���־��*/
extern const osEventFlagsAttr_t event_negativelimit_Attr;
extern osEventFlagsId_t event_negativelimit_ID;
/*�±ڵ�������޵�*/
extern const osEventFlagsAttr_t event_belownegativelimit_Attr;
extern osEventFlagsId_t event_belownegativelimit_ID; /*��־��ID */


/*�رղ�ѯ������� �¼���־��*/
extern const osEventFlagsAttr_t event_closurefinddata_Attr;
extern osEventFlagsId_t event_closurefinddata_ID;
/*�±ڵ���رղ�ѯ������� �¼���־��*/
extern const osEventFlagsAttr_t event_belowclosurefinddata_Attr;
extern osEventFlagsId_t event_belowclosurefinddata_ID; /*��־��ID */


/*����ٶ�ɨ��ģʽ �¼���־��*/
extern const osEventFlagsAttr_t event_SppedScanning_Attr;
extern osEventFlagsId_t event_SppedScanning_ID; /*��־��ID */

extern const osEventFlagsAttr_t event_belowSppedScanning_Attr;
extern osEventFlagsId_t event_belowSppedScanning_ID; /*��־��ID */


/* ����1 �¼���־������ */
extern const osEventFlagsAttr_t event_uart1_Attr;
extern osEventFlagsId_t event_uart1_ID;

/* ����2 �¼���־������ */
extern const osEventFlagsAttr_t event_uart2_Attr;
extern osEventFlagsId_t event_uart2_ID;

/* ����3 �¼���־������ */
extern const osEventFlagsAttr_t event_uart3_Attr;
extern osEventFlagsId_t event_uart3_ID;

/* ����4 �¼���־������ */
extern const osEventFlagsAttr_t event_uart4_Attr;
extern osEventFlagsId_t event_uart4_ID;

/* ����5 �¼���־������ */
extern const osEventFlagsAttr_t event_uart5_Attr;
extern osEventFlagsId_t event_uart5_ID;

/* ����6 �¼���־������ */
extern const osEventFlagsAttr_t event_uart6_Attr;
extern osEventFlagsId_t event_uart6_ID;

/* ���ڻ�ȡ���ݽ� �¼���־������ */
extern const osEventFlagsAttr_t event_UdpGetData_Attr;
extern osEventFlagsId_t event_UdpGetData_ID;

/* �߳�ͬ����־λ ���ڷ����������� */
extern const osEventFlagsAttr_t event_UdpSendData_Attr;
extern osEventFlagsId_t event_UdpSendData_ID;

/************************* ���� *******************************/

/*���� . ����*/
extern osThreadId_t ThreadIdStart;

extern void AppTaskUDPSend(void *argument);
extern const osThreadAttr_t TaskUDPSend_Attr;
extern osThreadId_t ThreadIdTaskUDPSend;

/*�ӻ� . ����*/
extern void AppTaskGroce(void *argument);
extern const osThreadAttr_t ThreadGroce_Attr;
extern osThreadId_t ThreadIdTaskGroce;


/*DYT���ݽ�������*/
extern void AppTaskDYTData(void *argument);
extern const osThreadAttr_t ThreadDYTData_Attr;
extern osThreadId_t ThreadIdDYTData;

/*LM1266 . ����*/
//extern void AppTaskIM1266(void *argument);
//extern const osThreadAttr_t ThreadIM1266_Attr;
//extern osThreadId_t ThreadIdTaskIM1266;

/*LM1266 . ����*/
extern void AppTaskIM1266Num1(void *argument);
extern const osThreadAttr_t ThreadIM1266Num1_Attr;
extern osThreadId_t ThreadIdTaskIM1266Num1;


/*�ڶ��� LM1266*/
extern void AppTaskIM1266Num2(void *argument);
extern const osThreadAttr_t ThreadIM1266Num2_Attr;
extern osThreadId_t ThreadIdTaskIM1266Num2;


/*LWIP������ѯ . ����*/
extern osThreadId_t ThreadIdTaskLwIPPro;
extern const osThreadAttr_t ThreadLwIPPro_Attr;
extern void AppTaskLwIPPro(void *argument);

/*���߲�� . ����*/
extern osThreadId_t ThreadIdTaskEthCheck;
extern void AppTaskEthCheck(void *argument);
extern const osThreadAttr_t ThreadEthCheck_Attr;

/*CAN1�жϽ��� . ����*/
extern osThreadId_t ThreadIdTaskcan1_anwser;
extern void AppTaskCan1_answer(void *argument);
extern const osThreadAttr_t ThreadCan1answer_Attr;

/*CAN2�жϽ��� . ����*/
extern osThreadId_t ThreadIdTaskcan2_anwser;
extern void AppTaskCan2_answer(void *argument);
extern const osThreadAttr_t ThreadCan2answer_Attr;

/*LED . ����*/
extern osThreadId_t ThreadIdTaskLED;
extern void AppTaskLED(void *argument);
extern const osThreadAttr_t ThreadLED_Attr;

/*UDP�����������*/
extern void AppTaskUdpNetWork(void *argument);
extern const osThreadAttr_t ThreadUdpNetWork_Attr;
extern osThreadId_t ThreadIdTaskUdpNetWork;

/*���Ѱ������*/
extern void AppTaskMotorFindZero(void *argument);
extern const osThreadAttr_t ThreadMotorFindZero_Attr;
extern osThreadId_t ThreadIdTaskMotorFindZero;
/*�±ڵ�� - Ѱ������*/
extern void AppTaskUnderMotorFindZero(void *argument);
extern const osThreadAttr_t ThreadUnderMotorFindZero_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorFindZero;



/*���λ������*/
extern void AppTaskMotorPosition(void *argument);
extern const osThreadAttr_t TaskMotorPosition_Attr;
extern osThreadId_t ThreadIdTaskMotorPosition;
/*�±ڵ�� - λ������*/
extern void AppTaskUnderMotorPosition(void *argument);
extern const osThreadAttr_t TaskUnderMotorPosition_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorPosition;


/*�����ѯ��������*/
extern void AppTaskMotorDataquery(void *argument);
extern const osThreadAttr_t TaskMotorDataquery_Attr;
extern osThreadId_t ThreadIdTaskMotorDataquery;
/*�±ڵ������ - ��ѯ����*/
extern void AppTaskUnderMotorDataquery(void *argument);
extern const osThreadAttr_t TaskUnderMotorDataquery_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorDataquery;


/*����ٶ�ɨ������*/
extern void AppTaskMotorSppedScanning(void *argument);
extern const osThreadAttr_t TaskMotorSppedScanning_Attr;
extern osThreadId_t ThreadIdTaskMotorSppedScanning;
/*�±ڵ���ٶ�ɨ������*/
extern void AppTaskUnderMotorSppedScanning(void *argument);
extern const osThreadAttr_t TaskUnderMotorSppedScanning_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorSppedScanning;

/*����ٶ�����*/
extern void AppTaskMotorSpped(void *argument);
extern const osThreadAttr_t TaskMotorSpped_Attr;
extern osThreadId_t ThreadIdTaskMotorSpped;



/* ���ֹͣ���� */
extern void AppTaskMotorStop(void *argument);
extern const osThreadAttr_t TaskMotorStop_Attr;
extern osThreadId_t ThreadIdTaskMotorStop;


/* �±ڵ��ֹͣ���� */
extern void AppTaskUnderMotorStop(void *argument);
extern const osThreadAttr_t TaskUnderMotorStop_Attr;
extern osThreadId_t ThreadIdTaskUnderMotorStop;


#endif /* */