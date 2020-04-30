/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--key driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "bsp.h"
#include "bsp_gpio.h"
#ifdef HAVE_KEY

/*! \brief Key Code definition */
typedef enum _KeyTypeDef{
    KEY_1 = '1',            /*!< \brief key code 1 */
    KEY_2 = '2',            /*!< \brief key code 2 */
    KEY_3 = '3',            /*!< \brief key code 3 */
    KEY_4 = '4',            /*!< \brief key code 4 */
    KEY_5 = '5',            /*!< \brief key code 5 */
    KEY_6 = '6',            /*!< \brief key code 6 */
    KEY_COMB_1 = 'a',       /*!< \brief key combination code 1 */
    KEY_COMB_2 = 'b',       /*!< \brief key combination code 2 */
    KEY_COMB_3 = 'c',       /*!< \brief key combination code 3 */
    KEY_COMB_4 = 'd',       /*!< \brief key combination code 4 */
    KEY_COMB_5 = 'e',       /*!< \brief key combination code 5 */
    KEY_IDLE = 0,           /*!< \brief no key code */
}KeyTypeDef;

typedef enum _eKeyRet{
	E_KEY_OK = 0,
	E_KEY_ERR_INVALID_KEY,
}eKeyRet;

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \note
*      1.key code is one-to-one correspondence with key configure array index,the first key code is KEY_1.
*      2.Key port confirure array must be ended with "NULL".
*      ex.
*           __far PortTypeDef key_handler[4] = {
*               {PORTA, PIN0},      // Key 1
*               {PORTA, PIN1},      // Key 2
*               {PORTA, PIN2},      // Key 3
*            };
*/
void BSP_Key_LoadConfig(PortTypeDef *handler, u8 num);

/*! \brief
*      key initilization
*/
u8 BSP_Key_Init(void);

/*! \brief
*      Get key code by pin number
* \param key_idx[IN]       - key define index 
* \return 
*		key code
*/
u8 BSP_Key_Get(u8 key_idx);

/*! \brief
*      Verify key code is valid
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       E_KEY_ERR_INVALID_KEY   - invalid
*/
u8 BSP_Key_CheckValid(u8 key);

/*! \brief
*      Check whether the key is pressed.
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       E_KEY_ERR_INVALID_KEY   - invalid
*/
u8 BSP_Key_Release(u8 key);

#endif

#endif
