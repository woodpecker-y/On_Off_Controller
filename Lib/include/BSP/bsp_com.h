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

/*! \brief ���ں� */
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

/*! \brief ����������Ϣ */
typedef struct _COMConfig{
	COM		com;
	PortTypeDef	tx;
	PortTypeDef	rx;
	PortTypeDef	ctrl;
	uint8_t*	rcv_buffer_ptr;
	uint16_t	max_rcv_buf_size;
}COMConfig;

/*! \brief ���ڽӿڷ����� */
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
* \param com_cfg[IN]        - �������ýṹ��
* \param com_cfg_cnt[IN]	- ������������		
*/
void BSP_COM_LoadConfig(COMConfig *com_cfg, uint8_t com_cfg_cnt);

/*! \brief
*       ���ڳ�ʼ��
* \param com[IN]					- ���ں�
* \param baudrate[IN]				- ������
* \param wordlength[IN]				- ����λ
* \param stopbits[IN]				- ֹͣλ
* \param mode[IN]					- ����ģʽ
* \param rx_interrupt_state[IN]		- �����ж�״̬
* \param rx_interrupt_prority[IN]	- �����ж����ȼ�
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*		E_COM_ERR_NOT_SUPPORT_COM		- ��֧�ֵĴ���
*/
uint8_t BSP_COM_Init(COM com, uint32_t baudrate, uint16_t wordlength, uint16_t stopbits, uint16_t parity, uint16_t mode, uint8_t rx_interrupt_state, uint8_t  rx_interrupt_prority);

uint8_t BSP_COM_Close(COM com);

/*! \brief
*       ��ȡ���ڽ������ݱ�ʶ
* \param com[IN]					- ���ں�
* \return
*		RESET						- ������
*		SET							- ������
*/
uint8_t BSP_COM_GetRxFlag(COM com);

/*! \brief
*       ���ڳ�ʼ��
* \param com[IN]					- ���ں�
* \param ch[IN]						- һ���ֽ�
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*		E_COM_ERR_NOT_SUPPORT_COM		- ��֧�ֵĴ���
*/
eCOMRet BSP_COM_RecvChar(COM com, uint16_t *dat);

/*! \brief
*       ���ڳ�ʼ��
* \param com[IN]					- ���ں�
* \param ch[IN]						- һ���ֽ�
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*		E_COM_ERR_NOT_SUPPORT_COM		- ��֧�ֵĴ���
*/
uint8_t BSP_COM_SendChar(COM com, uint8_t ch);

/*! \brief
*       ���ڳ�ʼ��
* \param com[IN]					- ���ں�
* \param dat[IN]					- �ֽ�����
* \param len[IN]					- �ֽ����ݳ���
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*		E_COM_ERR_NOT_SUPPORT_COM		- ��֧�ֵĴ���
*/
uint8_t BSP_COM_SendData(COM com, uint8_t *dat, uint16_t len);

/*! \brief
*       ���ڳ�ʼ��
* \param com[IN]					- ���ں�
* \param str[IN]					- �ַ���
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*		E_COM_ERR_NOT_SUPPORT_COM		- ��֧�ֵĴ���
*/
uint8_t BSP_COM_SendStr(COM com, char *str);

/*! \brief
*       ���ڽ��ճ�ʼ��
* \param com[IN]					- ���ں�
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
*/
uint8_t BSP_COM_RecvInit(COM com);

///*! \brief
//*       ���ڽ�������
//* \param com[IN]						- ���ں�
//* \param dat[OUT]						- ���ջ�����
//* \param len[IN/OUT]					- ���ݵĳ���
//* \return
//*		E_COM_OK						- �ɹ�
//*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
//* \note
//*		����len����Ϊ���������Ҳ��Ϊ�����������Ϊ�������ʱ��Ҫָ�����ջ������Ĵ�С����Ϊ�������ʱ�������յ�����ʵ�ʳ���
//*/
//uint8_t BSP_COM_RecvData(COM com, uint8_t *dat, uint16_t *len);

/*! \brief
*       ���ڽ�������
* \param com[IN]						- ���ں�
* \param ptr[OUT]						- ���ݽ���ָ��
* \param len[OUT]						- ���ݵĳ���
* \return
*		E_COM_OK						- �ɹ�
*		E_COM_ERR_NOT_DEFINED_COM_ID	- δ����Ĵ���
* \note
*		����ptr��һ������ָ�룬��ʹ��ʱ����ҪΪptr�����ڴ�ռ䣬����һ����ָ�뼴�ɣ����ʱ����������������ݻ��������׵�ַ��
*		ʹ�ð�����
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
