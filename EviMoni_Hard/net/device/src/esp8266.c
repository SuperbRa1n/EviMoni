/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	esp8266.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-05-08
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		ESP8266�ļ�����
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f103xb.h"

//�����豸����
#include "esp8266.h"

//Ӳ������
#include "gpio.h"
#include "delay.h"
#include "usart.h"

//C��
#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"****\",\"******\"\r\n"

#define ESP8266_PHONE_INFO		"AT+CIPSTART=\"TCP\",\"****\",***\r\n"


unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
uint8_t ch=0;
uint8_t WarningStr[25]="Data Overflow";
uint8_t esp8266_APIP[16]; //���esp8266��APip
unsigned char *phone_ssid_pwd;//����ֻ��ȵ�id������
unsigned char *remoteip;
uint8_t i=0;
uint8_t j=0;
//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0;							//��0���ռ���
			
		return REV_OK;								//���ؽ�����ɱ�־
	}
		
	esp8266_cntPre = esp8266_cnt;					//��Ϊ��ͬ
	
	return REV_WAIT;								//���ؽ���δ��ɱ�־

}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;

	HAL_UART_Transmit(&huart2,(unsigned char *)cmd,sizeof(cmd),1000);
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//����յ�����
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//����������ؼ���
			{
				//ESP8266_Clear();									//��ջ��棬�Ұ���ע�͵��Ƿ���������յ������ݵĴ�����Ȼÿ�ζ������ˣ���Ȼ���·�������Ҫ��Ҫ�ظ����㣩
				
				return 0;
			}
		}
		
		delay_ms(10);
	}
	
	return 1;

}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//��ս��ջ���
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//�յ���>��ʱ���Է�������
	{
		HAL_UART_Transmit(&huart2, data, sizeof(data),1000);		//�����豸������������
	}
	ESP8266_Clear();
}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//������IPD��ͷ
			if(ptrIPD == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				printf("\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//�ҵ�':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	ESP8266_Init
//
//	�������ܣ�	��ʼ��ESP8266
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
	delay_ms(250);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	delay_ms(500);
	
	ESP8266_Clear();
	
	//AT����
	printf("1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//����ΪAP+Stationģʽ
	printf("2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=3\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//��̬����IP
	printf("3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//����esp8266wifi ����
	printf("4.AT+CWSAP_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWSAP_CUR='EnviMoni','1234567890',5,3\r\n", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//��ѯesp8266APIP
	printf("5.AT+CIFSR\r\n");
	while(ESP8266_SendCmd("AT+CIFSR", "OK"))	
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	//ESP8266_Clear(); ���ﲻ�������Ӧ�������ȡesp8266APIP
	
	printf("6.��ȡesp8266APIP��...\r\n");
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
	ESP8266_Clear();//���ս�������
	
	printf("7.�������ֻ��ȵ�id������\r\n");
	//�ȴ��ֻ������ȵ�id�����벢��ȡ
	phone_ssid_pwd = ESP8266_GetIPD(50);
	ESP8266_Clear();//���ս�������
	
	//�����ֻ��ȵ�
	printf("8.AT+CWJAP_DEF=%s\r\n",phone_ssid_pwd);
	sprintf(ESP8266_WIFI_INFO, "AT+CWJAP_DEF=%s\r\n", phone_ssid_pwd);		//��������
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	ESP8266_Clear();
	
	//��ѯ�ֻ�IP
	printf("9.AT+CIPSTATUS\r\n");	
	while(ESP8266_SendCmd("AT+CIPSTATUS", "OK"))
	{
		ESP8266_Clear();
		delay_ms(500);
	}
	
	//�����ֻ�IP
	remoteip=ESP8266_remoteIP_CIPSTATUS(50);
	ESP8266_Clear();
	
	//����tcp����
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
//	�������ƣ�	ESP8266_remoteIP_CIPSTATUS
//
//	�������ܣ�	TCP�����¶�ȡԶ��IP��ַ
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	Զ��IP��ַ
//	
//==========================================================
//TCP�����¶�ȡԶ��IP��ַ
unsigned char *ESP8266_remoteIP_CIPSTATUS(unsigned short timeOut)
{
	uint8_t i=0;
	char *ptrCIPSTATUS = NULL;
	uint8_t numofcomma = 0;
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//����������
		{
			ptrCIPSTATUS = strstr((char *)esp8266_buf, "CIPSTATUS");				//������IPD��ͷ
			if(ptrCIPSTATUS == NULL)											//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
			{
				printf("\"CIPSTATUS\" not found\r\n");
			}
			else
			{
				ptrCIPSTATUS = strchr(ptrCIPSTATUS, ':');							//�ҵ�':'
				if(ptrCIPSTATUS != NULL)
				{
					while(numofcomma!=1)	//���������remoteip֮ǰ��ֻ��һ������
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
		delay_ms(5);													//��ʱ�ȴ�
	} while(timeOut--);
	return NULL;														//��ʱ��δ�ҵ������ؿ�ָ��
}

//esp8266���ڽ����ж�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(esp8266_cnt>sizeof(esp8266_buf)+2-1)//��ֹ���
	{
		HAL_UART_Transmit(huart, WarningStr, sizeof(WarningStr), 100);
		memset(esp8266_buf,'\0',sizeof(esp8266_cnt));//buffer����
		esp8266_cnt=0;
	}
	else
	{
		 esp8266_buf[esp8266_cnt++]=ch;
		 if((esp8266_buf[esp8266_cnt-1]=='\n')&&(esp8266_buf[esp8266_cnt-2]=='\r'))//�ж��Ƿ���ս���
		 {
			 printf("the signal is %s \r\n",esp8266_buf);
			 memset(esp8266_buf,'\0',sizeof(esp8266_buf));
			 esp8266_cnt=0;
		 }
	}
	HAL_UART_Receive_IT(&huart2,&ch,1);//ÿ�ν���һ���ֽھͲ���һ���ж�
}
