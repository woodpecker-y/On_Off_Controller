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
 
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
TaskHandle_t     AppTaskCreate_Handle          = NULL;                          /* ���������� */
TaskHandle_t     Task_Display_Handle           = NULL;                          /* LCD��ʾ������ */
TaskHandle_t     Task_System_Run_State_Handle  = NULL;                          /* ϵͳ����״̬������ */
TaskHandle_t     Task_COM_RF_DATA_Handle       = NULL;                          /* ���ݽ��մ��������� */
TaskHandle_t     Task_Valve_Calculater_Handle  = NULL;

/************************************************
�������� �� AppTaskCreate
��    �� �� ��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� yujh_empty@foxmail.com
*************************************************/
void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL();           //�����ٽ���

        /* ���� Com_RF_DATA ���� */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Com_Rf_Valve,                  /* ���ڼ��������ݴ������� */
                        (const char*    )"RECV_DATA",                           /* �������� */
                        (uint16_t       )TASK_COM_RF_DATA_SIZE,                 /* ����ջ��С */
                        (void*          )NULL,                                  /* ������ں������� */
                        (UBaseType_t    )TASK_COM_RF_DATA_PRIORITY,             /* ��������ȼ� */
                        (TaskHandle_t*  )&Task_COM_RF_DATA_Handle);             /* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
        printf("��foryon��-> ���� RECV_DATA--����ɹ�!\r\n");
    
    /* ���� ��ʾ �� LED ���� */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Power_Display_Temp_Runled,     /* LCD��ʾ���� */
                        (const char*    )"LCD_DISP",                            /* �������� */
                        (uint16_t       )TASK_DISPLAY_SIZE,                     /* ����ջ��С */
                        (void*          )NULL,                                  /* ������ں������� */
                        (UBaseType_t    )TASK_DISPLAY_PRIORITY,                 /* ��������ȼ� */
                        (TaskHandle_t*  )&Task_Display_Handle);                 /* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
        printf("��foryon��-> ���� LCD_DISP---����ɹ�!\r\n");

        /* ���� ���ż��� ���� */
    xReturn = xTaskCreate((TaskFunction_t )vTask_Calculate,                     /* ϵͳ����״̬���� */
                        (const char*    )"CALCULATER",                          /* �������� */
                        (uint16_t       )TASK_VALVE_CALCULATER_SIZE,            /* ����ջ��С */
                        (void*          )NULL,                                  /* ������ں������� */
                        (UBaseType_t    )TASK_VALVE_CALCULATER_PRIORITY,        /* ��������ȼ� */
                        (TaskHandle_t*  )&Task_Valve_Calculater_Handle);        /* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
        printf("��foryon��-> ���� CALCULATER-����ɹ�!\r\n");

        /* ���� Sys_Run_ST ���� */
    xReturn = xTaskCreate((TaskFunction_t )vTaskRunTimeStats,                   /* ϵͳ����״̬���� */
                        (const char*    )"RUN_ST",                              /* �������� */
                        (uint16_t       )TASK_SYSTEM_RUN_STATE_SIZE,            /* ����ջ��С */
                        (void*          )NULL,	                                /* ������ں������� */
                        (UBaseType_t    )TASK_SYSTEM_RUN_STATE_PRIORITY,        /* ��������ȼ� */
                        (TaskHandle_t*  )&Task_System_Run_State_Handle);        /* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
        printf("��foryon��-> ���� RUN_ST-----����ɹ�!\r\n\r\n");
    
    //vTaskDelete(AppTaskCreate_Handle);    //ɾ��AppTaskCreate����
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();                //�˳��ٽ���
}





