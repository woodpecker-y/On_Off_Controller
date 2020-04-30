/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--usart driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_COM_H
#define _BSP_COM_H

#include "bsp.h"
#ifdef HAVE_COM

#include "stm32f10x.h"
#include "bsp_gpio.h"

/*! \brief 串口号 */
typedef enum _eCOM{
	COM1 = 0, 
	COM2, 
	COM3, 
	COM4, 
	COM5, 
	COM6, 
	COM7, 
	COM8
}COM;

/*! \brief 串口配置信息 */
typedef struct _COMConfig{
	COM		com;
	PortTypeDef	tx;
	PortTypeDef	rx;
	PortTypeDef	ctrl;
	uint8_t*	rcv_buffer_ptr;
	uint16_t	max_rcv_buf_size;
}COMConfig;

/*! \brief 串口接口反馈码 */
typedef enum _eCOMRet{
	E_COM_OK = 0,
	E_COM_ERR_NOT_DEFINED_COM_ID,
	E_COM_ERR_NOT_SUPPORT_COM,
	E_COM_ERR_INPUT_NULL_VALUE,
	E_COM_ERR_NO_DATA,
	E_COM_ERR_NOT_ENOUGH_MEMORY,
}eCOMRet;

/*! \brief
*       com confirure handler
* \param com_cfg[IN]        - 串口配置结构体
* \param com_cfg_cnt[IN]	- 串口配置数量		
*/
void BSP_COM_LoadConfig(COMConfig *com_cfg, uint8_t com_cfg_cnt);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param baudrate[IN]				- 波特率
* \param wordlength[IN]				- 数据位
* \param stopbits[IN]				- 停止位
* \param mode[IN]					- 工作模式
* \param rx_interrupt_state[IN]		- 接收中断状态
* \param rx_interrupt_prority[IN]	- 接收中断优先级
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t BSP_COM_Init(COM com, uint32_t baudrate, uint16_t wordlength, uint16_t stopbits, uint16_t parity, uint16_t mode, uint8_t rx_interrupt_state, uint8_t  rx_interrupt_prority);

uint8_t BSP_COM_Close(COM com);

/*! \brief
*       获取串口接收数据标识
* \param com[IN]					- 串口号
* \return
*		RESET						- 无数据
*		SET							- 有数据
*/
uint8_t BSP_COM_GetRxFlag(COM com);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param ch[IN]						- 一个字节
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
eCOMRet BSP_COM_RecvChar(COM com, uint16_t *dat);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param ch[IN]						- 一个字节
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t BSP_COM_SendChar(COM com, uint8_t ch);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param dat[IN]					- 字节数据
* \param len[IN]					- 字节数据长度
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t BSP_COM_SendData(COM com, uint8_t *dat, uint16_t len);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param str[IN]					- 字符串
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t BSP_COM_SendStr(COM com, char *str);

/*! \brief
*       串口接收初始化
* \param com[IN]					- 串口号
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*/
uint8_t BSP_COM_RecvInit(COM com);

///*! \brief
//*       串口接收数据
//* \param com[IN]						- 串口号
//* \param dat[OUT]						- 接收缓冲区
//* \param len[IN/OUT]					- 数据的长度
//* \return
//*		E_COM_OK						- 成功
//*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
//* \note
//*		参数len即作为输入参数，也作为输出参数，作为输入参数时需要指定接收缓冲区的大小，作为输出参数时反馈接收的数据实际长度
//*/
//uint8_t BSP_COM_RecvData(COM com, uint8_t *dat, uint16_t *len);

/*! \brief
*       串口接收数据
* \param com[IN]						- 串口号
* \param ptr[OUT]						- 数据接收指针
* \param len[OUT]						- 数据的长度
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
* \note
*		参数ptr是一个二级指针，在使用时不需要为ptr分配内存空间，定义一个空指针即可，输出时返回驱动层接收数据缓冲区的首地址。
*		使用案例：
*			uint8_t *rcv_ptr = NULL;
*			uint16_t rcv_len = 0;
*			
*			BSP_COM_RecvDataPtr(COM1, &rcv_ptr, &rcv_len);
*/
uint8_t BSP_COM_RecvData(COM com, uint8_t **ptr, uint16_t *len);

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       >0          read data successfully.
*       =0          no data
*/
u8   BSP_COM_RecvDataStream(COM com, u8 **pval, u16 *len);

void BSP_COM_ISR(COM com);

u8 BSP_COM_LprRecvData(COM com, uint8_t **ptr, uint16_t *len);

#endif

#endif
