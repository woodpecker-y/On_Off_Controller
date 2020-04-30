#ifndef _ERROR_H
#define _ERROR_H

#include "bsp.h"

typedef enum _ErrorSet{
    SYS_ERROR_RECORD_PARAMS     = 0X01,  // 记录参数错误、
    SYS_ERROR_SHARE_PARAMS      = 0X02,  // 分摊参数错误、
    SYS_ERROR_DEVICE_PARAMS     = 0X04,  // 设备参数错误、
    SYS_ERROR_FACTORY_PARAMS    = 0X08,  // 出厂参数错误、
    SYS_ERROR_VALVE             = 0X10,  // 传动机构故障、
    SYS_ERROR_OUTPUT_TEMP       = 0X20,  // 出口温度故障、
    SYS_ERROR_INPUT_TEMP        = 0X40,  // 入口传感器故障、
    SYS_ERROR_RTC               = 0X80,  // RTC故障、
}ErrorSet;

void error_set(u8 code);

void error_clr(u8 code);

#endif


