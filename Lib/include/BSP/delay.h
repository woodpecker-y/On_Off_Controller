/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package-- delay driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _DELAY_H
#define _DELAY_H

#include "bsp.h"

/*! \brief
*       millisecond delay
* \param _1ms[IN]        - delay time
*/
void delay_ms(u16 _1ms);

/*! \brief
*       microseconds delay
* \param us[IN]        - delay time
*/
void delay_us(u16 us);

#endif
