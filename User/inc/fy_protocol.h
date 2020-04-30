#ifndef _FY_PROTOCOL_H
#define _FY_PROTOCOL_H

#include "bsp.h"
#include "sysparams.h"

// ������ݸ��س���
#define MSG_DATA_MAX            80

// ��ʼ֡
#define FY_PROTOCOL_STX         0x3A

// ����֡
#define FY_PROTOCOL_ETX         0xAA

// �Ǳ��ų���
#define LENGTH_ADDR		        4//4���ֽ� (8λ)

//�㲥��ַ
#define ForyonBroadcastAddr	    0XAAAAAAAA

typedef enum _FY_PROTOCOLRet{
    FY_PROTOCOL_OK = 0,
    FY_PROTOCOL_ERR_CHECKSUM = 0x40,            // У��ʹ���
    FY_PROTOCOL_ERR_LESS_MEMORY = 0x41,			// �ڴ治��
    FY_PROTOCOL_ERR_LENGTH = 0x42,				// ���ȴ���
    FY_PROTOCOL_ERR_ADDR = 0x43,				// ��ַ��ƥ��
    FY_PROTOCOL_ERR_INVALID_PKG = 0x44,			// ���ݰ����� ��ͷ��β����
    FY_PROTOCOL_ERR_CTRL_CODE = 0x45,           // ���������
    FY_PROTOCOL_ERR_DATA_FLAG = 0x46,           // ���ݱ�ʶ����
    FY_PROTOCOL_ERR_ADDR_EXSITED = 0x47,        // �豸�����д��
    FY_PROTOCOL_ERR_UNKNOWN_DATA_FLAG = 0x48,   // δ֪�����ݱ�ʶ
    FY_PROTOCOL_ERR_INVALID_DATETIME = 0x49,    // �Ƿ���ʱ��
    FY_PROTOCOL_ERR_VALUE_MODE = 0x50,          // ����ķ���ʱ��
    FY_PROTOCOL_ERR_BROADCAST_ADDR = 0x51,      // �㲥��ַ
    FY_PROTOCOL_ERR_HAVE_DEAL_NO_RESP = 0x52,   // ���������Ƿ��Ų�����
}FY_PROTOCOLRet;


// FY_PROTOCOLЭ��֡����
typedef struct _FY_PROTOCOLPkg{
    unsigned char       ver;                    // VER      �Ǳ�����
    unsigned int        sn;                     // SN       �Ǳ��ַ
    unsigned short int  desn;                   // DESN     �����Ա���
    unsigned short int  gn;                     // GN       �������
    unsigned char       type;                   // TYPE     Э�����
    unsigned char       lenth;			        // LENTH    ���ݳ���
    unsigned char       data[MSG_DATA_MAX];     // DATA     ����
}FY_PROTOCOLPkg;



unsigned char fy_protocol_pack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, u8 *packaged_data_size);
unsigned char fy_protocol_unpack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, int packaged_size);


// ����Э���� TYPE �жϹ�������
u8 fy_protocol_func_request(FY_PROTOCOLPkg *pkg, RunParams *run_params, SysParams *sys_params);
u8 fy_protocol_func_response(FY_PROTOCOLPkg *pkg,  unsigned char *packaged_data, u8 *packaged_data_size);

#endif
