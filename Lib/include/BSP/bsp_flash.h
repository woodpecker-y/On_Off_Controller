#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

#ifdef HAVE_FLASH

#include "bsp.h"

/*! \brief ������ʼ��ַ */
#define FLASH_ADDR_START	0x8000000

/*! \brief ����洢������С���� */
/*! \brief С�������ͼ�С������ֵ���� */
#if	  defined (STM32F10X_LD) || defined (STM32F10X_LD_VL)
#define FLASH_SIZE			0x8000
/*! \brief ���������ͼ���������ֵ���� */
#elif defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
#define FLASH_SIZE			0x10000
/*! \brief ���������ͼ���������ֵ���� */
#elif defined (STM32F10X_HD) || defined (STM32F10X_HD_VL)
#define FLASH_SIZE			0x80000
/*! \brief �������� */
#elif defined (STM32F10X_CL)
#define FLASH_SIZE			0x40000
#else
	#error "Please configure MCU Type. (in Keil uVision4 Options->C/C++->Define)"
#endif

/*! \brief ����ҳ������С���� */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
	#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
	#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

/*! \brief Flash������ */
typedef enum _eFlashRet{
	E_FLASH_OK = 0,
	E_FLASH_ERR_INCORRECT_LENGTH,
	E_FLASH_ERR_WRITE_FAILED,
	E_FLASH_ERR_ERASE_FAILED,
}eFlashRet;

/*! \brief
*       доƬ�ڲ�FLASH
* \param addr[IN]			- д�׵�ַ
* \param dat[IN]			- ����
* \param len[IN]			- ���ݳ���
* \return
*		E_FLASH_OK						- �ɹ�
*		E_FLASH_ERR_INCORRECT_LENGTH	- ���Ȳ���ȷ
*		E_FLASH_ERR_WRITE_UNCOMPLETE	- д��ʧ��
* \notes
*		��flashд�����С����Ϊ2Bytes����˳��ȱ�����2����������
*/
u8  BSP_FLASH_Write(u32 addr, u8* dat, u32 len);

/*! \brief
*       ��оƬ�ڲ�FLASH
* \param addr[IN]			- ���׵�ַ
* \param dat[IN]			- ����
* \param len[IN]			- ���ݳ���
* \return
*		E_FLASH_OK						- �ɹ�
*/
u8  BSP_FLASH_Read(u32 addr, u8* dat, u32 len);

/*! \brief
*       ����FLASH
* \param addr[IN]			- д�׵�ַ
* \param count[IN]			- ����
* \return
*		E_FLASH_OK					- �ɹ�
*		E_FLASH_ERR_ERASE_FAILED	- ����ʧ��
*/
u8 BSP_FLASH_Erase_sector(u32 addr, u32 count);

#endif

#endif













































