#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "bsp.h"
//////////////////////////显示//////////////////////////
typedef enum _E_Display_Idx{
    
    SEG_VALVE_OPEN     = 0,         // "开" 开阀符号
    SEG_VALVE_CLOSE    = 1,         // “关” 关阀符号
    SEG_SYS_FAULT      = 2,         // ○“X” 系统故障标识
    SEG_VALVE_STOPE    = 3,         // 堵转符号

    //---- 数码管1字段 ----
    SEG_1A             = 4,
    SEG_1F             = 5,
    SEG_1E             = 6,
    SEG_TIME           = 7,         // "时间"标识
    
    SEG_1B             = 8,
    SEG_1G             = 9,
    SEG_1C             = 10,
    SEG_1D             = 11,

    //---- 数码管2字段 ----
    SEG_2A             = 12,
    SEG_2F             = 13,
    SEG_2E             = 14,
    SEG_TEMP_INLET     = 15,         // 入口符号
    
    SEG_2B             = 16,
    SEG_2G             = 17,
    SEG_2C             = 18,
    SEG_2D             = 19,

    //---- 数码管3字段 ----
    SEG_3A             = 20,
    SEG_3F             = 21,
    SEG_3E             = 22,
    SEG_TEMP_RETURN    = 23,         // 出口符号
    
    SEG_3B             = 24,
    SEG_3G             = 25,
    SEG_3C             = 26,
    SEG_3D             = 27,

    //---- 数码管4字段 ----
    SEG_4A             = 28,
    SEG_4F             = 29,
    SEG_4E             = 30,
    SEG_4DP_POINT      = 31,         // 左数第一个小数点
    
    SEG_4B             = 32,
    SEG_4G             = 33,
    SEG_4C             = 34,
    SEG_4D             = 35,

    //---- 数码管5字段 ----
    SEG_5A             = 36,
    SEG_5F             = 37,
    SEG_5E             = 38,
    NULL_              = 39,         // 空
    
    SEG_5B             = 40,
    SEG_5G             = 41,
    SEG_5C             = 42,
    SEG_5D             = 43,

    //---- 数码管6字段 ----
    SEG_6A             = 44,
    SEG_6F             = 45,
    SEG_6E             = 46,
    SEG_6DP_POINT      = 47,         // 左数第二个小数点
    
    
    SEG_6B             = 48,
    SEG_6G             = 49,
    SEG_6C             = 50,
    SEG_6D             = 51,

    //---- 数码管7字段 ----
    SEG_7A             = 52,
    SEG_7F             = 53,
    SEG_7E             = 54,
    SEG_7DP_POINT      = 55,         // 左数第三个小数点
    
    SEG_7B             = 56,
    SEG_7G             = 57,
    SEG_7C             = 58,
    SEG_7D             = 59,

    //---- 数码管8字段 ----
    SEG_8A             = 60,
    SEG_8F             = 61,
    SEG_8E             = 62,
    SEG_TEMP_DIFFERENCE= 63,         // 温差符号
    
    SEG_8B             = 64,
    SEG_8G             = 65,
    SEG_8C             = 66,
    SEG_8D             = 67,
    
    //---- 系统标识 ----
    SEG_CONNECT        = 68,        // 接通符号
    SEG_DISCONNECT     = 69,        // 断开符号
    SEG_HEAT_QUANTITY  = 70,        // 热量符号
    SEG_COMM_ID        = 71,        // "通讯"（显示ID）符号

    SEG_DEGREES_CELSIUS= 72,        // 摄氏度℃符号
    SEG_KWH            = 73,        // KW.h 千瓦时符号
    SEG_DAY            = 74,        // d（时间单位）符号
    SEG_HOUR           = 75,        // h（时间单位）符号

    SEG_SIGNAL         = 76,        // 信号
    SEG_ANT            = 77,        // 天线符号
    SEG_ANT_FULAT      = 78,        // 通信故障 “X”
    SEG_BATTERY        = 79         // 电池符号
    
}E_Display_Idx;

/*字段的显示开关*/
typedef enum _DispMode{
    E_DISPLAY_OFF = 0,    
    E_DISPLAY_ON
}E_DispMode;

/*显示阀门开关的状态*/
typedef enum _DispValveMode{
    E_DISPLAY_VALVE_OPEN = 0,    
    E_DISPLAY_VALVE_CLOSE,
    E_DISPLAY_VALVE_PLUG
}E_DispValveMode;

/*显示菜单轮询枚举*/
typedef enum _MenuIndex{
    E_MENU_TIME = 0,        // 显示 系统时间
    E_MENU_TEMP_INLET,      // 显示 入口温度
    E_MENU_TEMP_RETURN,     // 显示 出口温度
    E_MENU_TEMP_DIFF,       // 显示 温差
    E_MENU_CONNECT_TIME,    // 显示 接通累计时间
    E_MENU_FAULT,           // 显示 错误状态表示符
    E_MENU_ID_LEFT,         // 显示 ID左边
    E_MENU_ID_RIGHT,        // 显示 ID右边
    E_MENU_HEAT_QUANTITY    // 显示 总累计热量
}E_MenuIndex;

/*! \brief   刷新显示*/
void disp_update(void);

/*! \brief   清除所有显示*/
void disp_clear(void);

/*! \brief   全显示*/
void disp_full(void);

/*! \brief   轮询显示*/
void disp_poll(void);

/*! \brief   显示数字*/
void disp_digit(u8 idx, u8 val, u8 disp_sts);

/*! \brief   显示数值*/
void disp_number(u8 start, u8 end, u32 val);

/*! \brief   显示字符*/
void disp_char(u8 start, u8 ch);

/*! \brief   显示字符串*/
void disp_str(u8 start, u8 end, const char *str);

/*! \brief 显示系统时间 年月日*/
void disp_systime_year_mon_day(void);

/*! \brief  显示系统时间 时分秒  */
void disp_systime_hour_min_sec(void);

/*! \brief   显示进水温度*/
void disp_temp_inlet(long temp_inlet);

/*! \brief  显示出水温度*/
void disp_temp_return(long temp_return);

/*! \brief  显示温差*/
void disp_temp_difference(long temp_dif, u8 flag);

/*! \brief  显示开阀累计时间*/
void disp_connect_sum_time(u32 connect_time);

/*! \brief  显示系统错误状态标志 */
void disp_sys_fault_flag(u8 code);

/*! \brief  显示系统错误状态*/
void disp_sys_fault(u8 code);

/*! \brief  显示设备编码（ID）*/
void disp_id(const unsigned char *id, u8 len);

/*! \brief  显示通讯编码（GN）*/
void disp_gn_id(const unsigned char *id, u8 len);

/*! \brief  显示总热量*/
void disp_heat_quantity(u32 heat_quantity);
    
/*! \brief   显示阀门状态*/
void disp_valve_state(E_DispValveMode sts);

/*! \brief   显示信号*/
void disp_signal_state(E_DispMode sts);

/*! \brief   显示电池*/
void disp_battery_state(E_DispMode sts);
    
void disp_sys_version(u8 version_h, u8 version_s);
#endif



//    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_OFF);       //时间
//    lcd_write_seg(disp_buffer, SEG_TEMP_INLET, E_DISPLAY_OFF);       //入口
//    lcd_write_seg(disp_buffer, SEG_TEMP_RETURN, E_DISPLAY_OFF);       //出口
//    lcd_write_seg(disp_buffer, SEG_CONNECT, E_DISPLAY_OFF);       //接通
//    lcd_write_seg(disp_buffer, SEG_DISCONNECT, E_DISPLAY_OFF);       //断开
//    lcd_write_seg(disp_buffer, SEG_HEAT_QUANTITY, E_DISPLAY_OFF);       //热量
//    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_OFF);       //通讯
//    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_OFF);       //摄氏度℃符号

//    lcd_write_seg(disp_buffer, SEG_KWH, E_DISPLAY_OFF);       //KW.h 千瓦时符号
//    lcd_write_seg(disp_buffer, SEG_DAY, E_DISPLAY_OFF);       //热量
//    lcd_write_seg(disp_buffer, SEG_HOUR, E_DISPLAY_OFF);       //通讯
//    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_OFF);       //摄氏度℃符号
