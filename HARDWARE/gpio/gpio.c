#include "gpio.h"
void GPIO_Configuration(void)//GPIO ��ʼ��
{
GPIO_InitTypeDef GPIO_InitStructure; 
/*��ʼ���ṹ��*/ 
GPIO_StructInit(&GPIO_InitStructure);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ�� PA �˿�ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ�� PC �˿�ʱ��
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ�� PC �˿�ʱ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� PA.0
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� PA.1
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ�� PA.4������ NB ģ�鿪��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� PB.5�����ƴ���������
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ�� PC.13�����ƺ�ɫ R_LED
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ�� PC.14��������ɫ G_LED
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ�������
GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ�� PC.15�����ƻ�ɫ Y_LED
R_LED_OFF;
G_LED_OFF;
Y_LED_OFF;
}
