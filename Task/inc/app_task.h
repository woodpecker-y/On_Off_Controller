/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _APP_TASK_H
#define _APP_TASK_H

/* �궨�� --------------------------------------------------------------------*/
/* ��������ջ��С   -----------------------------------------------------------*/
#define APPTASKCREATE_SIZE                  100
#define TASK_COM_RF_DATA_SIZE               256
#define TASK_DISPLAY_SIZE                   256
#define TASK_VALVE_CALCULATER_SIZE          256
#define TASK_SYSTEM_RUN_STATE_SIZE          256

/* �������ȼ�   -----------------------------------------------------------*/
#define TASK_COM_RF_DATA_PRIORITY           4
#define TASK_DISPLAY_PRIORITY               3
#define TASK_VALVE_CALCULATER_PRIORITY      2
#define TASK_SYSTEM_RUN_STATE_PRIORITY      1

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "task.h"
#include "vTask_Com_Rf_Valve.h"
#include "vTask_Power_Display_Temp_Runled.h"
#include "vTask_Calculate.h"
#include "vTaskRunTimeStats.h"

/* ������ �������� -----------------------------------------------------------*/
extern TaskHandle_t     AppTaskCreate_Handle;           /* ���������� */
extern TaskHandle_t     Task_Display_Handle;            /* LCD��ʾ������ */
extern TaskHandle_t     Task_System_Run_State_Handle;   /* ϵͳ����״̬������ */
extern TaskHandle_t     Task_COM_RF_DATA_Handle;        /* ���ݽ��մ��������� */
extern TaskHandle_t     Task_Valve_Calculater_Handle;   /* ���ݼ��㴦�� */

/* �������� ------------------------------------------------------------------*/
void AppTaskCreate(void);

#endif /* _APP_TASK_H */




