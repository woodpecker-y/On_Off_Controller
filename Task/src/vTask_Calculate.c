#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "stdlib.h"
#include "bsp.h"
#include "app_task.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"

//检测供暖期
u8 heating_period(void);

//RTC和系统时钟 对比 时间
void Check_RTC(void);

/************************************************
函数名称 ： vAppTask1
功    能 ： 应用任务2 LED闪烁，系统运行状态灯，及兼顾检测温度任务
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： yujh_empty@foxmail.com
*************************************************/
void vTask_Calculate(void *pvParameters)
{
    
    portTickType xLastExecutionTime;
    xLastExecutionTime = xTaskGetTickCount();
    u32 rf433_init_timer = 0;
	struct tm t;
    
    //自动RTC和系统时钟 对比 时间
    rtc_read(&t);
    g_run_params.Rtc_check_sec = t.tm_hour*60*60+t.tm_min*60+t.tm_sec;
    
    for(;;)
    {
//温控器失联自动开阀
#if RF_LOSS_OPEN_VALVE_SWITCH
        {
            //无线计时  超过60分钟没有和温控器连接则显示失联
            g_run_params.rf_comm_timer++;

            //一个小时没有和温控器建立连接则表示失联，则阀门在用户控制模式下自动打开，阀门显示失联
            if(g_run_params.rf_comm_timer >= UNIT_HOUR)//  UNIT_HOUR
            {
                g_run_params.rf_comm_timer = 0;   //阀门和温控器失联计时
                g_run_params.rf_comm_flag = 0;  //温控器失联标志
                
                //控费类型  强制控制  在供暖期
                if(g_sys_params.ControlType != 0xAA && g_sys_params.ForceCtrl == 0xFF && g_run_params.WorkFlg == 0xAA)//未启用控费类型,并且强制打开或者强制关闭情况下不允许自动打开
                {
                    valve_triggered();
                    g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                }
            }
        }
#endif
        //每天 检测供暖期
        {
            g_run_params.heating_timer++;
            if(g_run_params.heating_timer >= UNIT_DAY)//一天检测一次供暖期
            {
                g_run_params.heating_timer = 0;
                if(heating_period() == 0)
                    g_run_params.WorkFlg = 0xAA;//更新是否在供暖季标志 //供暖季标志记录  0XAA 在供暖季   0X00 不在供暖季
                else
                    g_run_params.WorkFlg = 0x00;//更新是否在供暖季标志 //供暖季标志记录  0XAA 在供暖季   0X00 不在供暖季
                
                //每天检查一次是否欠费，如果欠费则关闭阀门
                if(g_sys_params.ControlType == 0xAA)
                {
                    valve_triggered();
                    g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                }
                
                //读取RTC实时时钟
                rtc_read(&g_sys_params.sys_t);
                //保存数据
                save_params(SYS_SAVE_RECORD_PARAMS);//主要记录累计开阀时间
            }
        }
        
        //在供暖期 10分钟计时保存 累计开阀时间
        {
            if(g_run_params.WorkFlg == 0xAA)
            {
                if(g_run_params.ValveState == VALVE_OPEN)
                {
                    //定时器10分钟存储一次累计开阀时间
                    g_run_params.save_cycle_optim_timer++;
                    if(g_run_params.save_cycle_optim_timer >= 10*UNIT_MIN)
                    {
                        g_run_params.save_cycle_optim_timer = 0;
                        
                        //读取RTC实时时钟
                        rtc_read(&g_sys_params.sys_t);
                        //保存数据
                        save_params(SYS_SAVE_RECORD_PARAMS);//主要记录累计开阀时间
                    }
                    //累计开阀时间 自加1秒
                    g_sys_params.Cycle_OpTim++;
                }
            }
        }
        
//一周 自转功能
#if ONE_WEEK_SWITCH
        {
            g_run_params.one_week_turn_timer++;
            if(g_run_params.one_week_turn_timer >= ONE_WEEK_TURN_TIME)
            {
                //当 g_run_params.one_week_turn_timer == 7*UNIT_DAY仅执行一次
                if(g_run_params.one_week_turn_timer == ONE_WEEK_TURN_TIME)
                {
                    if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)
                        g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                    else
                        g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                    valve_triggered();
                }
                
                //再次恢复之前阀门状态
                if(g_run_params.one_week_turn_timer >= ONE_WEEK_TURN_TIME+18)
                {
                    g_run_params.one_week_turn_timer = 0;
                    if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)
                        g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                    else
                        g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                    valve_triggered();
                }
            }
        }
#endif
        //一小时计时
        {
            //自动RTC和系统时钟 对比 时间
            if(g_run_params.heating_timer < UNIT_HOUR)
            {
                //根据供暖期定时器，实现阀门启动一小时前是5秒检测一次RTC功能
                g_run_params.Check_time = 5;
                g_run_params.Deviation = 1;//5秒之内的误差
            }
            else
            {
                g_run_params.Check_time = 10*UNIT_MIN;
                g_run_params.Deviation = 3;//10分钟之内的误差
            }
            
            if(g_run_params.Rtc_check_timer >= g_run_params.Check_time)
            {
                g_run_params.Rtc_check_timer = 0; 
                Check_RTC();
            }
            g_run_params.Rtc_check_timer++;
        }
        
        //自动初始化无线每35分钟初始化一次无线模块
        {
            rf433_init_timer++;
            if(rf433_init_timer >= 35*UNIT_MIN)
            {
                rf433_init_timer = 0;
                rf433_init();//初始化无线
            }
        }
        
        //等待延时
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );//1000 单位ms
    }
    
    //while(1);//正常不会执行到这里
}

//RTC和系统时钟 对比 时间
void Check_RTC(void)
{
    int check_rtc_sec = 0;
    int Deviation = 0;

    //读取RTC时间
    rtc_read(&g_sys_params.sys_t);
    
    //RTC和系统时钟 对比 RTC
    if(g_sys_params.sys_t.tm_hour < 23)  //防止在24点中检测，导致计算时间秒数不正确
    {
        //计算总共秒数
        check_rtc_sec = (int)( (u32)(g_sys_params.sys_t.tm_hour*60*60+g_sys_params.sys_t.tm_min*60+g_sys_params.sys_t.tm_sec) - (u32)g_run_params.Rtc_check_sec );
        
        //计算误差
        Deviation = abs(check_rtc_sec) - g_run_params.Check_time;
        
        if( abs(Deviation) > g_run_params.Deviation )//误差在 g_run_params.Deviation 秒范围内
            error_set(SYS_ERROR_RTC);
        else
            error_clr(SYS_ERROR_RTC);
    }

    g_run_params.Rtc_check_sec = g_sys_params.sys_t.tm_hour*60*60+g_sys_params.sys_t.tm_min*60+g_sys_params.sys_t.tm_sec;
    
    //保存数据
    save_params(SYS_SAVE_RECORD_PARAMS);//主要记录累计开阀时间
    
}

//查询是否在供暖期
//return 0 在
//return 1 不在
u8 heating_period(void)
{
    u8 rv = 0;
    u16 cur = 0, st = 0, et = 0;
    struct tm t;
    
    rtc_read(&t);
    
    cur = (t.tm_mon+1)*100 + t.tm_mday;
    
    st  = g_sys_params.Start_Time.Month*100 + g_sys_params.Start_Time.Day;
    et  = g_sys_params.Over_Time.Month*100 + g_sys_params.Over_Time.Day;
    
    //printf("Cur:%d\r\n St:%d\r\n Et:%d\r\n", cur, st, et);
    //判断是否在供暖期内，并且是否是在供暖期年份
    if((cur>=st || cur<=et) && (g_sys_params.Start_Time.Year == t.tm_year+1900 || g_sys_params.Over_Time.Year == t.tm_year+1900))
    {
        rv = 0;
        //printf("#### 在供暖期000 ####\r\n");
    }
    else
    {
        rv = 1;
        //printf("#### 不在供暖期111 ####\r\n");
    }
    return rv;
}


