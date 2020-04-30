#ifdef HAVE_XMODEM

#ifndef _XMODEM_H
#define _XMODEM_H

#include "bsp.h"

#define XMODEM_FRAME_SIZE   128

//Xmoden¿ØÖÆ×Ö·û
#define XMODEM_NUL         0x00
#define XMODEM_SOH         0x01
#define XMODEM_STX         0x02
#define XMODEM_EOT         0x04
#define XMODEM_ACK         0x06
#define XMODEM_NAK         0x15
#define XMODEM_CAN         0x18
#define XMODEM_EOF         0x1A
#define XMODEM_RWC         'C'
#define FD_PASSWD		   'D'

typedef enum _StoreType{
	E_STORE_TYPE_FLASH_EXT,
	E_STORE_TYPE_FLASH_INT,
	E_STORE_TYPE_EEPROM,
}StoreType;

typedef enum _XModemSts{
	E_XMODEM_STS_READY,
	E_XMODEM_STS_TRANSFER,
	E_XMODEM_STS_SUCCESS,
	E_XMODEM_STS_FAILED,
}XModemSts;

typedef enum _XModemRet{
	E_XMODEM_OK,
	E_XMODEM_ERR_INVALID_LENGTH,
	E_XMODEM_ERR_INVALID_PACKAGE_NO,
	E_XMODEM_ERR_INVALID_CRC16,
}XModemRet;

u8 xmodem_download(u8 com_id, u8 type, u32 addr, u32 size);

#endif

#endif
