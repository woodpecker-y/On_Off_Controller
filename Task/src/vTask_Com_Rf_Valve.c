#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "bsp.h"
#include "app_task.h"
#include "board.h"
#include "display.h"
#include "dbg.h"
#include "sysparams.h"
#include "valve_deal.h"
#include "recv_data_deal.h"

/************************************************
�������� �� vTask_Com_Rf_Valve
��    �� �� Ӧ������2 ���ݴ����������ݡ��������ݣ�
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� yujh_empty@foxmail.com
*************************************************/
void vTask_Com_Rf_Valve(void *pvParameters)
{
    portTickType xLastExecutionTime;
    xLastExecutionTime = xTaskGetTickCount();

    //��ս��ն� ���ݽṹ����
    memset(&s_com1_handler, 0x00, sizeof(s_com1_handler));
    memset(&s_com3_handler, 0x00, sizeof(s_com3_handler));
    memset(&s_rf_handler,   0x00, sizeof(s_rf_handler));
    
    rf433_init();

    for(;;)
    {
        //���ݽ��մ���
        if(g_run_params.auto_test_flag != 1)
            com_recv_data_deal();
        
        //���Ŷ���
        valve_deal();
        
        //�ȴ���ʱ
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)50 );
    }
    
    //while(1);//��������ִ�е�����
}


void rf433_init(void)
{
    u8      rv = 0;
    u8      hundred     = 0;
    u8      decade      = 0;
    u8      unit        = 0;
    u8      channel     = 0;
    
    //����ģ��ͨ��
    unit    = (g_sys_params.Device_SN & 0x0000000F);
    decade  = ((g_sys_params.Device_SN & 0x000000F0)>>4)*10;
    hundred = ((g_sys_params.Device_SN & 0x00000F00)>>8)*100;
    channel = hundred + decade + unit;//ͨ����
    
    rv = rf_init((RF_FRE)((g_sys_params.Device_SN & 0x0000F000)>>12), RF_DATA_RATE_1200, TXPOW_20dBm, channel);//�����ʼ��Ϊ420.5M  ���Է��ų���ҲҪ����Ϊ���Ƶ�β���ͨѶ�ɹ�
    if (rv != E_RF_OK)
    {
        MYLOG_DEBUG("RF_ERROR_RV=%d\r\n",rv);
    }
    rf_rcv_init();
    
}


