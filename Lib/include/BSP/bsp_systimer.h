#ifndef _BSP_SYSTIMER_H
#define _BSP_SYSTIMER_H

#include "bsp.h"
#include "stm32f10x.h"

#ifdef HAVE_SYSTIMER

/*! \brief support max timer counter */
#define MAX_TIMER_CNT	17

/*! \brief 定时器反馈码 */
typedef enum _SysTimerRet{
	E_SYSTIMER_OK = 0,
	E_SYSTIMER_ERR_IMPOSSIBLE_VALUE,
}SysTimerRet;

/*! \brief 定时句柄工作状态 */
typedef enum _SysTimerSts{
	E_TIMER_IDLE		= 0,		/*!< \brief 空闲 */
	E_TIMER_BUSY		= 1,		/*!< \brief 繁忙 */
	E_TIMER_OVF			= 2 		/*!< \brief 结束 */
}SysTimerSts;

/*! \brief 软件定时器ID */
typedef enum _TimerIDX{
	TIMER_T0		= 0,			/*!< \brief 定时器0 */
	TIMER_T1		= 1,			/*!< \brief 定时器1 */
	TIMER_T2		= 2,			/*!< \brief 定时器2 */
	TIMER_T3		= 3,			/*!< \brief 定时器3 */
	TIMER_T4		= 4,			/*!< \brief 定时器4 */
	TIMER_T5		= 5,			/*!< \brief 定时器5 */
	TIMER_T6		= 6,			/*!< \brief 定时器6 */
	TIMER_T7		= 7,			/*!< \brief 定时器7 */
	TIMER_T8		= 8,			/*!< \brief 定时器8 */
	TIMER_T9		= 9,			/*!< \brief 定时器9 */
	TIMER_T10		= 10,			/*!< \brief 定时器10 */
	TIMER_T11		= 11,			/*!< \brief 定时器11 */
	TIMER_T12		= 12,			/*!< \brief 定时器12 */
	TIMER_T13		= 13,			/*!< \brief 定时器13 */
	TIMER_T14		= 14,			/*!< \brief 定时器14 */
	TIMER_T15		= 15,			/*!< \brief 定时器15 */
	TIMER_SYSTIMER	= 16,			/*!< \brief 系统定时器 */
}TimerIDX;

/*! \brief
*       定时器初始化
*/
u8 BSP_SysTimer_Init(void);

/*! \brief
*       配置定时器初始值
* \param[IN]id				- 定时器索引号	
* \param[IN]val				- 定时器设定参考值			
*/
void BSP_SysTimer_Config(unsigned char id, unsigned long val);

/*! \brief
*       获取定时器状态，当状态为E_TIMER_OVF表明定时器溢出
* \param[IN]id				- 定时器索引号	
* \param[IN]val				- 定时器设定参考值			
*/
u8 BSP_SysTimer_GetOverFlag(unsigned char id);

/*! \brief
*       清除定时器溢出标志
* \param[IN]id				- 定时器索引号				
*/
void BSP_SysTimer_ClrOverFlag(unsigned char id);

/*! \brief
 *       获得定时器的值，方便调试
 * \param[IN]id				- 定时器索引号
 *
 */
u8 BSP_SysTimer_GetTimerState(unsigned char id, unsigned char *sts, unsigned long *val);

/*! \brief
*       定时器中断服务处理函数				
*/
void BSP_SysTimer_ISR(void);

#endif

#endif

