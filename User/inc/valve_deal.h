#ifndef _VALVE_DEAL_H
#define _VALVE_DEAL_H

#include "bsp.h"

typedef enum _STATE{
    E_IDLE = 0,             //空闲状态
    E_REDAY,                //准备状态
    E_ACTIVE,               //动作
    E_FINISH                //完成状态
}_STATE;


typedef struct _ValveHandler{
    u8      st;
    u16     all_open_timer;
    u16     all_close_timer;
    
    

}ValveHandler;

typedef enum _VALVE_LED_TWINKLE
{
    VALVE_OPEN_TWINKLE,
    VALVE_CLOSE_TWINKLE,
    VALVE_OPEN_CONST,
    VALVE_CLOSE_CONST
}Valve_led_twinkle;

extern ValveHandler s_valve_handler;

//阀门LED处理
void valve_action_LED_twinkle(Valve_led_twinkle valve_action_st);

//阀门处理进程
void valve_deal(void);

//触发阀门动作
void valve_triggered(void);

#endif


