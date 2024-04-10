#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "OLED.h"
uint8_t Bluetooth_RxData;
uint8_t Bluetooth_RxFlag;


void Bluetooth_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_DeInit(USART2);//���ڸ�λ 

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	//RXNE��־λ��1����NVIC�����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//����RXNEλ��NVIC�ж�����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
}

void Bluetooth_SendByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	// USART_FLAG_TXE ==0 ���ݻ�û�б���λ�Ĵ���ת��
	// USART_FLAG_TXE ==1 �����Ѿ�����λ�Ĵ���ת��
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET); //�ȴ�����ת��
}

void Bluetooth_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i = 0;i < Length;i ++)
		Bluetooth_SendByte(Array[i]);
}

void Bluetooth_SendString(char *String)
{
	uint16_t i;
	for(i = 0;String[i] != '0'; i ++)
		Bluetooth_SendByte(String[i]);
}

uint32_t Bluetooth_Pow(int x,int y)
{
	while(y --)
	{
		x *= x;
	}
	return x;
}

void Bluetooth_SendNumber(uint32_t Number,uint8_t Length)
{
	uint16_t i;
	for(i = 0;i < Length;i ++)
	{
		Bluetooth_SendByte(Number / Bluetooth_Pow(10,Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch,FILE*f)  //printf�����ض���
{
	Bluetooth_SendByte(ch);
	return ch;
}

uint8_t Bluetooth_GetRxFlag(void) //��ȡ�����־λ������
{
	if(Bluetooth_RxFlag == 1)
	{
		Bluetooth_RxFlag = 0;
		return 1;
	}	
	return 0;
}

uint8_t Bluetooth_GetRxData(void)
{
	return Bluetooth_RxData;
}


void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // �������жϱ�־λ
    {
        uint8_t data = USART_ReceiveData(USART2); // ����һ���ֽڵ�����
		OLED_ShowChar(1,1,data);
		OLED_ShowChar(3,10,'A');
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // ��������жϱ�־λ
    }
}
