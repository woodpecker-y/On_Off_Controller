#ifndef _BSP_WDG_H
#define _BSP_WDG_H

/*! \brief
*       内部看门狗初始化
* \param _1s[IN]			- 看门狗溢出时间
*/
void BSP_WDG_Init(unsigned char _1s);

/*! \brief
*       内部看门狗喂狗
*/
void BSP_WDG_Reset(void);

#endif

