#ifndef _VALVE_DEAL_H
#define _VALVE_DEAL_H

#include "bsp.h"

typedef enum _STATE{
    E_IDLE = 0,             //����״̬
    E_REDAY,                //׼��״̬
    E_ACTIVE,               //����
    E_FINISH                //���״̬
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

//����LED����
void valve_action_LED_twinkle(Valve_led_twinkle valve_action_st);

//���Ŵ������
void valve_deal(void);

//�������Ŷ���
void valve_triggered(void);

#endif


