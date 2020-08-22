#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "bsp.h"
#include "app_task.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"
#include "error.h"

//断电检测
void Power_detect(void);

//LCD轮询显示
void lcd_disp_menu(void);

//温度检测
void collect_temp(void);

//LED运行灯闪烁
void led_run_state(void);

//读取温度
s16 read_temp(E_ADC_NUM_Idx adc_num_idx);

/************************************************
函数名称 ： vTask_Power_Display_Temp_Runled
功    能 ： 应用任务3 液晶显示任务
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： yujh_empty@foxmail.com
*************************************************/
void vTask_Power_Display_Temp_Runled(void *pvParameters)
{
    portTickType xLastExecutionTime;
	xLastExecutionTime = xTaskGetTickCount();
    
    //启动之前动作阀门
    valve_triggered();
    
    for(;;)
    {
        //喂狗
        wdg_reset();
        
        //断电检测
#if OUTAGE_POWER_SWITCH
        Power_detect();  
#endif
        
        //LCD轮询显示
#if LCD_SWITCH
        if(g_run_params.auto_test_flag !=1 )
        lcd_disp_menu(); 
#endif
        
        //温度检测
#if IN_OUT_TEMP_SWITCH
        collect_temp();  
#endif  //#if IN_OUT_TEMP_SWITCH             //进回水温度开关
        
        //运行灯状态
        if(g_run_params.auto_test_led_flag != 1)
            led_run_state();
        
        //等待延时
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    //while(1);//正常不会执行到这里
}




#if OUTAGE_POWER_SWITCH

u8 powerdel_flag = 0;//执行过 "掉电开阀动作" 标志
//断电检测
void Power_detect(void)
{
    //掉电检测
    if(exti_read_state(0) == 0)//断电
    {
        if(g_run_params.PowerDownFlag == 0)
        {
            if(powerdel_flag == 0)
            {
                disp_battery_state(E_DISPLAY_ON);   //显示掉电
                powerdel_flag = 1;
                valve_triggered();
            }
        }
        g_run_params.PowerDownFlag = 0;         //断电
    }
    else
    {
        if(g_run_params.PowerDownFlag == 1)
        {
            if(powerdel_flag == 1)
            {
                disp_battery_state(E_DISPLAY_OFF);  //显示上电
                powerdel_flag = 0;
                soft_reset();//系统复位
                //valve_triggered();
            }
        }
        g_run_params.PowerDownFlag = 1;     //上电
        
    }
    
    //PVD
    if(pvd_get_state() == 1)//当低电压时
    {
        while(1)
        {
            disp_clear();//清空显示
            if(pvd_get_state() == 0)//当高电压
                break;
        }
    }
}
#endif

//LCD 轮询显示
void lcd_disp_menu(void)
{
    //显示
    static u8  disp_num = 0;           //显示时间计数
    static u8  disp_menu = 0;          //显示菜单
    u8  temp_array[9] = {0};    //临时数组
    
    /* 循环液晶显示，显示内容: 
        1.错误状态表示符、2.通信地址、3.通信地址、4.显示日期、5.显示时间、6.显示入口温度、
        7.显示出口温度、8.显示温差、9.接通累计时间、10总累计热量 
    */
                        
    //无线实时显示
    if(g_run_params.rf_comm_flag == 0)
        disp_signal_state(E_DISPLAY_OFF);   //信号显示关闭
    else
        disp_signal_state(E_DISPLAY_ON);    //信号显示连接

    //显示系统错误标志
    disp_sys_fault_flag(g_run_params.DevErrState);
    
    //菜单轮询
    switch(disp_menu)
    {
        case 0://阀门运行状态
            if(disp_num == 1)
            {
                disp_sys_fault(g_run_params.DevErrState);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 1://显示阀门sn编号
            if(disp_num == 1)
            {
                memset(temp_array, 0x00, sizeof(temp_array));
                sprintf((char *)temp_array, "%08x", g_sys_params.Device_SN);
                disp_id((u8*)temp_array, 8);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 2://显示阀门GN网络编号
            if(disp_num == 1)
            {
                memset(temp_array, 0x00, sizeof(temp_array));
                sprintf((char *)temp_array, "%04x", g_sys_params.Device_GN);
                disp_gn_id((u8*)temp_array, 4);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 3://显示日期
            if(disp_num == 1)
            {
                disp_systime_year_mon_day();
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 4://显示时间
            disp_systime_hour_min_sec();
            if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
#if IN_OUT_TEMP_SWITCH
        case 5://显示进水温度
            if(disp_num == 1)
            {
                disp_temp_inlet(g_run_params.Input_Temp);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 6://显示回水温度
            if(disp_num == 1)
            {
                disp_temp_return(g_run_params.Output_Temp);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;

        case 7://显示进水温度和回水温度之差
            if(disp_num == 1)
            {
                //根据判断g_run_params.DevErrState 中进回水温度标志有错误则直接显示 --.- ℃
                disp_temp_difference(g_run_params.Input_Temp-g_run_params.Output_Temp, (((g_run_params.DevErrState>>5)&0x03) != 0) ? 1:0);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
#else
        case 5://跳过显示进水温度
            disp_num = 0;
            disp_menu++;
            disp_menu++;
            disp_menu++;
        break;
#endif
        case 8://显示阀门开启累计时间
            if(disp_num == 1)
            {
                //disp_connect_sum_time(i);
                
                disp_connect_sum_time(g_sys_params.Cycle_OpTim/(u32)36);//显示单位计算为小时，显示两位小数点
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 9://显示分摊热量
            if(disp_num == 1)
            {
                //用户的使用热量或者分摊热量(KWH) 含2位小数
                disp_heat_quantity(g_sys_params.Apportion_Energy);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu = 0;
            }
            break;
        default:
            disp_menu = 0;
            break;
    }
    disp_num++;
}

#if IN_OUT_TEMP_SWITCH             //进回水温度开关
//温度检测
void collect_temp(void)
{
    //检测温度
    static u8 collect_temp_num = 0;
    
    //温度检测
    collect_temp_num++;
    if(collect_temp_num >= 5)
    {
        collect_temp_num = 0;
        
        g_run_params.Input_Temp  = read_temp(E_ADC_TEMP_INLET)  + g_sys_params.Offect_Input;
        g_run_params.Output_Temp = read_temp(E_ADC_TEMP_RETURN) + g_sys_params.Offect_Output;
        
        //出水温度错误检测
        if(g_run_params.Output_Temp>1000 || g_run_params.Output_Temp < -100)
            error_set(SYS_ERROR_OUTPUT_TEMP);
        else
            error_clr(SYS_ERROR_OUTPUT_TEMP);
        
        //进水温度错误检测
        if(g_run_params.Input_Temp>1000 || g_run_params.Input_Temp < -100)
            error_set(SYS_ERROR_INPUT_TEMP);
        else
            error_clr(SYS_ERROR_INPUT_TEMP);

    }
}
#endif      //#if IN_OUT_TEMP_SWITCH             //进回水温度开关



//运行灯闪烁
void led_run_state(void)
{
    static FlagStatus led_state = RESET;
    
    if(led_state == RESET)
    {
        led_on(RUN_LED);
        led_state = SET;
    }
    else if(led_state == SET)
    {
        led_off(RUN_LED);
        led_state = RESET;
    }
}


#if IN_OUT_TEMP_SWITCH             //进回水温度开关
//电阻兑换温度
u16 query_temp_by_resistance(u32 r)
{
    u16 temp = 0;

    //temp = (r - 1000000)*10.00/3862.1;
	temp = (r - 1000000)*10.00/3850;

    return temp;
}


//读取温度
s16 read_temp(E_ADC_NUM_Idx adc_num_idx)
{
    u32 adc_temp = 0;
    u32 r = 0;
    u16 read_temp = 0;
    
    adc_init(adc_num_idx);
    adc_temp = adc_sample(adc_num_idx);
    adc_close(adc_num_idx);
    
    r = (u32)((10*adc_temp+79852.5) / (120802.5-10*adc_temp) * 1000.00 * 1000.00);
    if(r <= 1000000)        //小于0℃，默认为0℃
        r = 1000000;
    
//    MYLOG_DEBUG("\r\n R = %u\r\n ", r);
//    MYLOG_DEBUG("\r\n adc_temp = %u\r\n ", adc_temp);

    read_temp = query_temp_by_resistance(r);

    return read_temp;
}
#endif  //#if IN_OUT_TEMP_SWITCH             //进回水温度开关

