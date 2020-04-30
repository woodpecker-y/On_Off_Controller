#ifndef _BSP_PVD_H
#define _BSP_PVD_H

#ifdef HAVE_PVD

#include "stm32f10x.h"

/*! \brief
*       PVD Init
*/
void bsp_pvd_init(void);

/*! \brief
 *       获得PVD低电压状态标志
 * \return
 *		0 	- 电压正常
 *		1 	- 低电压
 */
u8 bsp_pvd_get_state(void);

/*! \brief
 *       PCD中断处理函数，放置在void PVD_IRQHandler(void)中断函数中即可
 * \return
 *		0 	- 电压正常
 *		1 	- 低电压
 */
void bsp_pvd_isr(void);



#endif

#endif

