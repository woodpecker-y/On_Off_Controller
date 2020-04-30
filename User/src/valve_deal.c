#include "bsp.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"
#include "valve_deal.h"

ValveHandler s_valve_handler;

u8 valve_action(void);
void RecordValveTime(void);

//阀门处理进程
void valve_deal(void)
{
    switch(s_valve_handler.st)
    {
        case E_IDLE:
            
            break;
        case E_REDAY:
            //广播延时开阀（阀间动作延时）单位毫秒  根据 DESN来判断顺序延时的。
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
    //如果断电则常亮阀门提示LED灯
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

//触发阀门动作
void valve_triggered(void)
{
        s_valve_handler.st = E_REDAY; 
}

//阀门进程处理函数
u8 valve_action(void)
{
    static u16 plug_valve_timer = 0;//开阀关阀时间计时，最长时间为20秒
    u8 rturn_st = 1;
    static u16 open_delay_timer = 0, close_delay_timer = 0;
    
    if(g_run_params.Plug_valve_state != 0x00)
    {
        g_run_params.ValvEerror = 0xFF;//阀门故障状态  0xFF(堵转、两个微动开关同时闭合、驱动芯片坏)     0x00正常
    }
    
    //阀门动作
    if(g_run_params.PowerDownFlag == 1)//上电的情况下执行
    {
        if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)//用户控制开阀
        {
            //堵阀计时
            plug_valve_timer++;
            if(plug_valve_timer >= 400) //400ms==20s
            {
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x05;//关阀堵转
                
                //阀门故障标志
                error_set(SYS_ERROR_VALVE);
                disp_valve_state(E_DISPLAY_VALVE_PLUG);//显示堵转标志
                //rturn_st = 0;
            }
            
            motor_run(E_MOTOR_SWITCH_OPENED);
            if(motor_get_state() == MOTOR_STATE_OPENED)
            {
                g_run_params.ValveState = VALVE_OPEN;
                disp_valve_state(E_DISPLAY_VALVE_OPEN);
                
                //堵阀
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x00;

                RecordValveTime();//记录阀门最后动作时间

                //清除阀门故障标志
                error_clr(SYS_ERROR_VALVE);
                open_delay_timer++;
                if(open_delay_timer>=20)//开阀等待时间
                {
                    open_delay_timer = 0;
                    motor_stop();
                    rturn_st = 0;
                }
            }
            
        }
        else if(g_sys_params.ValveCtrlDemandFlg == VALVE_CLOSE)//用户控制关阀
        {
            //堵阀计时
            plug_valve_timer++;
            if(plug_valve_timer >= 400) //400ms==20s
            {
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x50;//关阀堵转

                //阀门故障标志
                error_set(SYS_ERROR_VALVE);
                disp_valve_state(E_DISPLAY_VALVE_PLUG);//显示堵转标志
                //rturn_st = 0;
            }
            
            motor_run(E_MOTOR_SWITCH_CLOSED);
            if(motor_get_state() == MOTOR_STATE_CLOSED)
            {
                g_run_params.ValveState = VALVE_CLOSE;
                disp_valve_state(E_DISPLAY_VALVE_CLOSE);
                
                //堵阀
                plug_valve_timer = 0;
                g_run_params.Plug_valve_state = 0x00;

                RecordValveTime();//记录阀门最后动作时间

                //清除阀门故障标志
                error_clr(SYS_ERROR_VALVE);
                close_delay_timer++;
                if(close_delay_timer>=20)//关阀等待时间
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
    
    //判断阀门LED灯闪烁方式
    if((g_run_params.ValveState==VALVE_OPEN) && (g_sys_params.ValveCtrlDemandFlg==VALVE_CLOSE))
        g_run_params.valve_led_action = VALVE_CLOSE_TWINKLE;    //关阀灯闪烁
    
    else if((g_run_params.ValveState==VALVE_CLOSE) && (g_sys_params.ValveCtrlDemandFlg==VALVE_OPEN))
        g_run_params.valve_led_action = VALVE_OPEN_TWINKLE;     //开阀灯闪烁
    
    else if((g_run_params.ValveState==VALVE_OPEN) && (g_sys_params.ValveCtrlDemandFlg==VALVE_OPEN))
        g_run_params.valve_led_action = VALVE_OPEN_CONST;       //开阀灯打开
    
    else if((g_run_params.ValveState==VALVE_CLOSE) && (g_sys_params.ValveCtrlDemandFlg==VALVE_CLOSE))
        g_run_params.valve_led_action = VALVE_CLOSE_CONST;      //关阀灯打开
    
    //阀门LED灯状态
    if(g_run_params.auto_test_led_flag != 1)//当不在自动测试状态才可以执行阀门灯动作状态
        valve_action_LED_twinkle(g_run_params.valve_led_action);//阀门灯亮的状态
    
    return rturn_st;
    
}


//LED灯阀门动作闪烁（阀门状态灯）
void valve_action_LED_twinkle(Valve_led_twinkle valve_action_st)
{
    static u8 valve_num = 0;
    static FlagStatus led_state = RESET;
    
        // 阀门LED灯 显示状态
        if(valve_action_st <= 1)   //闪烁
        {
            valve_num++;
            if(valve_action_st == VALVE_OPEN_TWINKLE)            //开阀灯闪烁
            {
                led_off(CLOSE_VALVE_LED);//关闭关阀LED
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
                else if(valve_num >= 4)     //闪烁的频率  4则为4*50ms = 200ms
                {
                    valve_num = 0;
                }
            }
            else if(valve_action_st == VALVE_CLOSE_TWINKLE)       //关阀灯闪烁
            {
                led_off(OPEN_VALVE_LED);//关闭开阀LED
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
        else if(valve_action_st>1 && valve_action_st<=3)    //常亮
        {
            if(valve_action_st == VALVE_OPEN_CONST){      //开阀灯常亮
                led_on(OPEN_VALVE_LED);
                led_off(CLOSE_VALVE_LED);
            }
            else if(valve_action_st == VALVE_CLOSE_CONST)
            {       //关阀灯常亮
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

