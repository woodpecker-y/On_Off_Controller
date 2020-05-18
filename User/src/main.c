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
 
/* ������ͷ�ļ� --------------------------------------------------------------*/

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
    
	//adf_init();//��Ҫ��ʹ����ʱ��
    
    //PVD��ʼ��
    pvd_init();
    //PVD
    if(pvd_get_state() == 1)//���͵�ѹʱ
    {
        while(1)
        {
            disp_clear();//�����ʾ
            if(pvd_get_state() == 0)//���ߵ�ѹ
                break;
        }
    }
    
    /*! \brief ������־�ȼ�  �� L_NONE = 0, �ر� ���� L_ERROR = 1, ���� ���� L_WARN  = 2, ���� ��\
                            �� L_TRACE = 3, �켣���� L_INFO  = 4, ��Ϣ ���� L_DEBUG = 5, ���� ���� L_ALL   = 6, ȫ�� �� */
    MYLOG_LEVEL(L_DEBUG);
    
    // ������������ �� ��ʼ��
	com_load_config(com_cfg, sizeof(com_cfg)/sizeof(com_cfg[0]));
	com_init(COM1, 115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,   USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	com_init(COM3, 2400,   USART_WordLength_9b, USART_StopBits_1, USART_Parity_Even, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
    
    // RTC(ʵʱʱ��) �������� �� ��ʼ��
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
    MYLOG_DEBUG("��foryon��-> ��1��System time:%02d-%02d-%02d %02d:%02d:%02d------------------------�̡̡�!\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    MYLOG_DEBUG("��foryon��-> ��2��Debugging serial port load successfully----------------�̡̡�!\r\n");
    MYLOG_DEBUG("��foryon��-> ��3��The communication serial port is loaded successfully---�̡̡�!\r\n");
    MYLOG_DEBUG("��foryon��-> ��4��The real-time clock was loaded successfully------------�̡̡�!\r\n");
    
    // LED �������� �� ��ʼ��
    led_load_config(led_config, sizeof(led_config)/sizeof(led_config[0]));
    led_init();
    MYLOG_DEBUG("��foryon��-> ��5��System running LED state is loaded successfully--------�̡̡�!\r\n");
    
    // ADC �������� �� ��ʼ��
#if IN_OUT_TEMP_SWITCH
    adc_load_config(adc_config, 2);
    MYLOG_DEBUG("��foryon��-> ��6��Temperature probe loaded successfully------------------�̡̡�!\r\n");
#endif  //#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
    
    // �ⲿ�ж� �������� �� ��ʼ��
	exti_load_config(exti_config, sizeof(exti_config)/sizeof(exti_config[0]));
	exti_init();
    MYLOG_DEBUG("��foryon��-> ��7��The power off valve opened and loaded successfully-----�̡̡�!\r\n");
    
    // �ⲿFLASH �������� �� ��ʼ��
	flash_ext_load_config(&flash_config);
	flash_ext_init();
    //printf("Flash Type:%06x\r\n", flash_ext_read_id());
    MYLOG_DEBUG("��foryon��-> ��8��External flash [Flash Type:%06x] loaded successfully-�̡̡�!\r\n", flash_ext_read_id());
    
    // �ļ�ϵͳ���� �� ����
	//flash_ext_load_config(&flash_config);
	//flash_ext_init();
    fs_mount();
	rv = fs_load(E_FILE_TYPE_SYSINF, TEST_FILE_SYSINF, TEST_FILE_SYSINF_SIZE, sizeof(SysParams));
	if (rv != E_FS_OK)
	{
		MYLOG_DEBUG("��foryon��-> ��9��File system load failed--------------------------------XXX!\r\n");
	}
    else
    {
        MYLOG_DEBUG("��foryon��-> ��9��File system loaded successfully------------------------�̡̡�!\r\n");
    }
    rv = fs_load(E_FILE_TYPE_SYSINF, AUTO_TEST_LOG, AUTO_TEST_LOG_SIZE, sizeof(AutoTestStruct));
	if (rv != E_FS_OK)
	{
		while(1)
        {
            MYLOG_DEBUG("��foryon��-> �����Զ���������ʧ�� ... ... ------------------------�̡̡�!\r\n");
        }
	}
    // LCD �������� �� ��ʼ��
#if LCD_SWITCH
	lcd_load_config(&lcd_config);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
	lcd_init();
    BSP_GPIO_Init(PORTB, PIN6, GPIO_Mode_IN_FLOATING);//IRQ
    disp_poll();
    disp_signal_state(E_DISPLAY_OFF);//������ʾ�ر�
    
    MYLOG_DEBUG("��foryon��-> ��10��The LCD screen loaded successfully--------------------�̡̡�!\r\n");
#endif
    
    // RF ��������
    rf_load_config(&rf_config);
    rf_power_on();
    
    // MOTOR(����) �������� �� ��ʼ��
    motor_load_config(&motor_config);
    motor_init();
    if(motor_get_state() == MOTOR_STATE_OPENED)
        motor_run(E_MOTOR_SWITCH_CLOSED);
    
    if(motor_get_state() == MOTOR_STATE_CLOSED)
        motor_run(E_MOTOR_SWITCH_OPENED);
    MYLOG_DEBUG("��foryon��-> ��11��Valve drive loaded successfully-----------------------�̡̡�!\r\n");
    MYLOG_DEBUG(" ******************************************************************************\r\n\r\n");
}

void software_init(void)
{
    u8 rv = 0;
    
    //���ϵͳ����
    memset(&g_sys_params,   0x00, sizeof(g_sys_params));
    memset(&g_run_params,   0x00, sizeof(g_run_params));
    
    rv = fs_read(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params, sizeof(g_sys_params));
    if (rv == E_FS_OK)
    {
        //�ж��Ƿ����
        if(g_sys_params.factory_flag != 1)
        {
            //����ʹ�� -- �豸��Ϣ
            //g_sys_params.Device_SN           = 0x19996001;
            //g_sys_params.Device_GN           = 0x6001;
            
            //��������
            sys_factory_reset();
        }
        //д���ڴ��е�ʱ��
        rtc_write(g_sys_params.sys_t);
        
        //����Ƿ��ڹ�ů��
        g_run_params.heating_timer = UNIT_DAY-2;           //��ů�ڼ����ʱ����������������������Ƿ��ڹ�ů��
    }
    
    /*! \brief ������־�ȼ�  �� L_NONE = 0, �ر� ���� L_ERROR = 1, ���� ���� L_WARN  = 2, ���� ��\
                            �� L_TRACE = 3, �켣���� L_INFO  = 4, ��Ϣ ���� L_DEBUG = 5, ���� ���� L_ALL   = 6, ȫ�� �� */
    if(g_sys_params.DebugFlag == 0)
        MYLOG_LEVEL(L_NONE);
    else
        MYLOG_LEVEL(L_DEBUG);
}

#if DEBUG_SWITCH
void system_print(void)
{
    //��ʾ�汾��
    disp_sys_version(g_sys_params.Version_H, g_sys_params.Version_S);

    //�豸����
    MYLOG_DEBUG(" ****************************** �豸���� **************************************\r\n");
    MYLOG_DEBUG("��foryon��-> ��������־���� Factory     = %d\r\n",    g_sys_params.factory_flag);
    MYLOG_DEBUG("��foryon��-> ���豸��š��� Device_SN     = %08x\r\n",  g_sys_params.Device_SN);
    MYLOG_DEBUG("��foryon��-> �������š��� Device_GN     = %02x\r\n",  g_sys_params.Device_GN);
    MYLOG_DEBUG("��foryon��-> ����----�š��� Device_DESN   = %d\r\n",    g_sys_params.Device_DESN);
    MYLOG_DEBUG("��foryon��-> ��Ӳ���汾���� Version_H     = %d\r\n",    g_sys_params.Version_H);
    MYLOG_DEBUG("��foryon��-> ������汾���� Version_S     = %d\r\n",    g_sys_params.Version_S);
    MYLOG_DEBUG("��foryon��-> ���豸���͡��� DeviceType    = %d\r\n\r\n",g_sys_params.DeviceType);
    
    MYLOG_DEBUG(" **************************** ������̯���� ************************************\r\n");
    MYLOG_DEBUG("��foryon��-> ������------��Ϣ�� ��%d¥ %d��Ԫ ��%d�� %d0%d��  ���%d.%dƽ��\r\n", 
                                                                        g_sys_params.Buld, \
                                                                        g_sys_params.Unit, \
                                                                        g_sys_params.Floor, \
                                                                        g_sys_params.Floor, \
                                                                        g_sys_params.Cell, \
                                                                        g_sys_params.CellArea/100, \
                                                                        g_sys_params.CellArea%100);
    
    //%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    MYLOG_DEBUG("��foryon��-> ����ů�ڿ�ʼʱ�䡿 ��%02d-%02d-%02d %02d:%02d:%02d    ����%d\r\n",
                                                                        g_sys_params.Start_Time.Year,\
                                                                        g_sys_params.Start_Time.Month,\
                                                                        g_sys_params.Start_Time.Day,\
                                                                        g_sys_params.Start_Time.Hour,\
                                                                        g_sys_params.Start_Time.Minute,\
                                                                        g_sys_params.Start_Time.Second,\
                                                                        g_sys_params.Start_Time.Week);

    MYLOG_DEBUG("��foryon��-> ����ů�ڽ���ʱ�䡿 ��%02d-%02d-%02d %02d:%02d:%02d    ����%d\r\n",
                                                                        g_sys_params.Over_Time.Year,\
                                                                        g_sys_params.Over_Time.Month,\
                                                                        g_sys_params.Over_Time.Day,\
                                                                        g_sys_params.Over_Time.Hour,\
                                                                        g_sys_params.Over_Time.Minute,\
                                                                        g_sys_params.Over_Time.Second,\
                                                                        g_sys_params.Over_Time.Week);

    MYLOG_DEBUG("��foryon��-> ������-��¼-ʱ�䡿 ��%02d-%02d-%02d %02d:%02d:%02d    ����%d\r\n\r\n",
                                                                        g_sys_params.RecordTime.Year,\
                                                                        g_sys_params.RecordTime.Month,\
                                                                        g_sys_params.RecordTime.Day,\
                                                                        g_sys_params.RecordTime.Hour,\
                                                                        g_sys_params.RecordTime.Minute,\
                                                                        g_sys_params.RecordTime.Second,\
                                                                        g_sys_params.RecordTime.Week);
    
    MYLOG_DEBUG(" ****************************** ϵͳ��¼���� **********************************\r\n");
    MYLOG_DEBUG("��foryon��-> ���ѿ�----���͡��� ControlType           = 0x%02X\r\n",      g_sys_params.ControlType);
    MYLOG_DEBUG("��foryon��-> ������----���ơ��� ValveCtrlDemandFlg    = 0x%02X\r\n\r\n",  g_sys_params.ValveCtrlDemandFlg);
    
    MYLOG_DEBUG("��foryon��-> ��ǿ�ƿ������͡��� ForceCtrl             = 0x%02X\r\n",      g_sys_params.ForceCtrl);
    MYLOG_DEBUG("��foryon��-> ���¶��������͡��� TempCtrlType          = 0x%02X\r\n",      g_sys_params.TempCtrlType);
    MYLOG_DEBUG("��foryon��-> ���¶�--����ֵ���� TempSetValue          = %d\r\n",          g_sys_params.TempSetValue);
    MYLOG_DEBUG("��foryon��-> �������¶����͡��� LimtTE_Flag           = 0x%02X\r\n",      g_sys_params.LimtTE_Flag);
    MYLOG_DEBUG("��foryon��-> ������--�¶�ֵ���� LimtTE_Value          = %d\r\n\r\n",      g_sys_params.LimtTE_Value);
    
    MYLOG_DEBUG("��foryon��-> ���ۼƿ���ʱ�䡿�� Cycle_OpTim           = %d(��)\r\n",      g_sys_params.Cycle_OpTim);
    MYLOG_DEBUG("��foryon��-> ����̯----�������� Apportion_Energy      = %u(kwh)\r\n",     g_sys_params.Apportion_Energy);
    MYLOG_DEBUG("��foryon��-> ����ů----ʱ�䡿�� loop_time_59          = %d(��)\r\n",      g_sys_params.loop_time_59);
    MYLOG_DEBUG("��foryon��-> ��ʣ��----�������� Remain_Heat           = %d(kwh)\r\n",     g_sys_params.Remain_Heat);
    MYLOG_DEBUG("��foryon��-> ������������������ Buy_Total_Heat        = %d(kwh)\r\n\r\n", g_sys_params.Buy_Total_Heat);
    
    MYLOG_DEBUG("��foryon��-> ��У׼-�¶�-�㡿�� Offect_Temp           = %d\r\n",          g_sys_params.Offect_Temp);
    MYLOG_DEBUG("��foryon��-> ������----б�ʡ��� RakeRatioOffect_L     = %u\r\n",          g_sys_params.RakeRatioOffect_L);
    MYLOG_DEBUG("��foryon��-> ������----б�ʡ��� RakeRatioOffect_H     = %d\r\n",          g_sys_params.RakeRatioOffect_H);
    MYLOG_DEBUG("��foryon��-> ������¶�ƫ��� Offect_Input          = %d\r\n",          g_sys_params.Offect_Input);
    MYLOG_DEBUG("��foryon��-> �������¶�ƫ��� Offect_Output         = %d\r\n",          g_sys_params.Offect_Output);
    MYLOG_DEBUG(" *****************************************************************************\r\n\r\n");
}
#endif

int main(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    // Ӳ����ʼ��
    hardware_init();
    // �����ʼ��
    software_init();
    // ϵͳ���
#if DEBUG_SWITCH
    system_print();
#endif
    
    //���Ź���ʼ��
    wdg_init(5);
    /* ������ʱ����ʼ��	*/
	BASIC_TIM_Init();
    
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
    
    /* ����������ǰ��Ϊ�˷�ֹ��ʼ��STM32����ʱ���жϷ������ִ�У������ֹȫ���ж�(����NMI��HardFault)��
    �������ĺô��ǣ�
        1. ��ִֹ�е��жϷ����������FreeRTOS��API������
        2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
        3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
    ����ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�ͨ��ָ��cpsie i������__set_PRIMASK(1)
    ��cpsie i�ǵ�Ч�ġ�*/
	__set_PRIMASK(1); //�����ֹȫ���ж�(����NMI��HardFault)������ֲ�ļ�port.c�еĺ���prvStartFirstTask�л����¿���ȫ���жϡ�
    
    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,             /* �����������ں��� */
                         (const char*     )"AppTaskCreate",           /* ������������� */
                         (uint16_t        )64,                        /* ���������ջ��С */
                         (void*           )NULL,                      /* �����������ں������� */
                         (UBaseType_t     )1,                         /* ������������ȼ� */
                         (TaskHandle_t*   )&AppTaskCreate_Handle);    /* ��������Ŀ��ƿ�ָ�� */ 
    
    /* ����������� */
    if(pdPASS == xReturn)
        vTaskStartScheduler();   /* �������񣬿������� */
    else
        return -1;
    
    while(1);   /* ��������ִ�е����� */  
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
