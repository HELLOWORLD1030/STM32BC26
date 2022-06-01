#include "stm32f10x.h"
#include "lcd.h"
#include "spi.h"
#include "usart.h"
#include "GPIO.h"
#include "delay.h"
#include <string.h>
#include "tsensor.h"
#include "cJson.h"
unsigned char addr = 0;
unsigned int i;
unsigned char Query(char * src,char * des,unsigned int LEN);
bool Wait_Str_x_100ms(char* str1,char* str2,unsigned char time_x_100ms);
bool Wait_Str1_Str2_x_100ms(char uartx,char and_or,char str_NO,char* str1,char* str2,unsigned char 
time_x_100ms);
bool Wait_OK_x_100ms(unsigned char time_x_100ms);
void Wait_OK(void);
void copy_str(char* des,char* src,unsigned char len);
void DealUpData(char *from,uint8_t *to,uint16_t len);
void Array_CLR(char *src,char len) ;
void Init_Nbiot(void);//初始化 Nbiot 模块
bool Flag_Need_Init_Nbiot = 1;
bool Flag_device_error = 0;
int seqNum=0;
char seqNum1[25];
void trim_string(char *str);
void IntToChar(int num,char*);
char usert[355]={0};
char NB_Send_buf1[355]={0};
void SendData();
float GetTemp();
float temp;
char Receive_BUF[500]={0};
int setRun=1;
uint8_t get_sub_str(char * str,char * separator1,char * separator2,int8_t num, char * substr);
char Receive_Data[250]={0};
int main(void)
{
	/* 实验1
delay_init(); //???????
Lcd_Init(); //??? LCD
BACK_COLOR=BLACK; //?????
POINT_COLOR=YELLOW; //?????
LCD_Show_Chinese16x16(80,20,"实验");
LCD_ShowString(140,20,"1.5.1");
LCD_ShowString(80,80,"This is a test!");//???????
LCD_Show_Chinese16x16(80,120,"这是一条测试语句");
while(1);
	*/
	/*
	GPIO_Configuration();//GPIO 初始化
delay_init(); //延时函数初始化
Lcd_Init(); //初始化 LCD
Usart1_Init(115200); //串口 1 初始化
Usart2_Init(115200); //串口 2 初始化
NB_PWR_ON(); //NB 模块上电
BACK_COLOR=BLACK; //背景为黑色
POINT_COLOR=YELLOW; //画笔为黄色
LCD_Show_Chinese16x16(100,20,"实验");
LCD_ShowString(160,20,"2.5.1");
LCD_Show_Chinese16x16(50,70,"串口透传模式！");
LCD_ShowString(50,100,"usart1 <---> usart2");
while(1);
}
*/

unsigned char NB_Send_buf[355]={0};
unsigned char n;
GPIO_Configuration();//GPIO 初始化
delay_init(); //延时函数初始化
Usart1_Init(115200); //下载口、调试端口
Usart2_Init(115200); //连接 NBIOT
TIM3_Int_Init(999,7199); //开启定时器 3，计数 100ms
Lcd_Init(); //初始化 LCD
T_Adc_Init();
LCD_Clear(BLACK); //清屏 ,这是真正的屏幕背景色
POINT_COLOR=GREEN;
LCD_DrawRectangle(0, 0, 320, 240);//画矩形
POINT_COLOR=YELLOW; //画笔为黄色
NB_PWR_ON(); //NB 模块上电
while(1)
{
if(Flag_Need_Init_Nbiot)
{
Flag_Need_Init_Nbiot = 0;
Init_Nbiot();
USART1TxStr("请发送英文字符，并以回车结束！\r\n");
}

SendData();
delay_ms(1500);
delay_ms(1500);
delay_ms(1500);


if((Flag_Usart1_Receive) && (!Count_Timer3_value_USART1_receive_timeout))//如果串口 1 接收到数据且未超时
{
Flag_Usart1_Receive = 0; //接收标志清零
USART1TxStr("中断传送 \r\n");
USART1TxStr(USART1_RX_BUF);


CLR_Buf1(); //清空串口 1

}

}
}
void Init_Nbiot(void)//初始化 Nbiot 模块
{
unsigned int i,j,n = 3;
char n_str;
char temp_str[30];
for(i = 0;i < 30;i ++)
temp_str[i] = 0;
Y_LED_ON;
USART1TxStr("检测 NB 模块上电时返回的数据...\r\n");
LCD_ShowString(8*2,16*2,"restarting...");
delay_ms(1500); //延时 1500ms
delay_ms(1500);
Wait_Str1_Str2_x_100ms(2,1,1,"Leaving the BROM","",60);
CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
while(n--)
{
USART1TxStr("发送 AT...\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*3,&n_str);
LCD_ShowString(8*2,16*3,"AT...");
USART2TxStr("AT\r\n");//测试 NB 模块
if(!Wait_Str1_Str2_x_100ms(2,1,1,"OK","",50))
n = 0;
}CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
n = 3;
while(n--)
{
USART1TxStr("查询设备 PDP 地址...\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*4,&n_str);
LCD_ShowString(8*2,16*4,"AT+CGPADDR=1");
USART2TxStr("AT+CGPADDR=1\r\n");//查询设备的 PDP 地址，返回格式：+CGPADDR: 1,21.176.194.17,0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.1 后跟 OK
if(!Wait_Str1_Str2_x_100ms(2,2,2,"+CGPADDR:","OK",50))
n = 0;
}CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
n = 3;
while(n--)
{
	/*
USART1TxStr("配置电信 IOT 平台地址和端口号...\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*5,&n_str);
LCD_ShowString(8*2,16*5,"AT+QLWSERV=\"221.229.214.202\",5683");
USART2TxStr("AT+QLWSERV=\"221.229.214.202\",5683\r\n");//配置电信 IOT 平台地址和端口，返回格式：OK
if(!Wait_Str1_Str2_x_100ms(2,1,1,"OK","",50))
n = 0;
	*/
USART1TxStr("配置电信 IOT 平台地址和端口号...\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*5,&n_str);
LCD_ShowString(8*2,16*5,"connecting...");
USART2TxStr("AT+QMTOPEN=0,\"mqtt.ctwing.cn\",1883\r\n");//配置电信 IOT 平台地址和端口，返回格式：OK
	if(!Wait_Str1_Str2_x_100ms(2,1,1,"+QMTOPEN: 0,0","",300))
	{n = 0;
G_LED_ON;
Y_LED_OFF;
R_LED_OFF;
LCD_ShowString(8*2,16*6,"MQTT connection has been open");
	}else{
		n=3;
	}
}CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
n = 3;

while(n--)
{
USART1TxStr("连接MQTT\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*7,&n_str);
LCD_ShowString(8*2,16*7,"connecting to the aep ");
USART2TxStr("AT+QMTCONN=0,\"15228122bc26zzg123\",\"d99b322bbd30467e93560f73c2c8b00e\",\"bY8mUy1JTrM8Sp-l1EgLD47FFVnBuFdITRTACe2cwrk\"\r\n");//添加 LwM2M 对象，返回格式：OK
//Wait_Str1_Str2_x_100ms(5,1,1,"UPDATE OK","",25);
	if(!Wait_Str1_Str2_x_100ms(2,1,1,"+QMTCONN: 0,0,0","",50))
	{
		n = 0;
G_LED_ON;
Y_LED_OFF;
R_LED_OFF;
LCD_ShowString(8*2,16*8,"MQTT connection has finished");
	}

delay_ms(1000);delay_ms(1000);delay_ms(1000);
}CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
n = 3;
while(n--)
{
USART1TxStr("订阅MQTT主题\r\n");
n_str = (3-n)+'0';
LCD_ShowString(8*1-3,16*9,&n_str);
LCD_ShowString(8*2,16*9,"subscribing ...");
USART2TxStr("AT+QMTSUB=0,1,\"run_flag\",0\r\n");//
	if(!Wait_Str1_Str2_x_100ms(2,1,1,"+QMTSUB: 0,1,0,0","",100))
	{
		n = 0;
G_LED_ON;
Y_LED_OFF;
R_LED_OFF;
LCD_ShowString(8*2,16*10,"MQTT subcribe has finished");
	}
}CLR_Buf2();
Y_LED_OFF;
delay_ms(1000);
Y_LED_ON;
n = 3;
}
char *my_strcat(char *str1, char *str2) //拼接字符串
{
 char *pt = str1;
 while(*str1!='\0') str1++;
 while(*str2!='\0') *str1++ = *str2++;
 *str1 = '\0';
 return pt;
}

void Wait_OK(void)
{
while(!Flag_usart1_receive_OK);
Flag_usart1_receive_OK = 0;
CLR_Buf1(); //清除串口 1 接收缓存
}
void copy_str(char* des,char* src,unsigned char len)
{
unsigned char i;
for(i = 0;i < len;i ++)
{
*(des + i) = *(src + i);
}
}
bool Wait_Str_x_100ms(char* str1,char* str2,unsigned char time_x_100ms)
{
bool Flag_receive_right = 0;
Count_time_wait_ok = time_x_100ms;
while(!Flag_receive_right && Count_time_wait_ok)
{
if(Flag_Usart1_Receive//如果串口 1 有数据接收
&&(!Count_Timer3_value_USART1_receive_timeout))//并且如果接收超时
{
Flag_Usart1_Receive = 0;
if(Query(USART1_RX_BUF,str1,USART1_REC_LEN))
{
Flag_receive_right = 1;
USART2TxStr(USART1_RX_BUF);
USART2TxStr("返回正确！\r\n");
}
else if(Query(USART1_RX_BUF,str2,USART1_REC_LEN))
{
Flag_receive_right = 1;
USART2TxStr(USART1_RX_BUF);
USART2TxStr("返回正确！\r\n");
}
}
}
if(!Count_time_wait_ok)
{
USART2TxStr("Flag_device_error = 1 \r\n");
Flag_device_error = 1;
}
else
Flag_device_error = 0;
USART2TxStr("Count_time_wait_ok = ");
USART2TxChar(Count_time_wait_ok / 100 + '0'); 
USART2TxChar(Count_time_wait_ok % 100 / 10 + '0'); 
USART2TxChar(Count_time_wait_ok % 10 + '0');
 USART2TxStr("\r\n");
delay_ms(100);
if(Flag_device_error)
return 1;//超时返回 1
else
return 0;//正常返回 0
}
bool Wait_Str1_Str2_x_100ms(char uartx,char and_or,char str_NO,char* str1,char* str2,unsigned char 
time_x_100ms)
{
bool Flag_receive_right = 0;
bool* add_Flag_Usart_x_Receive;
unsigned char* Count_Timer3_value_USART_x_receive_timeout;
char* USARTx_RX_BUF;
unsigned int LEN = 0;
switch(uartx)
{
case 1: add_Flag_Usart_x_Receive = &Flag_Usart1_Receive; 
Count_Timer3_value_USART_x_receive_timeout = 
&Count_Timer3_value_USART1_receive_timeout;
USARTx_RX_BUF = &USART1_RX_BUF[0];
LEN = USART_REC_LEN;
break;
case 2: add_Flag_Usart_x_Receive = &Flag_Usart2_Receive; 
Count_Timer3_value_USART_x_receive_timeout = 
&Count_Timer3_value_USART2_receive_timeout;
USARTx_RX_BUF = (char*)&USART2_RX_BUF[0];
LEN = USART_REC_LEN;
break;
}
Count_time_wait_ok = time_x_100ms;
while(!Flag_receive_right && Count_time_wait_ok)
{
if(*add_Flag_Usart_x_Receive//如果串口 x 有数据接收
&&(!*Count_Timer3_value_USART_x_receive_timeout))//并且如果接收超时
{
*add_Flag_Usart_x_Receive = 0;
if(and_or == 2)//如果要求满足两个字符串
{
if(Query(USARTx_RX_BUF,str1,LEN) && Query(USARTx_RX_BUF,str2,LEN))
{
Flag_receive_right = 1;
//USART1TxStr(USARTx_RX_BUF);
//USART1TxStr("返回正确！\r\n");
}
}
else if(and_or == 1)//如果只要求满足一个字符串
{
if(Query(USARTx_RX_BUF,str1,LEN))
{
Flag_receive_right = 1;
//USART1TxStr(USARTx_RX_BUF);
//USART1TxStr("返回正确！\r\n");
}
else
{
if(str_NO == 2)
{
if(Query(USARTx_RX_BUF,str2,LEN))
{
Flag_receive_right = 1;
//USART1TxStr(USARTx_RX_BUF);
//USART1TxStr("返回正确！\r\n");
}
}
}
}
}
}
if(!Count_time_wait_ok)
{
//USART1TxStr("Flag_device_error = 1 \r\n");
//USART1TxStr(USARTx_RX_BUF);
Flag_device_error = 1;
R_LED_ON;
}
else
{
R_LED_OFF;
Flag_device_error = 0;
}
// USART1TxStr("Count_time_wait_ok = ");
// USART1TxChar(Count_time_wait_ok / 100 + '0'); 
// USART1TxChar(Count_time_wait_ok % 100 / 10 + '0'); 
// USART1TxChar(Count_time_wait_ok % 10 + '0');
// USART1TxStr("\r\n");
delay_ms(100);
if(Flag_device_error)
return 1;//超时返回 1
else
return 0;//正常返回 0
}
bool Wait_OK_x_100ms(unsigned char time_x_100ms)
{
Count_time_wait_ok = time_x_100ms;
while(!Flag_usart1_receive_OK && Count_time_wait_ok)
{
}
if(!Count_time_wait_ok)
{
//USART1TxStr("Flag_device_error = 1 \r\n");
Flag_device_error = 1;
}
else
Flag_device_error = 0;
// USART1TxStr("Count_time_wait_ok = ");
// USART1TxChar(Count_time_wait_ok / 100 + '0'); 
// USART1TxChar(Count_time_wait_ok % 100 / 10 + '0'); 
// USART1TxChar(Count_time_wait_ok % 10 + '0');
// USART1TxStr("\r\n");
Flag_usart1_receive_OK = 0;
delay_ms(100);
if(Flag_device_error)
return 1;//超时返回 1
else
return 0;//正常返回 0
}
unsigned char Query(char * src,char * des,unsigned int LEN)
//查询数组有无包含该字符串，有则返回 1，无则返回 0
{
unsigned int y= 0,len= 0,n= 0;
unsigned char Result = 0;
char * i;
i = des;
for(; *i != '\0';i ++,len ++){}// 判断需要检测的字符的长度
for(y = 0; y < LEN - len;y ++)
//开始检测，次数为总长度减去字符长度的字节数
{
for(n = 0;n < len;n ++)
{
if(*(src + y + n) == *(des + n))
//开始检测双方的第一个字节，如果相等则结果等于 1，并且继续检测双方的第二个字节
{
Result = 1;
}
else
{
Result = 0; //不相等则结果等于 0，并且退出此次循环，
//开始检测数组的第二个字节和字符的第一个字节
break;
}
}
if(n == len)
{
return Result;
}
}
return Result;
}

void trim_string(char *str)
{
	int len = strlen(str);
 
	//去掉最后的换行符
	if(str[len-1] == '\n')
	{
		len--;		//字符串长度减一
		str[len] = 0;	//给字符串最后一个字符赋0，即结束符
	}
	if(str[len-1] == '\r')
	{
		len--;		//字符串长度减一
		str[len] = 0;	//给字符串最后一个字符赋0，即结束符
	}
}
void SendData(){
	int id ;
	cJSON* cjson;
	cJSON* payload;
	cJSON* setRUN;
	char tipStirng[25]={0};
	char tempString[255]={0};
	char jsonTemp[255]={0};
	int index;
int n1=0;
IntToChar(seqNum,seqNum1);
temp=GetTemp();
sprintf(NB_Send_buf1,"AT+QMTPUB=0,0,0,0,\"upload_data_flag\",\"{\"Test_flag\":\"%.2f\"}\"\r\n",temp);
if(!Wait_Str1_Str2_x_100ms(2,1,1,"+QMTRECV:","",20)){
	strcpy(Receive_BUF,USART2_RX_BUF);
	
	USART1TxStr("\r\n");
	get_sub_str((char *)Receive_BUF,",","\r\n",2,Receive_Data);
	
	
	for(index=1;index<strlen(Receive_Data)-1;index++){
		tempString[index-1]=Receive_Data[index];
	}
	//USART1TxStr(tempString);
	
	cjson = cJSON_Parse(tempString);
	id = cJSON_GetObjectItem(cjson,"taskId")->valueint;
	payload=cJSON_GetObjectItem(cjson,"payload");
	setRun=cJSON_GetObjectItem(payload,"run_flag")->valueint;
	sprintf(jsonTemp,"test:%d",setRun);
	USART1TxStr(jsonTemp);//打包成功调用cJSON_Print打印输出
	//LCD_ShowString(8*2,16*12,setRun);
	
	cJSON_Delete(cjson);
	cJSON_Delete(payload);

}
if(setRun==0){
	return;
}
//USART2TxStr("AT\r\n");//测试 NB 模块
//USART1TxStr(NB_Send_buf1);
//USART1TxStr("\r\n");

USART2TxStr((char*)NB_Send_buf1);//通过 NB 模块发送数据
LCD_ShowString(8*2,16*12,"send....");
if(!Wait_Str1_Str2_x_100ms(2,1,1,"OK","",20)) //如果收到发送成功返回的数据
{
n1 = 0;
G_LED_ON;
Y_LED_OFF;
R_LED_OFF;
Flag_Need_Init_Nbiot = 0;
LCD_ShowString(8*2,16*12,"send ok!");

sprintf(tipStirng,"Temperature: %.2f",temp);
LCD_ShowString(8*15,16*12,tipStirng);
}

else //发送数据三次都失败
{
G_LED_OFF;
Y_LED_OFF;
R_LED_ON;
Flag_Need_Init_Nbiot = 0;
LCD_ShowString(8*2,16*12,"Fail ! ");
}

for(i = addr;i < 355;i ++)
{
NB_Send_buf1[i] = 0;
}
CLR_Buf2();

}
float GetTemp(){
	int temp;
	float temp1;
	int TempInt;
	float TempFloat;
	unsigned long adcx;
	adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);
	TempFloat= temp1=(float)adcx*(3.3/4096);
	
	temp1-=(u8)temp1;
	TempFloat=(1.43-TempFloat)/0.0043+25;
	TempInt=temp1*1000;
	
	return TempFloat;
}
void IntToChar(int num,char* dest){
    char str2[25];
    sprintf(str2,"%d",num);
//    printf("%s",str2);
    strcpy(dest,str2);

}
/*
* 取两个分隔符之间的子串
* str  原始字符串
* separator1  第一个分隔符
* separator2  第二个分隔符
* num  字符串出现几次时，作为第一个分隔符的位置。
* substr  取出的两个分隔符间的子串，从0起数。"+QMTRECV: 0,0,"Hello",hello world"
*/
uint8_t get_sub_str(char * str,char * separator1,char * separator2,int8_t num, char * substr)
{
	int8_t i;
	char *p1,*p2;

	p1 = str;
	for(i = 0; i<=num; i++) {
		p1 = strstr(p1,separator1);
		if(p1 == NULL) return 1;
		p1++;
	}

	p2 = strstr(p1,separator2);
	if(p2 == NULL) return 2;
	memcpy(substr,p1,p2-p1);

	return 0;
}



