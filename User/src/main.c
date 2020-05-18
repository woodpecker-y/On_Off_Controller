/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2019 Hebei Foryon Intelligent Control Co.,Ltd</center></h2>
  ******************************************************************************
  */ 
  
/*
 *                                                                                                        
 *    8 8888888888       ,o888888o.     8 888888888o. `8.`8888.      ,8'  ,o888888o.     b.             8 
 *    8 8888          . 8888     `88.   8 8888    `88. `8.`8888.    ,8'. 8888     `88.   888o.          8 
 *    8 8888         ,8 8888       `8b  8 8888     `88  `8.`8888.  ,8',8 8888       `8b  Y88888o.       8 
 *    8 8888         88 8888        `8b 8 8888     ,88   `8.`8888.,8' 88 8888        `8b .`Y888888o.    8 
 *    8 888888888888 88 8888         88 8 8888.   ,88'    `8.`88888'  88 8888         88 8o. `Y888888o. 8 
 *    8 8888         88 8888         88 8 888888888P'      `8. 8888   88 8888         88 8`Y8o. `Y88888o8 
 *    8 8888         88 8888        ,8P 8 8888`8b           `8 8888   88 8888        ,8P 8   `Y8o. `Y8888 
 *    8 8888         `8 8888       ,8P  8 8888 `8b.          8 8888   `8 8888       ,8P  8      `Y8o. `Y8 
 *    8 8888          ` 8888     ,88'   8 8888   `8b.        8 8888    ` 8888     ,88'   8         `Y8o.` 
 *    8 8888             `8888888P'     8 8888     `88.      8 8888       `8888888P'     8            `Yo 
 */
 
/* 包含的头文件 --------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "board.h"
#include "app_task.h"
#include "bsp_TiMbase.h"
#include "display.h"
#include "sysparams.h"


void hardware_init(void)
{
	struct tm t;
    u8 rv = 0;
    
	//adf_init();//主要是使用软定时器
    
    //PVD初始化
    pvd_init();
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
    
    /*! \brief 调试日志等级  【 L_NONE = 0, 关闭 】【 L_ERROR = 1, 错误 】【 L_WARN  = 2, 警告 】\
                            【 L_TRACE = 3, 轨迹】【 L_INFO  = 4, 信息 】【 L_DEBUG = 5, 调试 】【 L_ALL   = 6, 全部 】 */
    MYLOG_LEVEL(L_DEBUG);
    
    // 串口引脚配置 及 初始化
	com_load_config(com_cfg, sizeof(com_cfg)/sizeof(com_cfg[0]));
	com_init(COM1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,   USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	com_init(COM3, 2400,   USART_WordLength_9b, USART_StopBits_1, USART_Parity_Even, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
    
    // RTC(实时时钟) 引脚配置 及 初始化
    rtc_init();
    rtc_read(&t);
    
    if(t.tm_year < 119)
    {
        t.tm_year = 2019-1900;
        t.tm_mon  = 12-1;
        t.tm_mday = 5;
        t.tm_hour = 11;
        t.tm_min  = 01;
        t.tm_sec  = 0;
        rtc_write(t);
    }
    
    rtc_read(&t);
    MYLOG_DEBUG(" ******************************************************************************\r\n");
    MYLOG_DEBUG("【foryon】-> 【1】System time:%02d-%02d-%02d %02d:%02d:%02d------------------------√√√!\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    MYLOG_DEBUG("【foryon】-> 【2】Debugging serial port load successfully----------------√√√!\r\n");
    MYLOG_DEBUG("【foryon】-> 【3】The communication serial port is loaded successfully---√√√!\r\n");
    MYLOG_DEBUG("【foryon】-> 【4】The real-time clock was loaded successfully------------√√√!\r\n");
    
    // LED 引脚配置 及 初始化
    led_load_config(led_config, sizeof(led_config)/sizeof(led_config[0]));
    led_init();
    MYLOG_DEBUG("【foryon】-> 【5】System running LED state is loaded successfully--------√√√!\r\n");
    
    // ADC 引脚配置 及 初始化
#if IN_OUT_TEMP_SWITCH
    adc_load_config(adc_config, 2);
    MYLOG_DEBUG("【foryon】-> 【6】Temperature probe loaded successfully------------------√√√!\r\n");
#endif  //#if IN_OUT_TEMP_SWITCH             //进回水温度开关
    
    // 外部中断 引脚配置 及 初始化
	exti_load_config(exti_config, sizeof(exti_config)/sizeof(exti_config[0]));
	exti_init();
    MYLOG_DEBUG("【foryon】-> 【7】The power off valve opened and loaded successfully-----√√√!\r\n");
    
    // 外部FLASH 引脚配置 及 初始化
	flash_ext_load_config(&flash_config);
	flash_ext_init();
    //printf("Flash Type:%06x\r\n", flash_ext_read_id());
    MYLOG_DEBUG("【foryon】-> 【8】External flash [Flash Type:%06x] loaded successfully-√√√!\r\n", flash_ext_read_id());
    
    // 文件系统配置 及 搭载
	//flash_ext_load_config(&flash_config);
	//flash_ext_init();
    fs_mount();
	rv = fs_load(E_FILE_TYPE_SYSINF, TEST_FILE_SYSINF, TEST_FILE_SYSINF_SIZE, sizeof(SysParams));
	if (rv != E_FS_OK)
	{
		MYLOG_DEBUG("【foryon】-> 【9】File system load failed--------------------------------XXX!\r\n");
	}
    else
    {
        MYLOG_DEBUG("【foryon】-> 【9】File system loaded successfully------------------------√√√!\r\n");
    }
    rv = fs_load(E_FILE_TYPE_SYSINF, AUTO_TEST_LOG, AUTO_TEST_LOG_SIZE, sizeof(AutoTestStruct));
	if (rv != E_FS_OK)
	{
		while(1)
        {
            MYLOG_DEBUG("【foryon】-> 加载自动测试数据失败 ... ... ------------------------√√√!\r\n");
        }
	}
    // LCD 引脚配置 及 初始化
#if LCD_SWITCH
	lcd_load_config(&lcd_config);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
	lcd_init();
    BSP_GPIO_Init(PORTB, PIN6, GPIO_Mode_IN_FLOATING);//IRQ
    disp_poll();
    disp_signal_state(E_DISPLAY_OFF);//无线显示关闭
    
    MYLOG_DEBUG("【foryon】-> 【10】The LCD screen loaded successfully--------------------√√√!\r\n");
#endif
    
    // RF 引脚配置
    rf_load_config(&rf_config);
    rf_power_on();
    
    // MOTOR(阀门) 引脚配置 及 初始化
    motor_load_config(&motor_config);
    motor_init();
    if(motor_get_state() == MOTOR_STATE_OPENED)
        motor_run(E_MOTOR_SWITCH_CLOSED);
    
    if(motor_get_state() == MOTOR_STATE_CLOSED)
        motor_run(E_MOTOR_SWITCH_OPENED);
    MYLOG_DEBUG("【foryon】-> 【11】Valve drive loaded successfully-----------------------√√√!\r\n");
    MYLOG_DEBUG(" ******************************************************************************\r\n\r\n");
}

void software_init(void)
{
    u8 rv = 0;
    
    //清空系统变量
    memset(&g_sys_params,   0x00, sizeof(g_sys_params));
    memset(&g_run_params,   0x00, sizeof(g_run_params));
    
    rv = fs_read(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params, sizeof(g_sys_params));
    if (rv == E_FS_OK)
    {
        //判断是否出厂
        if(g_sys_params.factory_flag != 1)
        {
            //测试使用 -- 设备信息
            //g_sys_params.Device_SN           = 0x19996001;
            //g_sys_params.Device_GN           = 0x6001;
            
            //保存数据
            sys_factory_reset();
        }
        //写入内存中的时间
        rtc_write(g_sys_params.sys_t);
        
        //检测是否在供暖期
        g_run_params.heating_timer = UNIT_DAY-2;           //供暖期检测延时计数，这样可以立即检测是否在供暖期
    }
    
    /*! \brief 调试日志等级  【 L_NONE = 0, 关闭 】【 L_ERROR = 1, 错误 】【 L_WARN  = 2, 警告 】\
                            【 L_TRACE = 3, 轨迹】【 L_INFO  = 4, 信息 】【 L_DEBUG = 5, 调试 】【 L_ALL   = 6, 全部 】 */
    if(g_sys_params.DebugFlag == 0)
        MYLOG_LEVEL(L_NONE);
    else
        MYLOG_LEVEL(L_DEBUG);
}

#if DEBUG_SWITCH
void system_print(void)
{
    //显示版本号
    disp_sys_version(g_sys_params.Version_H, g_sys_params.Version_S);

    //设备参数
    MYLOG_DEBUG(" ****************************** 设备参数 **************************************\r\n");
    MYLOG_DEBUG("【foryon】-> 【出厂标志】： Factory     = %d\r\n",    g_sys_params.factory_flag);
    MYLOG_DEBUG("【foryon】-> 【设备编号】： Device_SN     = %08x\r\n",  g_sys_params.Device_SN);
    MYLOG_DEBUG("【foryon】-> 【网络编号】： Device_GN     = %02x\r\n",  g_sys_params.Device_GN);
    MYLOG_DEBUG("【foryon】-> 【序----号】： Device_DESN   = %d\r\n",    g_sys_params.Device_DESN);
    MYLOG_DEBUG("【foryon】-> 【硬件版本】： Version_H     = %d\r\n",    g_sys_params.Version_H);
    MYLOG_DEBUG("【foryon】-> 【软件版本】： Version_S     = %d\r\n",    g_sys_params.Version_S);
    MYLOG_DEBUG("【foryon】-> 【设备类型】： DeviceType    = %d\r\n\r\n",g_sys_params.DeviceType);
    
    MYLOG_DEBUG(" **************************** 计量分摊参数 ************************************\r\n");
    MYLOG_DEBUG("【foryon】-> 【房间------信息】 ：%d楼 %d单元 第%d层 %d0%d室  面积%d.%d平米\r\n", 
                                                                        g_sys_params.Buld, \
                                                                        g_sys_params.Unit, \
                                                                        g_sys_params.Floor, \
                                                                        g_sys_params.Floor, \
                                                                        g_sys_params.Cell, \
                                                                        g_sys_params.CellArea/100, \
                                                                        g_sys_params.CellArea%100);
    
    //%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    MYLOG_DEBUG("【foryon】-> 【供暖期开始时间】 ：%02d-%02d-%02d %02d:%02d:%02d    星期%d\r\n",
                                                                        g_sys_params.Start_Time.Year,\
                                                                        g_sys_params.Start_Time.Month,\
                                                                        g_sys_params.Start_Time.Day,\
                                                                        g_sys_params.Start_Time.Hour,\
                                                                        g_sys_params.Start_Time.Minute,\
                                                                        g_sys_params.Start_Time.Second,\
                                                                        g_sys_params.Start_Time.Week);

    MYLOG_DEBUG("【foryon】-> 【供暖期结束时间】 ：%02d-%02d-%02d %02d:%02d:%02d    星期%d\r\n",
                                                                        g_sys_params.Over_Time.Year,\
                                                                        g_sys_params.Over_Time.Month,\
                                                                        g_sys_params.Over_Time.Day,\
                                                                        g_sys_params.Over_Time.Hour,\
                                                                        g_sys_params.Over_Time.Minute,\
                                                                        g_sys_params.Over_Time.Second,\
                                                                        g_sys_params.Over_Time.Week);

    MYLOG_DEBUG("【foryon】-> 【阀门-记录-时间】 ：%02d-%02d-%02d %02d:%02d:%02d    星期%d\r\n\r\n",
                                                                        g_sys_params.RecordTime.Year,\
                                                                        g_sys_params.RecordTime.Month,\
                                                                        g_sys_params.RecordTime.Day,\
                                                                        g_sys_params.RecordTime.Hour,\
                                                                        g_sys_params.RecordTime.Minute,\
                                                                        g_sys_params.RecordTime.Second,\
                                                                        g_sys_params.RecordTime.Week);
    
    MYLOG_DEBUG(" ****************************** 系统记录参数 **********************************\r\n");
    MYLOG_DEBUG("【foryon】-> 【费控----类型】： ControlType           = 0x%02X\r\n",      g_sys_params.ControlType);
    MYLOG_DEBUG("【foryon】-> 【阀门----控制】： ValveCtrlDemandFlg    = 0x%02X\r\n\r\n",  g_sys_params.ValveCtrlDemandFlg);
    
    MYLOG_DEBUG("【foryon】-> 【强制控制类型】： ForceCtrl             = 0x%02X\r\n",      g_sys_params.ForceCtrl);
    MYLOG_DEBUG("【foryon】-> 【温度设置类型】： TempCtrlType          = 0x%02X\r\n",      g_sys_params.TempCtrlType);
    MYLOG_DEBUG("【foryon】-> 【温度--设置值】： TempSetValue          = %d\r\n",          g_sys_params.TempSetValue);
    MYLOG_DEBUG("【foryon】-> 【极限温度类型】： LimtTE_Flag           = 0x%02X\r\n",      g_sys_params.LimtTE_Flag);
    MYLOG_DEBUG("【foryon】-> 【极限--温度值】： LimtTE_Value          = %d\r\n\r\n",      g_sys_params.LimtTE_Value);
    
    MYLOG_DEBUG("【foryon】-> 【累计开阀时间】： Cycle_OpTim           = %d(秒)\r\n",      g_sys_params.Cycle_OpTim);
    MYLOG_DEBUG("【foryon】-> 【分摊----热量】： Apportion_Energy      = %u(kwh)\r\n",     g_sys_params.Apportion_Energy);
    MYLOG_DEBUG("【foryon】-> 【供暖----时间】： loop_time_59          = %d(秒)\r\n",      g_sys_params.loop_time_59);
    MYLOG_DEBUG("【foryon】-> 【剩余----热量】： Remain_Heat           = %d(kwh)\r\n",     g_sys_params.Remain_Heat);
    MYLOG_DEBUG("【foryon】-> 【购买热量总数】： Buy_Total_Heat        = %d(kwh)\r\n\r\n", g_sys_params.Buy_Total_Heat);
    
    MYLOG_DEBUG("【foryon】-> 【校准-温度-点】： Offect_Temp           = %d\r\n",          g_sys_params.Offect_Temp);
    MYLOG_DEBUG("【foryon】-> 【低区----斜率】： RakeRatioOffect_L     = %u\r\n",          g_sys_params.RakeRatioOffect_L);
    MYLOG_DEBUG("【foryon】-> 【高区----斜率】： RakeRatioOffect_H     = %d\r\n",          g_sys_params.RakeRatioOffect_H);
    MYLOG_DEBUG("【foryon】-> 【入口温度偏差】： Offect_Input          = %d\r\n",          g_sys_params.Offect_Input);
    MYLOG_DEBUG("【foryon】-> 【出口温度偏差】： Offect_Output         = %d\r\n",          g_sys_params.Offect_Output);
    MYLOG_DEBUG(" *****************************************************************************\r\n\r\n");
}
#endif

int main(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    // 硬件初始化
    hardware_init();
    // 软件初始化
    software_init();
    // 系统输出
#if DEBUG_SWITCH
    system_print();
#endif
    
    //看门狗初始化
    wdg_init(5);
    /* 基本定时器初始化	*/
	BASIC_TIM_Init();
    
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
    
    /* 在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
    这样做的好处是：
        1. 防止执行的中断服务程序中有FreeRTOS的API函数。
        2. 保证系统正常启动，不受别的中断影响。
        3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
    在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)
    和cpsie i是等效的。*/
	__set_PRIMASK(1); //这里禁止全局中断(除了NMI和HardFault)。在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。
    
    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,             /* 创建任务的入口函数 */
                         (const char*     )"AppTaskCreate",           /* 创建任务的名字 */
                         (uint16_t        )64,                        /* 创建任务的栈大小 */
                         (void*           )NULL,                      /* 创建任务的入口函数参数 */
                         (UBaseType_t     )1,                         /* 创建任务的优先级 */
                         (TaskHandle_t*   )&AppTaskCreate_Handle);    /* 创建任务的控制块指针 */ 
    
    /* 启动任务调度 */
    if(pdPASS == xReturn)
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    else
        return -1;
    
    while(1);   /* 正常不会执行到这里 */  
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2019 Hebei Foryon Intelligent Control Co.,Ltd *****END OF FILE****/
