/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--AD sample driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    ��������:
*       |-->BSP_ADC_Init()
*           |-->BSP_ADC_SampleInit()
*               |-->BSP_ADC_SampleOnce()
*               |-->...
*               |-->BSP_ADC_SampleOnce()
*           |-->BSP_ADC_SampleFinish()
*       |-->BSP_ADC_Close()
* Copyright (c) 2010-2013, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _ADC_H
#define _ADC_H

#include "bsp.h"
#include "bsp_adc.h"

#ifdef HAVE_ADC

/*! \brief
*       Load ADC Sample port configure
* \param handler[IN]        - adc sample configure handler
*/
void adc_load_config(ADCConfig *handler, u8 num);

/*! \brief
*       adc sample all pin initilization
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void adc_power_on(u8 idx);
void adc_power_on_opendrain(u8 idx);

/*! \brief
*       adc sample all pin close
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void adc_power_off(u8 idx);
void adc_power_off_opendrain(u8 idx);

/*! \brief
*       adc sample initilization
* \param adc_channel[IN]        - adc samle channel
* \return
*       none
*/
int adc_init(u8 idx);
/*! \brief
 *       adc sample initilization with VREFINT Channel
 * \return
 *       0  - successful
 */
int adc_init_vrefint(void);

/*! \brief
*       adc sample once
* \return
*       adc sample value
* \note
*       1���������BSP_ADC_SampleInit֮��ſ���ʹ�ã�
*       2��BSP_ADC_SampleInit��ʼ����Ϻ������ʱ����25us���ܻ�ȡ��ȷֵ��
*/
u32 adc_sample_once(u8 idx);

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1���������BSP_ADC_SampleInit֮��ſ���ʹ�ã�
 *       2��BSP_ADC_SampleInit��ʼ����Ϻ������ʱ����25us���ܻ�ȡ��ȷֵ��
 */
u32 adc_sample(u8 idx);

/*! \brief
*       ��VREFINTͨ������һ��AD����
* \return
*       AD����ֵ
* \note
*       1���������BSP_ADC_Init_Vrefint֮��ſ���ʹ�ã�
*       2��BSP_ADC_Init_Vrefint��ʼ����Ϻ������ʱ����25us���ܻ�ȡ��ȷֵ��
*/
u32 adc_sample_once_vrefint(void);

/*! \brief
 *       adc sample 8 cycle,get average
 * \return
 *       adc sample value
 * \note
 *       1���������BSP_ADC_SampleInit֮��ſ���ʹ�ã�
 *       2��BSP_ADC_SampleInit��ʼ����Ϻ������ʱ����25us���ܻ�ȡ��ȷֵ��
 */
u32 adc_sample_vrefint(void);

/*! \brief
*       adc sample finish
* \param adc_channel[IN]        - adc sample channel
*/
void adc_close(u8 idx);

/*! \brief
*       AD��������
* \param adc_channel[IN]        ����ͨ��
*/
void adc_close_vrefint(void);

#endif

#endif
