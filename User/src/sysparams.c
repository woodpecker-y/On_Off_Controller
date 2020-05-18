
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "sysparams.h"

const char *foryon = \
"------------------- 欢迎使用 丰源智控通断控制器 -----------------------\r\n\
\r\n\
        888888b  .d88b.  88888b. db    db  .d88b.  d8b   88 \r\n\
        88`     .8P  Y8. 88` `8D `8b  d8` .8P  Y8. 888o  88 \r\n\
        88ooo   88    88 88oobY`  `8bd8`  88    88 88V8o 88 \r\n\
        88999   88    88 88`8b      88    88    88 88 V8o88 \r\n\
        88      `8b  d8` 88 `88.    88    `8b  d8` 88  V888 \r\n\
        88       `Y88P`  88   YD    88     `Y88P`  88   V8P \r\n\
\r\n\
----------------------------------------------- www.foryon.com --------\r\n\
\r\n";

//const char *foryon_log = 
//"            .::\r\n\
//-----------.:----------------------------------------------------------\r\n\
//        .:.:..:   .::    .: .:::.::   .::   .::    .:: .::  \r\n\
//          .::   .::  .::  .::    .:: .::  .::  .::  .::  .::\r\n\
//          .::  .::    .:: .::      .:::  .::    .:: .::  .::\r\n\
//          .::   .::  .::  .::       .::   .::  .::  .::  .::\r\n\
//          .::     .::    .:::      .::      .::    .:::  .::\r\n\
//----------------------------------.::---------- www.foryon.com --------\r\n";

SysParams g_sys_params;
RunParams g_run_params;
AutoTestStruct AutoTest;

//系统复位
void soft_reset()
{
    __set_PRIMASK(1); //这里禁止全局中断(除了NMI和HardFault)。在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。
    NVIC_SystemReset();//系统复位
}

void sys_factory_reset(void)
{
    struct tm t;
    
    rtc_read(&t);
    
    g_sys_params.Device_DESN         = 0x0000;
    g_sys_params.Version_H           = HARDWARE_VERSION;
    g_sys_params.Version_S           = SOFTWARE_VERSION;
    g_sys_params.DeviceType          = 0;                   //设备类型

    //房间信息
    g_sys_params.Buld                = 1;                   //楼栋号
    g_sys_params.Unit                = 1;                   //单元号
    g_sys_params.Floor               = 1;                   //楼层号
    g_sys_params.Cell                = 1;                   //房间号
    g_sys_params.CellArea            = 10000;               //100.00 房间面积,2位小数点

    //温度信息
    g_sys_params.TempCtrlType        = 0x00;                //温度设置类型 0X00 用户设置  0XAA 系统设置
    g_sys_params.TempSetValue        = 2550;                //25.50℃ 温度设置值

    g_sys_params.LimtTE_Flag         = 0xFF;                //极限温度设置标志 0xAA 系统强制设定温度， 0xFF 系统取消强制设定温度
    g_sys_params.LimtTE_Value        = 3000;                //30.00℃ 极限温度设置值

    g_sys_params.Offect_Temp         = 0;                   //校准温度点
    g_sys_params.RakeRatioOffect_L   = 0;                   //低区斜率偏差温度折算值每0.001度
    g_sys_params.RakeRatioOffect_H   = 0;                   //高区斜率偏差温度折算值0.001度
    g_sys_params.Offect_Input        = 0;                   //入口温度偏差
    g_sys_params.Offect_Output       = 0;                   //出口温度偏差

    //计量信息
    g_sys_params.ControlType         = 0x00;                //用户费控类型 高4位为费控方式  低4位为费控状态  0X00:未启用预付费,未欠费  0XA0:已启用预付费，没有欠费  0XAA:已启用预付费，用户已经欠费
    g_sys_params.Cycle_OpTim         = 0;                   //周期开阀时间 单位（秒）
    g_sys_params.loop_time_59        = 0;                   //周期供暖时间 单位（秒）
    g_sys_params.Apportion_Energy    = 0;                   //周期分摊热量 使用热量 单位（kwh）
    g_sys_params.Remain_Heat         = 0;                   //剩余热量 单位（kwh）
    g_sys_params.Buy_Total_Heat      = 0;                   //用户购入热量 单位（kwh）

    //阀门开关动作相关
    g_sys_params.ValveCtrlDemandFlg  = VALVE_OPEN;          //阀门控制命令标志 0x0A开阀 0xA0关阀
    g_run_params.ValveState          = VALVE_CLOSE;

    g_sys_params.ForceCtrl           = 0xFF;                //阀门强制控制标志  0x55 强制开阀  0xAA强制关阀   0xFF强制取消

    //供暖期开始时间
    g_sys_params.Start_Time.Year     = 2019;
    g_sys_params.Start_Time.Month    = 11;
    g_sys_params.Start_Time.Day      = 15;
    g_sys_params.Start_Time.Hour     = 8;
    g_sys_params.Start_Time.Minute   = 0;
    g_sys_params.Start_Time.Second   = 0;
    g_sys_params.Start_Time.Week     = 5;
    //供暖期结束时间
    g_sys_params.Over_Time.Year      = 2020;
    g_sys_params.Over_Time.Month     = 3;
    g_sys_params.Over_Time.Day       = 15;
    g_sys_params.Over_Time.Hour      = 8;
    g_sys_params.Over_Time.Minute    = 0;
    g_sys_params.Over_Time.Second    = 0;
    g_sys_params.Over_Time.Week      = 7;
    //阀门最后动作时间
    g_sys_params.RecordTime.Year      = t.tm_year + 1900;
    g_sys_params.RecordTime.Month     = t.tm_mon+1;
    g_sys_params.RecordTime.Day       = t.tm_mday;
    g_sys_params.RecordTime.Hour      = t.tm_hour;
    g_sys_params.RecordTime.Minute    = t.tm_min;
    g_sys_params.RecordTime.Second    = t.tm_sec;
    g_sys_params.RecordTime.Week      = t.tm_wday+1;

    g_sys_params.DebugFlag = 0;     //关闭日志模式
    
    if(g_sys_params.factory_flag != 1)
    {
        g_sys_params.sys_t.tm_year    = 2019 - 1900;
        g_sys_params.sys_t.tm_mon     = 10;
        g_sys_params.sys_t.tm_mday    = 15;
        g_sys_params.sys_t.tm_hour    = 8;
        g_sys_params.sys_t.tm_min     = 0;
        g_sys_params.sys_t.tm_sec     = 0;
        g_sys_params.sys_t.tm_wday    = 5;
    }
    
    //出厂标志
    g_sys_params.factory_flag = 1;
    
    //保存数据
    save_params(SYS_SAVE_FACTORY_PARAMS);
}



void save_params(save_enum params_flag)
{
    u8 rv = 0;
    //u8* save_ptr;
    //u8 sizeof_params = 0;
    ErrorSet error_enum;
    
    //如果在测试进程中不进行存储数据
    if(g_run_params.auto_test_flag != 0)
        return;
    
    switch(params_flag)
    {
        case SYS_SAVE_FACTORY_PARAMS:
            error_enum = SYS_ERROR_FACTORY_PARAMS;
//            save_ptr = (u8*)&g_sys_params;
//            sizeof_params = sizeof(SysParams);
            break;
        case SYS_SAVE_DEVICE_PARAMS:
            error_enum = SYS_ERROR_DEVICE_PARAMS;
//            save_ptr = (u8*)&g_sys_params.Device_SN;
//            sizeof_params = 11;
            break;
        case SYS_SAVE_SHARE_PARAMS:
            error_enum = SYS_ERROR_SHARE_PARAMS;
//            save_ptr = (u8*)&g_sys_params.Buld;
//            sizeof_params = 36;
            break;
        case SYS_SAVE_RECORD_PARAMS:
            error_enum = SYS_ERROR_RECORD_PARAMS;
//            save_ptr = (u8*)&g_sys_params.ControlType;
//            sizeof_params = 40;
            break;
        default:
           return;
           // break;
    }
    // 文件系统反馈码
    //typedef enum _eFSRet{
    //	E_FS_OK = 0,							  		// 成功
    //	E_FS_ERR_FILE_NUM_EXCEED_MAX,			   		// 文件数超过最大
    //	E_FS_ERR_HARDWARE,						   		// 底层硬件接口
    //	E_FS_ERR_NOT_EXSIT,		  						// 文件不存在
    //	E_FS_ERR_READ_FAIL,					   			// 读失败
    //	E_FS_ERR_WRITE_FAIL,							// 写失败
    //	E_FS_ERR_NO_RECORD,				   				// 无记录
    //	E_FS_ERR_EXCEED_FILE_SIZE,						// 超出文件大小
    //	E_FS_ERR_INVALID_PARAMS,						// 非法的参数
    //	E_FS_ERR_ALREADY_WRITED,						// 文件已写入
    //	E_FS_ERR_NOT_MATCH_READ_AFTER_WRITE,			// 读取已写入的数据后不匹配
    //}eFSRet;
    //保存数据
    rv = fs_write(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params, sizeof(SysParams));
    if (rv != E_FS_OK){
        //出厂参数错误
        error_set(error_enum);
        MYLOG_DEBUG("fs_write[E_FILE_TYPE_SYSINF] failed!Return Code[%d]\r\n", rv);
    }
    else{
        error_clr(error_enum);
    }
    
    return;
}

