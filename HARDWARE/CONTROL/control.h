#ifndef __gfp_control_H
#define __gfp_control_H	

#include "stm32f10x.h"



void OV7725_7670_Camera_Refresh_Phoenix(void);
void Key_Modify_Phoenix(void);

void OV7725_camera_refresh(void);
void OV7670_camera_refresh(void);
void Ov7725_7670_Choose_Init(void);	

void Draw_Sign_Cross(u16 x,u16 y);
void Color_Capture_Reconfigure(void);
void color_Value(u16 x,u16 y);

void PID_LR_figure_Phoenix(void);
void PID_UD_figure_Phoenix(void);
void Feature_Recognize_Start_Phoenix(void);

void Constant_Init(void);
void PWM_TEST(void);

#endif


