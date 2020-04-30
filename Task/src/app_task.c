#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "bsp.h"
#include "board.h"
#include "dbg.h"
#include "display.h"
#include "sysparams.h"
#include "app_task.h"

/***
 *                                                        
 *        ,...                                            
 *      .d' ""                                            
 *      dM`                                               
 *     mMMmm,pW"Wq.`7Mb,od8 `7M'   `MF',pW"Wq.`7MMpMMMb.  
 *      MM 6W'   `Wb MM' "'   VA   ,V 6W'   `Wb MM    MM  
 *      MM 8M     M8 MM        VA ,V  8M     M8 MM    MM  
 *      MM YA.   ,A9 MM         VVV   YA.   ,A9 MM    MM  
 *    .JMML.`Ybmd9'.JMML.       ,V     `Ybmd9'.JMML  JMML.
 *                             ,V                         
 *                          OOb"                          
 */
 
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
TaskHandle_t     AppTaskCreate_Handle          = NULL;                          /* 创建任务句柄 */
TaskHandle_t     Task_Display_Handle           = NULL;                          /* LCD显示任务句柄 */
TaskHandle_t     Task_System_Run_State_Handle  = NULL;                          /* 系统运行状态任务句柄 */
TaskHandle_t     Task_COM_RF_DATA_Handle       = NULL;                          /* 数据接收处理任务句柄 */
TaskHandle_t     Task_Valve_Calculater_Handle  = NULL;

/************************************************
函数名称 ： AppTaskCreate
功    能 ： 创建任务
参    数 ： 无
返 回 值 ： 无
作    者 ： yujh_empty@foxmail.com
*************************************************/
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL();           //进入临界区

        /* 创建 Com_RF_DATA 任务 */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Com_Rf_Valve,                  /* 串口及无线数据处理任务 */
                        (const char*    )"RECV_DATA",                           /* 任务名字 */
                        (uint16_t       )TASK_COM_RF_DATA_SIZE,                 /* 任务栈大小 */
                        (void*          )NULL,                                  /* 任务入口函数参数 */
                        (UBaseType_t    )TASK_COM_RF_DATA_PRIORITY,             /* 任务的优先级 */
                        (TaskHandle_t*  )&Task_COM_RF_DATA_Handle);             /* 任务控制块指针 */
    if(pdPASS == xReturn)
        printf("【foryon】-> 创建 RECV_DATA--任务成功!\r\n");
    
    /* 创建 显示 及 LED 任务 */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Power_Display_Temp_Runled,     /* LCD显示任务 */
                        (const char*    )"LCD_DISP",                            /* 任务名字 */
                        (uint16_t       )TASK_DISPLAY_SIZE,                     /* 任务栈大小 */
                        (void*          )NULL,                                  /* 任务入口函数参数 */
                        (UBaseType_t    )TASK_DISPLAY_PRIORITY,                 /* 任务的优先级 */
                        (TaskHandle_t*  )&Task_Display_Handle);                 /* 任务控制块指针 */
    if(pdPASS == xReturn)
        printf("【foryon】-> 创建 LCD_DISP---任务成功!\r\n");

        /* 创建 阀门计算 任务 */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Calculate,                     /* 系统运行状态任务 */
                        (const char*    )"CALCULATER",                          /* 任务名字 */
                        (uint16_t       )TASK_VALVE_CALCULATER_SIZE,            /* 任务栈大小 */
                        (void*          )NULL,                                  /* 任务入口函数参数 */
                        (UBaseType_t    )TASK_VALVE_CALCULATER_PRIORITY,        /* 任务的优先级 */
                        (TaskHandle_t*  )&Task_Valve_Calculater_Handle);        /* 任务控制块指针 */
    if(pdPASS == xReturn)
        printf("【foryon】-> 创建 CALCULATER-任务成功!\r\n");

        /* 创建 Sys_Run_ST 任务 */
    xReturn = xTaskCreate((TaskFunction_t )vTaskRunTimeStats,                   /* 系统运行状态任务 */
                        (const char*    )"RUN_ST",                              /* 任务名字 */
                        (uint16_t       )TASK_SYSTEM_RUN_STATE_SIZE,            /* 任务栈大小 */
                        (void*          )NULL,	                                /* 任务入口函数参数 */
                        (UBaseType_t    )TASK_SYSTEM_RUN_STATE_PRIORITY,        /* 任务的优先级 */
                        (TaskHandle_t*  )&Task_System_Run_State_Handle);        /* 任务控制块指针 */
    if(pdPASS == xReturn)
        printf("【foryon】-> 创建 RUN_ST-----任务成功!\r\n\r\n");
    
    //vTaskDelete(AppTaskCreate_Handle);    //删除AppTaskCreate任务
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                //退出临界区
}





