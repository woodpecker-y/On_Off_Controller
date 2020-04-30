#ifndef _RECV_DATA_DEAL_H
#define _RECV_DATA_DEAL_H

#include "bsp.h"
#include "fy_protocol.h"

typedef struct _Com1Handler{
    u8                  com1_sts;
    u8*                 com1_rcv_ptr;
    u16                 com1_rcv_len;
}Com1Handler;

typedef struct _Com3Handler{
    u8                  com3_sts;
    u8*                 com3_rcv_ptr;
    u16                 com3_rcv_len;
    FY_PROTOCOLPkg      pkg;
    u8                  pkg_len;
}Com3Handler;

typedef struct _RfHandler{
    u8                  rf_sts;
    u8*                 rf_rcv_ptr;
    u8                  rf_rcv_len;
    FY_PROTOCOLPkg      pkg;
    u8                  pkg_len;
}RfHandler;

extern Com1Handler s_com1_handler;
extern Com3Handler s_com3_handler;
extern RfHandler   s_rf_handler;

// ���ݽ��մ��� MBUS���� COM1���ݵ�����־����  RF���ݽ���
void com_recv_data_deal(void);


#endif

