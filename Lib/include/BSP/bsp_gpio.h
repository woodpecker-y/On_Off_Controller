/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--gpio driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "stm32f10x.h"

/*! \brief GPIO Port Redefined */
typedef enum _Port{
	PORT_NULL = 0,
    PORTA = 1,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF,
}Port;

/*! \brief GPIO Pin Redefined */
typedef enum _Pin{
	PIN_NULL = 0,
    PIN0 = 1,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7,
    PIN8,
    PIN9,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15,
}Pin;

/*! \brief ∂Àø⁄≈‰÷√–≈œ¢ */
typedef struct _PortTypeDef{
	u8	port;
	u8	pin;
}PortTypeDef;

typedef enum _ePortRet{
	E_PORT_OK = 0,
	E_PORT_ERR_NOT_DEFINED_PORT,
	E_PORT_ERR_NOT_DEFINED_PIN,
}ePortRet;

/*! \brief
*       gpio pin initilization
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
* \param mode[IN]           - gpio pin init mode.
*/
uint8_t BSP_GPIO_Init(u8 port, u8 pin, uint8_t mode);

/*! \brief
*       set gpio pin to 1
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
*/
uint8_t BSP_GPIO_SetHigh(u8 port, u8 pin);

/*! \brief
*       set gpio pin to 0
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
*/
uint8_t BSP_GPIO_SetLow(u8 port, u8 pin);

/*! \brief
*       read gpio pin state
* \param port[IN]           - gpio port index.
* \param pin[IN]            - gpio pin index.
* \return
*       pin state:0-low level;1-high level
*/
uint8_t  BSP_GPIO_ReadState(u8 port, u8 pin);

#endif

