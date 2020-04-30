/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 * 
 * \brief 
 *    HT1621B�ײ�����
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

#define HT1621_TYPE_CMD		0x04		// ��������
#define HT1621_TYPE_DATA	0x05		// ��������

#define HT1621_TYPE_LEN		3			// ����λ��

#define HT1621_BITLEN_CMD	8			// ����λ��
#define HT1621_BITLEN_ADDR	6			// ��ַλ��

#define HT1621_BIAS_1_3		0x28		// 1/3bias 4com
#define HT1621_RC256K		0x18		// ʹ��ϵͳ�ڲ�RC���� 
#define HT1621_XTAL32K		0x14		// ʹ��ϵͳ����		
#define HT1621_SYSDIS		0x00		// �ر�ϵͳ�𵴼�LCDƫ������
#define HT1621_WDTDIS		0x05		// ��ֹϵͳ���Ź�
#define HT1621_SYSEN		0x01		// ��ϵͳ�𵴼�LCDƫ������
#define HT1621_LCDON		0x03		// ��ϵͳ�� 
#define HT1621_LCDOFF       0x02        // �ر�ϵͳ��

void BSP_LCD_LoadConfig(HT1621Config *handler);
// �ϵ�
void BSP_LCD_PowerOn(void);
// �ϵ�
void BSP_LCD_PowerOff(void);
// ��ʼ��
void BSP_LCD_Init(void);
// �ڵ�ģʽ
void BSP_LCD_Close(void);
// �ر�
void BSP_LCD_WriteData(unsigned char addr, const unsigned char *data, unsigned char len);

#endif

#endif

