#ifndef __UPPER_COMPUTER_H
#define __UPPER_COMPUTER_H

#include "stm32f10x.h"

extern unsigned char DataScope_OutPut_Buffer[42];	   //������֡���ݻ�����




void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // дͨ�������� ������֡���ݻ�����
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // ����֡�������ɺ��� 
#endif 







