/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--BA6289 motor driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2013, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BA6289_H
#define _BA6289_H

#ifdef HAVE_MOTOR

#include "bsp.h"

/*! \brief Motor Port Configure Definition */
typedef struct _MotorConfig{
    PortTypeDef pwr;        /*!< \brief motor power */
    PortTypeDef fin;        /*!< \brief motor fin pin */
    PortTypeDef rin;        /*!< \brief motor rin pin */
    PortTypeDef state_close;/*!< \brief motor close state */
    PortTypeDef state_open; /*!< \brief motor open state */
    PortTypeDef ai;         /*!< \brief motor valve position check */
    u8          adc;        /*!< \brief motor valve position check adc idx */
}MotorConfig;

/*! \brief Motor State Definition */
typedef enum _MotorState{
    MOTOR_STATE_CLOSED = 1, /*!< \brief motor is closed */
    MOTOR_STATE_OPENED = 0, /*!< \brief motor is opened max */
    MOTOR_STATE_OTHERS = 2, /*!< \brief motor is in other state */
}MotorState;

/*! \brief Motor Run Direction Definition */
typedef enum _MotorDirection{
    MOTOR_DIR_FORWARD = 0,  /*!< \brief motor run forward */
    MOTOR_DIR_REVERSE = 1,  /*!< \brief motor run reverse */
    MOTOR_DIR_UNKNOWN = 2,  /*!< \brief motor dir unknown */
}MotorDirection;

typedef enum _MotorSwitch{
    E_MOTOR_SWITCH_OPENED = 0,
    E_MOTOR_SWITCH_CLOSED = 1,
}MotorSwitch;

/*! \brief
*      Load Motor Port Configure
* \param handler[IN]        - board motor port configure handler
*/
void BSP_MOTOR_LoadConfig(const MotorConfig *handler);

/*! \brief
*       motor power on
*/
void BSP_MOTOR_PowerOn(void);

/*! \brief
*       motor power off
*/
void BSP_MOTOR_PowerOff(void);

/*! \brief
*       motor initilization
*/
void BSP_MOTOR_Init(void);
void BSP_MOTOR_InitIPU(void);

/*! \brief
*       motor close
*/
void BSP_MOTOR_Close(void);

/*! \brief
*       motor run forword
* \note
*       when FIN is 1 and RIN is 0,the motor run forword.
*/
void BSP_MOTOR_Forword(void);

/*! \brief
*       motor run reverse
* \note
*       when FIN is 0 and RIN is 1, the motor run reverse.
*/
void BSP_MOTOR_Reverse(void);

/*! \brief
*       motor brake
* \note
*       when FIN is 1 and RIN is 1, the motor is braked.
*/
void BSP_MOTOR_Brake(void);

/*! \brief
*       motor standby
* \note
*       when FIN is 0 and RIN is 0, the motor standby.
*/
void BSP_MOTOR_Standby(void);

/*! \brief
*       get motor run state
* \return
*       MOTOR_STATE_CLOSED      - motor is closed
*       MOTOR_STATE_OPEND       - motor is opened max
*       MOTOR_STATE_OTHERS      - motor is in other state
*/
u8   BSP_MOTOR_GetState(void);

///*! \brief
//*       set motor run state
//* \param idx[IN]
//*    E_MOTOR_SWITCH_OPENED,
//*    E_MOTOR_SWITCH_CLOSED,
//*/
//u8 BSP_MOTOR_SetState(u8 idx);

/*! \brief
*       motor run by direction
* \param dir[IN]            - Motor run direction
*        MOTOR_DIR_FORWORD  - Run forword, the state of motor should be closed.
*        MOTOR_DIR_REVERSE  - Run Reverse, the state of motor should be opened.
*/
void BSP_MOTOR_Run(u8 dir);

/*! \brief
*       motor stop
* \note
*       motor should be stoped when the state of motor is closed or opened max.
*/
void BSP_MOTOR_Stop(void);

/*! \brief
*       motor get position
* \return
*       valve position adc value
* \note
*       motor should be stoped when the state of motor is closed or opened max.
*/
u32 BSP_MOTOR_GetPosition(void);

#endif

#endif
