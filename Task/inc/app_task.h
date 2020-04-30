/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _APP_TASK_H
#define _APP_TASK_H

/* 宏定义 --------------------------------------------------------------------*/
/* 创建任务栈大小   -----------------------------------------------------------*/
#define APPTASKCREATE_SIZE                  100
#define TASK_COM_RF_DATA_SIZE               256
#define TASK_DISPLAY_SIZE                   256
#define TASK_VALVE_CALCULATER_SIZE          256
#define TASK_SYSTEM_RUN_STATE_SIZE          256

/* 任务优先级   -----------------------------------------------------------*/
#define TASK_COM_RF_DATA_PRIORITY           4
#define TASK_DISPLAY_PRIORITY               3
#define TASK_VALVE_CALCULATER_PRIORITY      2
#define TASK_SYSTEM_RUN_STATE_PRIORITY      1

/* 包含的头文件 --------------------------------------------------------------*/
#include "task.h"
#include "vTask_Com_Rf_Valve.h"
#include "vTask_Power_Display_Temp_Runled.h"
#include "vTask_Calculate.h"
#include "vTaskRunTimeStats.h"

/* 任务句柄 变量申明 -----------------------------------------------------------*/
extern TaskHandle_t     AppTaskCreate_Handle;           /* 创建任务句柄 */
extern TaskHandle_t     Task_Display_Handle;            /* LCD显示任务句柄 */
extern TaskHandle_t     Task_System_Run_State_Handle;   /* 系统运行状态任务句柄 */
extern TaskHandle_t     Task_COM_RF_DATA_Handle;        /* 数据接收处理任务句柄 */
extern TaskHandle_t     Task_Valve_Calculater_Handle;   /* 数据计算处理 */

/* 函数申明 ------------------------------------------------------------------*/
void AppTaskCreate(void);

#endif /* _APP_TASK_H */




