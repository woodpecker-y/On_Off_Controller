#include "bsp.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"
#include "valve_deal.h"

ValveHandler s_valve_handler;

u8 valve_action(void);
void RecordValveTime(void);

//���Ŵ������
void valve_deal(void)
{
    switch(s_valve_handler.st)
    {
        case E_IDLE:
            
            break;
        case E_REDAY:
            //�㲥��ʱ���������䶯����ʱ����λ����  ���� DESN���ж�˳����ʱ�ġ�
            if(s_valve_handler.all_open_timer != 0 && s_valve_handler.all_close_timer != 0)
            {
                s_valve_handler.all_open_timer--;
                s_valve_handler.all_close_timer--;
            }            
            if(s_valve_handler.all_open_timer == 0 && s_valve_handler.all_close_timer == 0)
            {
                s_valve_handler.st = E_ACTIVE;
            }

            break;
        case E_ACTIVE:
            if(valve_action() == 0)
                s_valve_handler.st = E_FINISH;
            break;
        case E_FINISH:
            
            s_valve_handler.st = E_IDLE;
            break;
        default:
            break;
    }
    
#if OUTAGE_POWER_SWITCH
    //����ϵ�����������ʾLED��
    if(g_run_params.PowerDownFlag == 0)
    {
        if(g_run_params.ValveState==VALVE_OPEN)
        {
            g_run_params.valve_led_action = VALVE_OPEN_CONST;
        }
        else if(g_run_params.ValveState==VALVE_CLOSE)
        {
            g_run_params.valve_led_action = VALVE_CLOSE_CONST;
        }
    }
    
#endif
}

//�������Ŷ���
void valve_triggered(void)
{
        s_valve_handler.st = E_REDAY; 
}

//���Ž��̴�����
u8 valve_action(void)
{
    static u16 plug_valve_timer = 0;//�����ط�ʱ���ʱ���ʱ��Ϊ20��
    u8 rturn_st = 1;
    static u16 open_delay_timer = 0, close_delay_timer = 0;
    
    if(g_run_params.Plug_valve_state != 0x00)
    {
        g_run_params.ValvEerror = 0xFF;//���Ź���״̬  0xFF(��ת������΢������ͬʱ�պϡ�����оƬ��)     0x00����
    }
    
    //���Ŷ���
    if(g_run_params.PowerDownFlag == 1)//�ϵ�������ִ��
    {
        if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)//�û����ƿ���
        {
            //�·���ʱ
            plug_valve_timer++;
            if(plug_valve_timer >= 400) //400ms==20s
            {
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x05;//�ط���ת
                
                //���Ź��ϱ�־
                error_set(SYS_ERROR_VALVE);
                disp_valve_state(E_DISPLAY_VALVE_PLUG);//��ʾ��ת��־
                //rturn_st = 0;
            }
            
            motor_run(E_MOTOR_SWITCH_OPENED);
            if(motor_get_state() == MOTOR_STATE_OPENED)
            {
                g_run_params.ValveState = VALVE_OPEN;
                disp_valve_state(E_DISPLAY_VALVE_OPEN);
                
                //�·�
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x00;

                RecordValveTime();//��¼���������ʱ��

                //������Ź��ϱ�־
                error_clr(SYS_ERROR_VALVE);
                open_delay_timer++;
                if(open_delay_timer>=20)//�����ȴ�ʱ��
                {
                    open_delay_timer = 0;
                    motor_stop();
                    rturn_st = 0;
                }
            }
            
        }
        else if(g_sys_params.ValveCtrlDemandFlg == VALVE_CLOSE)//�û����ƹط�
        {
            //�·���ʱ
            plug_valve_timer++;
            if(plug_valve_timer >= 400) //400ms==20s
            {
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x50;//�ط���ת

                //���Ź��ϱ�־
                error_set(SYS_ERROR_VALVE);
                disp_valve_state(E_DISPLAY_VALVE_PLUG);//��ʾ��ת��־
                //rturn_st = 0;
            }
            
            motor_run(E_MOTOR_SWITCH_CLOSED);
            if(motor_get_state() == MOTOR_STATE_CLOSED)
            {
                g_run_params.ValveState = VALVE_CLOSE;
                disp_valve_state(E_DISPLAY_VALVE_CLOSE);
                
                //�·�
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x00;

                RecordValveTime();//��¼���������ʱ��

                //������Ź��ϱ�־
                error_clr(SYS_ERROR_VALVE);
                close_delay_timer++;
                if(close_delay_timer>=20)//�ط��ȴ�ʱ��
                {
                    close_delay_timer = 0;
                    motor_stop();
                    rturn_st = 0;
                }
            }
            
        }
    }
#if OUTAGE_POWER_SWITCH
    else if(g_run_params.PowerDownFlag == 0)
    {
        motor_run(E_MOTOR_SWITCH_OPENED);
        if(motor_get_state() == MOTOR_STATE_OPENED)
        {
            g_run_params.ValveState = VALVE_OPEN;
            disp_valve_state(E_DISPLAY_VALVE_OPEN);
            motor_stop();
            rturn_st = 0;
        }
    }
#endif
    
    //�жϷ���LED����˸��ʽ
    if((g_run_params.ValveState==VALVE_OPEN) && (g_sys_params.ValveCtrlDemandFlg==VALVE_CLOSE))
        g_run_params.valve_led_action = VALVE_CLOSE_TWINKLE;    //�ط�����˸
    
    else if((g_run_params.ValveState==VALVE_CLOSE) && (g_sys_params.ValveCtrlDemandFlg==VALVE_OPEN))
        g_run_params.valve_led_action = VALVE_OPEN_TWINKLE;     //��������˸
    
    else if((g_run_params.ValveState==VALVE_OPEN) && (g_sys_params.ValveCtrlDemandFlg==VALVE_OPEN))
        g_run_params.valve_led_action = VALVE_OPEN_CONST;       //�����ƴ�
    
    else if((g_run_params.ValveState==VALVE_CLOSE) && (g_sys_params.ValveCtrlDemandFlg==VALVE_CLOSE))
        g_run_params.valve_led_action = VALVE_CLOSE_CONST;      //�ط��ƴ�
    
    //����LED��״̬
    if(g_run_params.auto_test_led_flag != 1)//�������Զ�����״̬�ſ���ִ�з��ŵƶ���״̬
        valve_action_LED_twinkle(g_run_params.valve_led_action);//���ŵ�����״̬
    
    return rturn_st;
    
}


//LED�Ʒ��Ŷ�����˸������״̬�ƣ�
void valve_action_LED_twinkle(Valve_led_twinkle valve_action_st)
{
    static u8 valve_num = 0;
    static FlagStatus led_state = RESET;
    
        // ����LED�� ��ʾ״̬
        if(valve_action_st <= 1)   //��˸
        {
            valve_num++;
            if(valve_action_st == VALVE_OPEN_TWINKLE)            //��������˸
            {
                led_off(CLOSE_VALVE_LED);//�رչط�LED
                if(valve_num == 1)
                {
                    if(led_state == RESET)
                    {
                        led_off(OPEN_VALVE_LED);
                        led_state = SET;
                    }
                    else
                    {
                        led_on(OPEN_VALVE_LED);
                        led_state = RESET;
                    }
                }
                else if(valve_num >= 4)     //��˸��Ƶ��  4��Ϊ4*50ms = 200ms
                {
                    valve_num = 0;
                }
            }
            else if(valve_action_st == VALVE_CLOSE_TWINKLE)       //�ط�����˸
            {
                led_off(OPEN_VALVE_LED);//�رտ���LED
                if(valve_num == 1)
                {
                    if(led_state == RESET)
                    {
                        led_off(CLOSE_VALVE_LED);
                        led_state = SET;
                    }
                    else
                    {
                        led_on(CLOSE_VALVE_LED);
                        led_state = RESET;
                    }
                }
                else if(valve_num >= 4)
                {
                    valve_num = 0;
                }
            }
        }
        else if(valve_action_st>1 && valve_action_st<=3)    //����
        {
            if(valve_action_st == VALVE_OPEN_CONST){      //�����Ƴ���
                led_on(OPEN_VALVE_LED);
                led_off(CLOSE_VALVE_LED);
            }
            else if(valve_action_st == VALVE_CLOSE_CONST)
            {       //�ط��Ƴ���
                led_on(CLOSE_VALVE_LED);
                led_off(OPEN_VALVE_LED);
            }
        }
}

void RecordValveTime(void)
{
    struct tm t;
    
    rtc_read(&t);
    
    g_sys_params.RecordTime.Year      = t.tm_year + 1900;
    g_sys_params.RecordTime.Month     = t.tm_mon+1;
    g_sys_params.RecordTime.Day       = t.tm_mday;
    g_sys_params.RecordTime.Hour      = t.tm_hour;
    g_sys_params.RecordTime.Minute    = t.tm_min;
    g_sys_params.RecordTime.Second    = t.tm_sec;
    g_sys_params.RecordTime.Week      = t.tm_wday+1;
    
    
    save_params(SYS_SAVE_SHARE_PARAMS);
}

