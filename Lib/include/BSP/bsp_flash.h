#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

#ifdef HAVE_FLASH

#include "bsp.h"

/*! \brief 闪存起始地址 */
#define FLASH_ADDR_START	0x8000000

/*! \brief 闪存存储容量大小定义 */
/*! \brief 小容量类型及小容量超值类型 */
#if	  defined (STM32F10X_LD) || defined (STM32F10X_LD_VL)
#define FLASH_SIZE			0x8000
/*! \brief 中容量类型及中容量超值类型 */
#elif defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
#define FLASH_SIZE			0x10000
/*! \brief 大容量类型及大容量超值类型 */
#elif defined (STM32F10X_HD) || defined (STM32F10X_HD_VL)
#define FLASH_SIZE			0x80000
/*! \brief 互联类型 */
#elif defined (STM32F10X_CL)
#define FLASH_SIZE			0x40000
#else
	#error "Please configure MCU Type. (in Keil uVision4 Options->C/C++->Define)"
#endif

/*! \brief 闪存页容量大小定义 */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
	#define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
	#define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

/*! \brief Flash反馈码 */
typedef enum _eFlashRet{
	E_FLASH_OK = 0,
	E_FLASH_ERR_INCORRECT_LENGTH,
	E_FLASH_ERR_WRITE_FAILED,
	E_FLASH_ERR_ERASE_FAILED,
}eFlashRet;

/*! \brief
*       写芯片内部FLASH
* \param addr[IN]			- 写首地址
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		E_FLASH_OK						- 成功
*		E_FLASH_ERR_INCORRECT_LENGTH	- 长度不正确
*		E_FLASH_ERR_WRITE_UNCOMPLETE	- 写入失败
* \notes
*		因flash写入的最小长度为2Bytes，因此长度必须是2的整数倍；
*/
u8  BSP_FLASH_Write(u32 addr, u8* dat, u32 len);

/*! \brief
*       读芯片内部FLASH
* \param addr[IN]			- 读首地址
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		E_FLASH_OK						- 成功
*/
u8  BSP_FLASH_Read(u32 addr, u8* dat, u32 len);

/*! \brief
*       擦除FLASH
* \param addr[IN]			- 写首地址
* \param count[IN]			- 数据
* \return
*		E_FLASH_OK					- 成功
*		E_FLASH_ERR_ERASE_FAILED	- 擦除失败
*/
u8 BSP_FLASH_Erase_sector(u32 addr, u32 count);

#endif

#endif













































