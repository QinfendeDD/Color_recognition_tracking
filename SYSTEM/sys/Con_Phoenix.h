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
	float Position_KP,	Position_KI,		Position_KD;	//用于位置PID控制器的PID参数
	float Increment_KP,	Increment_KI;									//用于增量PI控制器的PI参数
	int 	Bias_LR,		 	Bias_UD;											//横向环偏差Bias_LR:左右LR:leftright，纵向环偏差Bias_UD:上下UD:updown
	int 	User_X, 			User_Y;												//用户期望值
	int 	Feedback_X,		Feedback_Y;										//反馈值
	
	int 	PID_LR_current_out,		PID_UD_current_out;		//当前时刻的PID输出
	int 	PWM_value_LR,					PWM_value_UD;					//横向环PWM最终加载值--左右LR:leftright		纵向环PWM最终加载值--上下UD:updown
	
	float KI_magnify,		KD_magnify;
}PID_Phoenix;
extern PID_Phoenix	PID;

#endif






