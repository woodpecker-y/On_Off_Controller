#ifndef _BSP_PVD_H
#define _BSP_PVD_H

#ifdef HAVE_PVD

#include "stm32f10x.h"

/*! \brief
*       PVD Init
*/
void bsp_pvd_init(void);

/*! \brief
 *       ���PVD�͵�ѹ״̬��־
 * \return
 *		0 	- ��ѹ����
 *		1 	- �͵�ѹ
 */
u8 bsp_pvd_get_state(void);

/*! \brief
 *       PCD�жϴ�������������void PVD_IRQHandler(void)�жϺ����м���
 * \return
 *		0 	- ��ѹ����
 *		1 	- �͵�ѹ
 */
void bsp_pvd_isr(void);



#endif

#endif

