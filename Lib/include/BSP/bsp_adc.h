/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--AD sample driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    调用流程:
*       |-->BSP_ADC_Init()
*           |-->BSP_ADC_SampleInit()
*               |-->BSP_ADC_SampleOnce()
*               |-->...
*               |-->BSP_ADC_SampleOnce()
*           |-->BSP_ADC_SampleFinish()
*       |-->BSP_ADC_Close()
* Copyright (c) 2010-2013, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _STM8L151_ADC1_H
#define _STM8L151_ADC1_H

#include "bsp.h"

#ifdef HAVE_ADC

typedef enum _E_ADC_Ret{
    E_ADC_OK = 0,
    E_ADC_ERR_EXCEED_MAX_IDX,
    E_ADC_ERR_UNKNOWN_ADC,
}E_ADC_Ret;

typedef enum _E_ADC_Idx{
  E_ADC1 = 1,
  E_ADC2,
  E_ADC3
}E_ADC_Idx;

typedef struct _ADCConfig{
    u8              adc;
    PortTypeDef     ctrl;
    PortTypeDef     ai;
    u8              pwr_le;
}ADCConfig;

/*! \brief
*       Load ADC Sample port configure
* \param handler[IN]        - adc sample configure handler
*/
void BSP_ADC_LoadConfig(ADCConfig *handler, u8 num);

/*! \brief
*       adc sample all pin initilization
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void BSP_ADC_PowerOn(u8 idx);
void BSP_ADC_PowerOn_OpenDrain(u8 idx);

/*! \brief
*       adc sample all pin close
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void BSP_ADC_PowerOff(u8 idx);
void BSP_ADC_PowerOff_OpenDrain(u8 idx);

int BSP_ADC_Init(u8 idx);
int BSP_ADC_Init_Single(u8 adc_idx, u8 port, u8 pin);
/*! \brief
*       进行一次AD采样
* \return
*       AD采样值
* \note
*       1、必须调用BSP_ADC_SampleInit之后才可以使用；
*       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 BSP_ADC_SampleOnce(u8 idx);
u32 BSP_ADC_SampleOnceSingle(u8 adc_idx, u8 port, u8 pin);
/*! \brief
*       AD采样结束
* \param adc_channel[IN]        采样通道
*/
void BSP_ADC_Close(u8 idx);
void BSP_ADC_CloseSingle(u8 adc_idx, u8 port, u8 pin);

#endif

#endif
