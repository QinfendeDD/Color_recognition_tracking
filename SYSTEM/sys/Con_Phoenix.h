#ifndef	_Con_Phoenix_H
#define _Con_Phoenix_H
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7725.h"
#include "ov7670.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "colorcfg.h"
#include "con_Phoenix.h"
#include "pid.h"
#include "control.h"
#include "Upper_Computer.h"
#include "show.h"
typedef struct
{
	float Position_KP,	Position_KI,		Position_KD;	//����λ��PID��������PID����
	float Increment_KP,	Increment_KI;									//��������PI��������PI����
	int 	Bias_LR,		 	Bias_UD;											//����ƫ��Bias_LR:����LR:leftright������ƫ��Bias_UD:����UD:updown
	int 	User_X, 			User_Y;												//�û�����ֵ
	int 	Feedback_X,		Feedback_Y;										//����ֵ
	
	int 	PID_LR_current_out,		PID_UD_current_out;		//��ǰʱ�̵�PID���
	int 	PWM_value_LR,					PWM_value_UD;					//����PWM���ռ���ֵ--����LR:leftright		����PWM���ռ���ֵ--����UD:updown
	
	float KI_magnify,		KD_magnify;
}PID_Phoenix;
extern PID_Phoenix	PID;

#endif






