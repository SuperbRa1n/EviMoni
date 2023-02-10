/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	esp8266.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		ESP8266的简单驱动
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f103xb.h"

//网络设备驱动
#include "esp8266.h"

//硬件驱动
#include "gpio.h"
#include "delay.h"
#include "usart.h"

//C库
#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"****\",\"******\"\r\n"

#define ESP8266_PHONE_INFO		"AT+CIPSTART=\"TCP\",\"****\",***\r\n"


unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
uint8_t ch=0;
uint8_t WarningStr[25]="Data Overflow";
uint8_t esp8266_APIP[16]; //存放esp8266的APip
unsigned char *phone_ssid_pwd;//存放手机热点id和密码
unsigned char *remoteip;
uint8_t i=0;
uint8_t j=0;
//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志

}

//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	HAL_UART_Transmit(&huart2,(unsigned char *)cmd,sizeof(cmd),1000);
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				//ESP8266_Clear();									//清空缓存，我把它注释掉是方便后续对收到的数据的处理，不然每次都清零了，虽然导致反馈不需要的要重复清零）
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		HAL_UART_Transmit(&huart2, data, sizeof(data),1000);		//发送设备连接请求数据
	}
	ESP8266_Clear();
}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				printf("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
	delay_ms(250);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	delay_ms(500);
	
	ESP8266_Clear();
	
	//AT测试
	printf("1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//设置为AP+Station模式
	printf("2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=3\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//动态分配IP
	printf("3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//设置esp8266wifi 参数
	printf("4.AT+CWSAP_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWSAP_CUR='EnviMoni','1234567890',5,3\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//查询esp8266APIP
	printf("5.AT+CIFSR\r\n");
	while(ESP8266_SendCmd("AT+CIFSR", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	//ESP8266_Clear(); 这里不能清楚响应，下面读取esp8266APIP
	
	printf("6.读取esp8266APIP中...\r\n");
	while(esp8266_buf[i]!='\"')
	{
		i++;
	}
	i++;

	while(esp8266_buf[i]!='\"')
	{
		esp8266_APIP[j]=esp8266_buf[i];
		i++;
		j++;
	}
	esp8266_APIP[j]='\0';
	ESP8266_Clear();//接收结束清零
	
	printf("7.请输入手机热点id和密码\r\n");
	//等待手机发送热点id和密码并读取
	phone_ssid_pwd = ESP8266_GetIPD(50);
	ESP8266_Clear();//接收结束清零
	
	//连接手机热点
	printf("8.AT+CWJAP_DEF=%s\r\n",phone_ssid_pwd);
	sprintf(ESP8266_WIFI_INFO, "AT+CWJAP_DEF=%s\r\n", phone_ssid_pwd);		//发送命令
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//查询手机IP
	printf("9.AT+CIPSTATUS\r\n");	
	while(ESP8266_SendCmd("AT+CIPSTATUS", "OK"))
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	
	//保存手机IP
	remoteip=ESP8266_remoteIP_CIPSTATUS(50);
	ESP8266_Clear();
	
	//建立tcp连接
	printf("10. CIPSTART\r\n");
	sprintf(ESP8266_PHONE_INFO, "AT+CIPSTART=\"TCP\",%s,333\r\n", remoteip);
	while(ESP8266_SendCmd(ESP8266_PHONE_INFO, "OK"))
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	printf("6. ESP8266 Init OK\r\n");

}
//==========================================================
//	函数名称：	ESP8266_remoteIP_CIPSTATUS
//
//	函数功能：	TCP单连下读取远端IP地址
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	远端IP地址
//	
//==========================================================
//TCP连接下读取远端IP地址
unsigned char *ESP8266_remoteIP_CIPSTATUS(unsigned short timeOut)
{
	uint8_t i=0;
	char *ptrCIPSTATUS = NULL;
	uint8_t numofcomma = 0;
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrCIPSTATUS = strstr((char *)esp8266_buf, "CIPSTATUS");				//搜索“IPD”头
			if(ptrCIPSTATUS == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				printf("\"CIPSTATUS\" not found\r\n");
			}
			else
			{
				ptrCIPSTATUS = strchr(ptrCIPSTATUS, ':');							//找到':'
				if(ptrCIPSTATUS != NULL)
				{
					while(numofcomma!=1)	//单连接情况remoteip之前就只有一个逗号
					{
						while(*ptrCIPSTATUS!=',')
						{
							ptrCIPSTATUS++;
						}	
						numofcomma++;
						ptrCIPSTATUS++;
					}
					while(*ptrCIPSTATUS!=',')
					{
						remoteip[i]=*ptrCIPSTATUS;
						ptrCIPSTATUS++;
						i++;
					}
					return remoteip;
				}
				else
					return NULL;
			}
		}
		delay_ms(5);													//延时等待
	} while(timeOut--);
	return NULL;														//超时还未找到，返回空指针
}

//esp8266串口接收中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(esp8266_cnt>sizeof(esp8266_buf)+2-1)//防止溢出
	{
		HAL_UART_Transmit(huart, WarningStr, sizeof(WarningStr), 100);
		memset(esp8266_buf,'\0',sizeof(esp8266_cnt));//buffer清零
		esp8266_cnt=0;
	}
	else
	{
		 esp8266_buf[esp8266_cnt++]=ch;
		 if((esp8266_buf[esp8266_cnt-1]=='\n')&&(esp8266_buf[esp8266_cnt-2]=='\r'))//判断是否接收结束
		 {
			 printf("the signal is %s \r\n",esp8266_buf);
			 memset(esp8266_buf,'\0',sizeof(esp8266_buf));
			 esp8266_cnt=0;
		 }
	}
	HAL_UART_Receive_IT(&huart2,&ch,1);//每次接收一个字节就产生一次中断
}
