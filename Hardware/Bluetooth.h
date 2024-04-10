#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include <stdio.h>

void Bluetooth_Init(void);
//����
void Bluetooth_SendByte(uint8_t Byte);
void Bluetooth_SendArray(uint8_t *Array,uint16_t Length);
void Bluetooth_SendString(char *String);
uint32_t Bluetooth_Pow(int x,int y);
void Bluetooth_SendNumber(uint32_t Number,uint8_t Length);
//����   
uint8_t Bluetooth_GetRxData(void);
uint8_t Bluetooth_GetRxFlag(void); //��ȡ�����־λ������

#endif
