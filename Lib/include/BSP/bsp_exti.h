/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--exti driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/

#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#include "bsp.h"

#ifdef HAVE_EXTI

/*! \brief exti confirgure Definition */
typedef struct _EXTIConfig{
    PortTypeDef gpio;       /*!< \brief exti port */
    u8          trigger;    /*!< \brief exti trigger mode */
}EXTIConfig;

typedef enum _eEXTIRet{
	E_EXTI_OK = 0,
	E_EXTI_ERR_NOT_DEFINED_PORT,
	E_EXTI_ERR_NOT_DEFINED_PIN,
}eEXTIRet;

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \param cnt[IN]            - extern interrupt count
* \note
*      1.exti port confirure array must be ended with "NULL".
*      ex.
*           __far PortTypeDef exti_handler[4] = {
*               {PORTA, PIN0},      // EXTI 1
*               {PORTA, PIN1},      // EXTI 2
*               {PORTA, PIN2},      // EXTI 3
*            };
*/
void BSP_EXTI_LoadConfig(EXTIConfig *handler, u8 cnt);

/*! \brief
*      exti initilization
*/
u8 BSP_EXTI_Init(void);

/*! \brief
*      exti initilization single port
*/
u8 BSP_EXTI_InitSingle(u8 port, u8 pin, uint8_t mode);

/*! \brief
*      extern interrupt pin state
* \param idx[IN]        extern interrupt pin array index
* \return
*      0    - low level
*      1    - high level
*/
u8  BSP_EXTI_ReadState(u8 idx);

/*! \brief
*      extern interrupt pin state
* \param idx[IN]        extern interrupt pin array index
* \return
*      0    - low level
*      1    - high level
*/
u8  BSP_EXTI_ReadStateSingle(u8 port, u8 pin);

#endif

#endif
