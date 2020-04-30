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
函数名称 ： vTask_Com_Rf_Valve
功    能 ： 应用任务2 数据处理（串口数据、无线数据）
参    数 ： pvParameters --- 可选参数
返 回 值 ： 无
作    者 ： yujh_empty@foxmail.com
*************************************************/
void vTask_Com_Rf_Valve(void *pvParameters)
{
    portTickType xLastExecutionTime;
    xLastExecutionTime = xTaskGetTickCount();

    //清空接收端 数据结构数据
    memset(&s_com1_handler, 0x00, sizeof(s_com1_handler));
    memset(&s_com3_handler, 0x00, sizeof(s_com3_handler));
    memset(&s_rf_handler,   0x00, sizeof(s_rf_handler));
    
    rf433_init();

    for(;;)
    {
        //数据接收处理
        if(g_run_params.auto_test_flag != 1)
            com_recv_data_deal();
        
        //阀门动作
        valve_deal();
        
        //等待延时
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)50 );
    }
    
    //while(1);//正常不会执行到这里
}


void rf433_init(void)
{
    u8      rv = 0;
    u8      hundred     = 0;
    u8      decade      = 0;
    u8      unit        = 0;
    u8      channel     = 0;
    
    //设置模块通道
    unit    = (g_sys_params.Device_SN & 0x0000000F);
    decade  = ((g_sys_params.Device_SN & 0x000000F0)>>4)*10;
    hundred = ((g_sys_params.Device_SN & 0x00000F00)>>8)*100;
    channel = hundred + decade + unit;//通道号
    
    rv = rf_init((RF_FRE)((g_sys_params.Device_SN & 0x0000F000)>>12), RF_DATA_RATE_1200, TXPOW_20dBm, channel);//这里初始化为420.5M  所以阀门程序也要设置为这个频段才能通讯成功
    if (rv != E_RF_OK)
    {
        MYLOG_DEBUG("RF_ERROR_RV=%d\r\n",rv);
    }
    rf_rcv_init();
    
}


