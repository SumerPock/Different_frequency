/*
*********************************************************************************************************
*
*	模块名称 : lwip网络协议栈测试
*	文件名称 : app_lwip_lib.c
*	版    本 : V1.0
*	说    明 : lwip服务器简单测试
*
*	修改记录 :
*		版本号   日期         作者        说明
*		V1.0    2019-12-06   Eric2013     首发
*
*	Copyright (C), 2018-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/	
#include "includes.h"	



/*
*********************************************************************************************************
*	                                  用于本文件的调试
*********************************************************************************************************
*/
#if 1
	#define printf_debug printf
#else
	#define printf_debug(...)
#endif


/*
*********************************************************************************************************
*	                                  宏定义
*********************************************************************************************************
*/
#define PORT_NUM       1001    /* TCP服务器监听端口号 */


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
struct netif netif;


/*
*********************************************************************************************************
*	函 数 名: LwIPTest
*	功能说明: LwIPTest应用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/  
unsigned char net_init(void) 
{
	unsigned char statr = 0;
	struct netif *Netif_Init_Flag;		//调用netif_add()函数时的返回值,用于判断网络初始化是否成功
	struct ip4_addr ipaddr;						//ip地址
	struct ip4_addr netmask;					//子网掩码
	struct ip4_addr gw;								//默认网关 

	/* 创建协议栈线程 */
	tcpip_init(NULL, NULL);

#if LWIP_DHCP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
	IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);		//板卡IP
	IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);	//板卡掩码
	IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);				//板卡网关
#endif

	Netif_Init_Flag = netif_add(&netif ,\
															&ipaddr ,\
															&netmask ,\
															&gw ,\
															NULL ,\
															&ethernetif_init ,
															&ethernet_input);//向网卡列表中添加一个网口
	/* 将网络接口添加到netif_list列表 */
	if(Netif_Init_Flag == NULL){
		statr = 3;//网卡添加失败 
	}else{
		/*  注册默认网络接口 */
		netif_set_default(&netif);
		
		/* Set the link callback function, this function is called on change of link status*/
		netif_set_up(&netif);
		statr = 0;
	}	
	#if LWIP_DHCP
		dhcp_start (&netif);
	#endif
		return statr;//操作OK.
}

/*
*********************************************************************************************************
*	函 数 名: LwIPTest
*	功能说明: LwIPTest应用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/    
void LwIPTest(void)
{
	udp_echoserver_init();
	
//	struct netconn *conn, *newconn;
//	err_t err;

//	/* 创建服务器 */
//	conn = netconn_new(0X10);
//	netconn_bind(conn, IP_ADDR_ANY, PORT_NUM);

//	/* 设置监听 */
//	netconn_listen(conn);
//	
//	while (1) 
//	{
//		/* 等待接收新的连接 */
//		err = netconn_accept(conn, &newconn);

//		/* 处理新连接 */
//		if (err == ERR_OK) 
//		{
//			struct netbuf *buf;
//			void *data;
//			u16_t len;

//			/* 简单的数据回环 */
//			while ((err = netconn_recv(newconn, &buf)) == ERR_OK) 
//			{
//		        do {
//		             netbuf_data(buf, &data, &len);
//		             err = netconn_write(newconn, data, len, 0x01);
//		        } while (netbuf_next(buf) >= 0);
//				netbuf_delete(buf);
//			}
//			
//			/* 删除连接  */
//			netconn_close(newconn);
//			netconn_delete(newconn);
//		}

//	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
