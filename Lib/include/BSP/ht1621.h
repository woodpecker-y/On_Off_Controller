/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 * 
 * \brief 
 *    HT1621B底层驱动
 * \author 
 *    yujh_empty@foxmail.com 
 * \date 
 *    2011-09-06
 * \version 
 *    v1.0
 * Copyright (c) 2011, yujh_empty@foxmail.com Corporation All rights reserved.
 ******************************************************************************/
#ifdef HAVE_HT1621

#ifndef _HT1621_H
#define _HT1621_H

#include <stdio.h>
#include <string.h>
#include "bsp_gpio.h"
//#include "stm8l15x_bsp.h"

/*! \brief LCD Extern Chip Port Configure Definition */
typedef struct _HT1621Config{
    PortTypeDef pwr;        /*!< \brief HT1621 Power Pin */
    PortTypeDef cs;         /*!< \brief HT1621 CS Pin */
    PortTypeDef rd;         /*!< \brief HT1621 RD Pin */
    PortTypeDef wr;         /*!< \brief HT1621 WR Pin */
    PortTypeDef dat;        /*!< \brief HT1621 DATA Pin */
}HT1621Config;	

#define HT1621_TYPE_CMD		0x04		// 命令类型
#define HT1621_TYPE_DATA	0x05		// 数据类型

#define HT1621_TYPE_LEN		3			// 类型位长

#define HT1621_BITLEN_CMD	8			// 命令位长
#define HT1621_BITLEN_ADDR	6			// 地址位长

#define HT1621_BIAS_1_3		0x28		// 1/3bias 4com
#define HT1621_RC256K		0x18		// 使用系统内部RC震荡器 
#define HT1621_XTAL32K		0x14		// 使用系统晶振		
#define HT1621_SYSDIS		0x00		// 关闭系统震荡及LCD偏振发生器
#define HT1621_WDTDIS		0x05		// 禁止系统看门狗
#define HT1621_SYSEN		0x01		// 打开系统震荡及LCD偏振发生器
#define HT1621_LCDON		0x03		// 打开系统震荡 
#define HT1621_LCDOFF       0x02        // 关闭系统震荡

void BSP_LCD_LoadConfig(HT1621Config *handler);
// 上电
void BSP_LCD_PowerOn(void);
// 断电
void BSP_LCD_PowerOff(void);
// 初始化
void BSP_LCD_Init(void);
// 节电模式
void BSP_LCD_Close(void);
// 关闭
void BSP_LCD_WriteData(unsigned char addr, const unsigned char *data, unsigned char len);

#endif

#endif

