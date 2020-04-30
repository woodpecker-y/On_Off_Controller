#ifndef _FY_PROTOCOL_H
#define _FY_PROTOCOL_H

#include "bsp.h"
#include "sysparams.h"

// 最大数据负载长度
#define MSG_DATA_MAX            80

// 起始帧
#define FY_PROTOCOL_STX         0x3A

// 结束帧
#define FY_PROTOCOL_ETX         0xAA

// 仪表编号长度
#define LENGTH_ADDR		        4//4个字节 (8位)

//广播地址
#define ForyonBroadcastAddr	    0XAAAAAAAA

typedef enum _FY_PROTOCOLRet{
    FY_PROTOCOL_OK = 0,
    FY_PROTOCOL_ERR_CHECKSUM = 0x40,            // 校验和错误
    FY_PROTOCOL_ERR_LESS_MEMORY = 0x41,			// 内存不足
    FY_PROTOCOL_ERR_LENGTH = 0x42,				// 长度错误
    FY_PROTOCOL_ERR_ADDR = 0x43,				// 地址不匹配
    FY_PROTOCOL_ERR_INVALID_PKG = 0x44,			// 数据包错误 包头包尾不对
    FY_PROTOCOL_ERR_CTRL_CODE = 0x45,           // 控制码错误
    FY_PROTOCOL_ERR_DATA_FLAG = 0x46,           // 数据标识错误
    FY_PROTOCOL_ERR_ADDR_EXSITED = 0x47,        // 设备编号已写入
    FY_PROTOCOL_ERR_UNKNOWN_DATA_FLAG = 0x48,   // 未知的数据标识
    FY_PROTOCOL_ERR_INVALID_DATETIME = 0x49,    // 非法的时间
    FY_PROTOCOL_ERR_VALUE_MODE = 0x50,          // 错误的阀门时间
    FY_PROTOCOL_ERR_BROADCAST_ADDR = 0x51,      // 广播地址
    FY_PROTOCOL_ERR_HAVE_DEAL_NO_RESP = 0x52,   // 动作处理但是阀门不反馈
}FY_PROTOCOLRet;


// FY_PROTOCOL协议帧定义
typedef struct _FY_PROTOCOLPkg{
    unsigned char       ver;                    // VER      仪表类型
    unsigned int        sn;                     // SN       仪表地址
    unsigned short int  desn;                   // DESN     网络成员编号
    unsigned short int  gn;                     // GN       网络组号
    unsigned char       type;                   // TYPE     协议编码
    unsigned char       lenth;			        // LENTH    数据长度
    unsigned char       data[MSG_DATA_MAX];     // DATA     数据
}FY_PROTOCOLPkg;



unsigned char fy_protocol_pack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, u8 *packaged_data_size);
unsigned char fy_protocol_unpack(FY_PROTOCOLPkg *pkg, unsigned char *packaged_data, int packaged_size);


// 根据协议中 TYPE 判断工作内容
u8 fy_protocol_func_request(FY_PROTOCOLPkg *pkg, RunParams *run_params, SysParams *sys_params);
u8 fy_protocol_func_response(FY_PROTOCOLPkg *pkg,  unsigned char *packaged_data, u8 *packaged_data_size);

#endif
