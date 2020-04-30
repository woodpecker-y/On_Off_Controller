/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--ADF driver head files
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_H
#define _BSP_H

typedef enum _LevelEffType{
    E_LE_LOW = 0,
    E_LE_HIGH,
}LevelEffType;

//////////////////////////////ϵͳͷ�ļ�////////////////////////////////
#include <stdio.h>
#include <string.h>

#include "pvd.h"


//////////////////////////////оƬͷ�ļ�////////////////////////////////
#include "stm32f10x.h"

//////////////////////////////��������////////////////////////////////
#include "bsp_com.h"

//////////////////////////////�˿�����////////////////////////////////
#include "bsp_gpio.h"

//////////////////////////////����������////////////////////////////////
#include "rc522.h"

//////////////////////////////RTC����////////////////////////////////
#include "bsp_rtc.h"

//////////////////////////////LED����////////////////////////////////
#include "bsp_led.h"
#include "led.h"

//////////////////////////////SI4432 RF433����////////////////////////////////
#include "si4432.h"
#include "rf.h"

//////////////////////////////BEEP����////////////////////////////////
#include "bsp_beep.h"

//////////////////////////////��������////////////////////////////////
#include "bsp_key.h"

//////////////////////////////�ⲿ�ж�����////////////////////////////////
#include "bsp_exti.h"

//////////////////////////////��ʱ������////////////////////////////////
#include "bsp_systimer.h"

//////////////////////////////��������////////////////////////////////
#include "bsp_flash.h"
#include "bsp_flash_ext.h"
#include "flash.h"

//////////////////////////////�����ѹ��⺯��////////////////////////////////
#include "bsp_pvd.h"
#include "pvd.h"

//////////////////////////////LCD����///////////////////////////////
#include "ht1621.h"
#include "lcd.h"

//////////////////////////////Һ������////////////////////////////////
//#include "st7565r.h"

//////////////////////////////��������////////////////////////////////
//#include "font.h"

//////////////////////////////��������////////////////////////////////
#include "ba6289.h"
#include "motor.h"

//////////////////////////////���Ź�����////////////////////////////////
#include "bsp_wdg.h"

//////////////////////////////��ʱ����////////////////////////////////
#include "delay.h"

#endif