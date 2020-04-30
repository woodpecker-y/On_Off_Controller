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

void auto_test(void);

/************************************************
函数名称 ： vTaskRunTimeStats
功    能 ： 应用任务3   系统运行状态任务
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： yujh_empty@foxmail.com
*************************************************/

void vTaskRunTimeStats(void *pvParameters)
{
    uint8_t CPU_RunInfo[400];		            //保存任务运行时间信息
//    portTickType xLastExecutionTime;
//	xLastExecutionTime = xTaskGetTickCount();
    printf("%s",foryon);
    vTaskSuspend( NULL );//挂起任务
    
    while (1)
    {
        if(g_run_params.auto_test_flag != 1)
        {
            memset(CPU_RunInfo,0,400);				//信息缓冲区清零

            vTaskList((char *)&CPU_RunInfo);        //获取任务运行时间信息
            
            printf("***********************************************************************\r\n");
            printf("任务名\t      任务状态\t优先级\t   剩余栈\t任务序号\r\n");
            printf("***********************************************************************\r\n");
            printf("%s", CPU_RunInfo);


            memset(CPU_RunInfo,0,400);				//信息缓冲区清零
            vTaskGetRunTimeStats((char *)&CPU_RunInfo);
            printf("***********************************************************************\r\n");
            printf("任务名\t         运行计数\t           使用率\r\n");
            printf("***********************************************************************\r\n");
            printf("%s", CPU_RunInfo);
            printf("-----------------------------------------------------------------------\r\n\r\n");
        }
        else
        {
            //自动化测试
            auto_test();
        }
        
        vTaskSuspend( NULL );//挂起任务
       //vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)5000 );   /* 延时500个tick */		
    }
    
    //while(1);//正常不会执行到这里
}


void auto_test(void)
{
    portTickType xLastExecutionTime;
	xLastExecutionTime = xTaskGetTickCount();
    
#if LCD_SWITCH
    char disp_num[9] = {0};
#endif
    
    u8 loop = 0;
    u8 rv = 0;
    
    u8 valve_error_flag = 0;
    u8 temp_error_flag  = 0;
    u8 flash_error_flag = 0;
    u8 mbus_error_flag  = 0;
    u8 rf_error_flag    = 0;
    u8 rtc_error_flag   = 0;
    
    printf("【foryon】->               ....................................................      \r\n");
    printf("【foryon】->            ..........................................................   \r\n");
    printf("【foryon】->         ............... 开始自动化测试，请关注以下打印信息 ............... \r\n");
    printf("【foryon】->            ..........................................................   \r\n");
    printf("【foryon】->               ....................................................      \r\n\r\n");
    
    //液晶屏测试
#if LCD_SWITCH//                                    【测试项目（1）  液晶屏测试】
    printf("【foryon】-> ---------------------------【测试项目（1） 液晶屏测试】---------------------------r\n");//项目1
    printf("【foryon】-> 液晶屏 开始测试 ");
    disp_clear();//清除显示
    disp_poll();//全部显示

    for(loop=0; loop<=9; loop++)//循环显示
    {
        sprintf(disp_num, "%d%d%d%d%d%d%d%d",loop,loop,loop,loop,loop,loop,loop,loop);
        disp_str(1, 8, disp_num);
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)500 );
        printf(".");
    }
    disp_clear();//清除显示
    printf("\r\n");
    printf("【foryon】-> 液晶屏测试完成，请记录是否有故障！\r\n\r\n");
#endif
    
    
    
    
    //LED测试                                       【测试项目（2）  LED灯测试】
    printf("【foryon】-> ---------------------------【测试项目（2） LED灯测试】---------------------------r\n");//项目2
    printf("【foryon】-> LED灯 开始测试 ");
    g_run_params.auto_test_led_flag = 1;
    led_off(0);
    led_off(1);
    led_off(2);
    
    for(int i=0; i<5; i++)
    {
        for(loop=0; loop<3; loop++)
        {
            led_on(loop);
            vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)100 );
            printf(".");
            
            if(loop>=2)
            {
                led_off(0);led_off(1);led_off(2);
                vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)400 );
            }
        }
    }
    
    led_off(0);led_off(1);led_off(2);
    g_run_params.auto_test_led_flag = 0;
    printf("\r\n");
    printf("【foryon】-> LED灯测试完成，请记录是否有故障！\r\n\r\n");

    
    
    
    
    //电机测试                                      【测试项目（3）   电机测试】
    printf("【foryon】-> ---------------------------【测试项目（3）  电机测试】---------------------------r\n");//项目3
    printf("【foryon】-> 阀门电机 开始测试 ... ...\r\n");
    int valve_timer = 0;
    
    g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
    valve_triggered();
    printf("【foryon】-> 阀门正在打开");
    while(1)
    {
        valve_timer++;
        if(valve_timer > 5)
        {
            printf("\r\n");
            printf("【Error 】-> 阀门打开异常 【电机 驱动芯片 问题或者关闭电机的到位检测线没有接好】！ X X X \r\n\r\n");
            valve_error_flag = 1;
           break;
        }
        if(motor_get_state() == MOTOR_STATE_OPENED)
        {
            printf("\r\n");
            printf("【foryon】-> 阀门已打开\r\n");
            break;
        }
        
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 ); 
        printf(".");
    }
    
    valve_timer = 0;
    g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
    valve_triggered();
    printf("【foryon】-> 阀门正在关闭");
    while(1)
    {
        valve_timer++;
        if(valve_timer > 5)
        {
            printf("\r\n");
            printf("【Error 】-> 阀门关闭异常！【电机 驱动芯片 问题或者关闭电机的到位检测线没有接好】! X X X \r\n\r\n");
            valve_error_flag = 1;
            break;
        }
        if(motor_get_state() == MOTOR_STATE_CLOSED)
        {
            printf("\r\n");
            printf("【foryon】-> 阀门已关闭\r\n");
            break;
        }
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
        printf(".");
    }
    if(valve_error_flag != 1)
        printf("【foryon】-> 阀门电机正常，测试完成 ... ...\r\n\r\n");

    
    
    
#if IN_OUT_TEMP_SWITCH             //进回水温度开关
    //进回水温度测试                                 【测试项目（4）   进回水温度测试】
    printf("【foryon】-> ---------------------------【测试项目（4）  进回水温度测试】---------------------r\n");//项目4
    printf("【foryon】-> 进回水温度开始测试 ... ...\r\n");
#if LCD_SWITCH
    disp_clear();//清除显示
    disp_poll();//全部显示
#endif
    
    //进水
    printf("【foryon】-> 进水温度是：%d.%d℃\r\n", g_run_params.Input_Temp/10, g_run_params.Input_Temp%10);

#if LCD_SWITCH
    disp_temp_inlet(g_run_params.Input_Temp);
#endif
    
    if(g_run_params.Input_Temp<0 && g_run_params.Input_Temp*(-1) >= 1000){
        printf("【Error 】-> 进水温度异常 【电阻线没有焊接好，或者探头损坏问题或者连焊】! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    else if(g_run_params.Input_Temp > 1000){
        printf("【Error 】-> 进水温度异常 【电阻线没有焊接好，或者探头损坏问题或者连焊】! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    
    //回水
    printf("【foryon】-> 回水温度是：%d.%d℃\r\n", g_run_params.Output_Temp/10, g_run_params.Output_Temp%10);

#if LCD_SWITCH
    disp_temp_return(g_run_params.Output_Temp);
#endif
    
    if(g_run_params.Output_Temp<0 && g_run_params.Output_Temp*(-1) >= 1000){
        printf("【Error 】-> 回水温度异常!【电阻线没有焊接好，或者探头损坏问题或者连焊】! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    else if(g_run_params.Output_Temp > 1000){
        printf("【Error 】-> 回水温度异常!【电阻线没有焊接好，或者探头损坏问题或者连焊】! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    
    //温差
    printf("【foryon】-> 温差 温度是：%d.%d℃\r\n", (g_run_params.Input_Temp-g_run_params.Output_Temp)/10, (g_run_params.Input_Temp-g_run_params.Output_Temp)%10);
    //根据判断g_run_params.DevErrState 中进回水温度标志有错误则直接显示 --.- ℃
    
#if LCD_SWITCH
    disp_temp_difference(g_run_params.Input_Temp-g_run_params.Output_Temp, (((g_run_params.DevErrState>>5)&0x03) != 0) ? 1:0);
#endif
    
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    if(abs(g_run_params.Input_Temp-g_run_params.Output_Temp)>10){
        printf("【Error 】-> 进回水温度测试，温差较大！X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    if(temp_error_flag != 1)
        printf("【foryon】-> 进回水温度正常，测试完成 ... ...\r\n\r\n");
#endif    //#if IN_OUT_TEMP_SWITCH             //进回水温度开关
    
    
    

    //FLASH测试                                     【测试项目（5）   存储芯片测试】
    printf("【foryon】-> ---------------------------【测试项目（5）  存储芯片测试】-----------------------r\n");//项目5
    printf("【foryon】-> 存储器 开始测试 ... ...\r\n");
    AutoTest.test_0 = '0';
    AutoTest.test_1 = 11;
    AutoTest.test_2 = -22;
    AutoTest.test_3 = 33;
    AutoTest.test_4 = -44;
    AutoTest.test_5 = 55;
    AutoTest.test_6 = -66;
    AutoTest.test_7 = 77;
    //保存数据
    rv = fs_write(AUTO_TEST_LOG, 0, (u8*)&AutoTest, sizeof(AutoTestStruct));
    if (rv != E_FS_OK){
        printf("【Error 】-> 写入数据存储异常！【存储芯片 异常，请重新焊接 FLASH】! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    memset(&AutoTest,   0x00, sizeof(AutoTestStruct));
    rv = fs_read(AUTO_TEST_LOG, 0, (u8*)&AutoTest, sizeof(AutoTest));
    if (rv != E_FS_OK){
        printf("【Error 】-> 读取数据存储异常！【存储芯片 异常，请重新焊接 FLASH】! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    if(AutoTest.test_0!='0' || AutoTest.test_1!=11  || AutoTest.test_2!=-22 || \
       AutoTest.test_3!=33  || AutoTest.test_4!=-44 || AutoTest.test_5!=55  || \
       AutoTest.test_6!=-66 || AutoTest.test_7!=77)
    {
        printf("【Error 】-> 对比存储数据异常！【存储芯片 异常，请更换 FLASH】! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    if(flash_error_flag != 1)
        printf("【foryon】-> 存储器正常，测试完成 ... ...\r\n\r\n");

    
    
    
    
    //断电开阀测试

    
    
    

    //MBUS测试                                      【测试项目（6）   MBUS测试】
    printf("【foryon】-> ---------------------------【测试项目（6）  MBUS测试】---------------------------r\n");//项目6
    printf("【foryon】-> MBUS总线 开始测试 ... ...\r\n");
    u8  Mbus_timer = 0;
    u8* com3_rcv_ptr;
    u16 com3_rcv_len;
    u8 test_array[15] = "www.foryon.com";
    
    com_send_data(COM3, test_array, sizeof(test_array));
    while(Mbus_timer < 5)
    {
        Mbus_timer++;
        rv = com_recv_data(COM3, &com3_rcv_ptr, &com3_rcv_len);
        if (E_COM_OK == rv)
        {
           if( memcmp( com3_rcv_ptr, test_array, com3_rcv_len) != 0)
           {
                com_recv_init(COM3);
                printf("【Error 】-> MBUS数据 接收数据对比异常！异常！X X X \r\n\r\n");
                mbus_error_flag = 1;
           }
           else
           {//MBUS数据对比正常
               com_recv_init(COM3);
               break;
           }
           
        }
        
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    if(Mbus_timer>=5)
    {
        printf("【Error 】-> MBUS数据 接收超时 异常！X X X \r\n\r\n");
        mbus_error_flag = 1;
    }
    if(mbus_error_flag != 1)
        printf("【foryon】-> MBUS总线正常，测试完成 ... ...\r\n\r\n");

    
    
    

    //RF测试                                        【测试项目（7）  无线测试】
    printf("【foryon】-> ---------------------------【测试项目（7） 无线测试】----------------------------r\n");//项目7
    printf("【foryon】-> RF无线 开始测试 ... ...\r\n");
    u8 rf_timer = 0;
    u8* rf_rcv_ptr;
    u8 rf_rcv_len;
    
    rv = rf_init(RF_FRE_420_5M, RF_DATA_RATE_1200, TXPOW_20dBm, 70);//这里初始化为420.5M  所以阀门程序也要设置为这个频段才能通讯成功
    if (rv != E_RF_OK)
    {
        printf("【Error 】-> 无线模块 配置发生 异常！X X X \r\n\r\n");
        rf_error_flag = 1;
    }
    rf_rcv_init();
    
    rf_snd_data(test_array, sizeof(test_array));
    while(rf_timer < 5)
    {
        rf_timer++;
        rv = rf_rcv_data(&rf_rcv_ptr, &rf_rcv_len);
        if (E_COM_OK == rv)
        {
           if( memcmp( rf_rcv_ptr, test_array, rf_rcv_len) != 0)
           {
                rf_rcv_init();
                printf("【Error 】-> RF无线 接收数据对比异常！ 请重新检查无线模块！X X X \r\n\r\n");
                rf_error_flag = 1;
           }
           else
           {//RF接收数据对比正常
               rf_rcv_init();
               break;
           }

        }
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    if(rf_timer>=5)
    {
        printf("【Error 】-> RF无线 接收超时！ 请重新检查无线模块！X X X \r\n\r\n");
        rf_error_flag = 1;
    }
    if(rf_error_flag != 1)
        printf("【foryon】-> 无线正常，测试完成 ... ...\r\n\r\n");
    rf433_init();   //无线测试完成，重新恢复之前配置

    
    

    

    //RTC测试
    {   //                                              【测试项目（8）   时钟测试】
        printf("【foryon】-> ---------------------------【测试项目（8）  时钟测试】----------------------------r\n");//项目8
        printf("【foryon】-> 实时时钟 开始测试 ... ...\r\n");
        if( (g_run_params.DevErrState>>7) == 1)
        {
            printf("【Error 】-> 实时时钟 异常，请更换晶振重试！X X X \r\n\r\n");
            rtc_error_flag = 1;
        }
        if(rtc_error_flag != 1)
            printf("【foryon】-> 时钟正常，测试完成 ... ...\r\n\r\n");
    }

    //测试完成
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    printf("\r\n \r\n \r\n \r\n");
//    printf("【foryon】->         ..............................................................\r\n");
//    printf("【foryon】->             ................... 生成测试结果 .....................     \r\n");
//    printf("【foryon】->         ..............................................................\r\n\r\n");
    
    printf("【foryon】->               ....................................................      \r\n");
    printf("【foryon】->            ..........................................................   \r\n");
    printf("【foryon】->         ......................... 生成测试结果 .......................... \r\n");
    printf("【foryon】->            ..........................................................   \r\n");
    printf("【foryon】->               ....................................................      \r\n\r\n");
    
    if(valve_error_flag==1 || temp_error_flag==1 || flash_error_flag==1 ||\
        mbus_error_flag==1 || rf_error_flag==1   || rtc_error_flag==1)
    {
        printf("【总结故障项】-> ");
        if(valve_error_flag == 1)
        {
            printf("电机故障(3)、");
        }
        if(temp_error_flag == 1)
        {
            printf("进回水温度故障(4)、");
        }
        if(flash_error_flag == 1)
        {
            printf("存储芯片故障(5)、");
        }
        if(mbus_error_flag == 1)
        {
            printf("MBUS总线故障(6)、");
        }
        if(rf_error_flag == 1)
        {
            printf("无线故障(7)、");
        }
        if(rtc_error_flag == 1)
        {
            printf("时钟故障(8)");
        } 
        printf("\r\n");        
    }
    else
    {
        printf("【foryon】-> 全部硬件检测完毕全部正常，可组装生产 ... ...\r\n");   
    }

    g_run_params.auto_test_flag = 0;//关闭自动化测试，打开之前屏蔽一些和自动化测试冲突任务
    
}

 

 

