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

//�ϵ���
void Power_detect(void);

//LCD��ѯ��ʾ
void lcd_disp_menu(void);

//�¶ȼ��
void collect_temp(void);

//LED���е���˸
void led_run_state(void);

//��ȡ�¶�
s16 read_temp(E_ADC_NUM_Idx adc_num_idx);

/************************************************
�������� �� vTask_Power_Display_Temp_Runled
��    �� �� Ӧ������3 Һ����ʾ����
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� yujh_empty@foxmail.com
*************************************************/
void vTask_Power_Display_Temp_Runled(void *pvParameters)
{
    portTickType xLastExecutionTime;
	xLastExecutionTime = xTaskGetTickCount();
    
    //����֮ǰ��������
    valve_triggered();
    
    for(;;)
    {
        //ι��
        wdg_reset();
        
        //�ϵ���
#if OUTAGE_POWER_SWITCH
        Power_detect();  
#endif
        
        //LCD��ѯ��ʾ
#if LCD_SWITCH
        if(g_run_params.auto_test_flag !=1 )
        lcd_disp_menu(); 
#endif
        
        //�¶ȼ��
#if IN_OUT_TEMP_SWITCH
        collect_temp();  
#endif  //#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
        
        //���е�״̬
        if(g_run_params.auto_test_led_flag != 1)
            led_run_state();
        
        //�ȴ���ʱ
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    //while(1);//��������ִ�е�����
}




#if OUTAGE_POWER_SWITCH

u8 powerdel_flag = 0;//ִ�й� "���翪������" ��־
//�ϵ���
void Power_detect(void)
{
    //������
    if(exti_read_state(0) == 0)//�ϵ�
    {
        if(g_run_params.PowerDownFlag == 0)
        {
            if(powerdel_flag == 0)
            {
                disp_battery_state(E_DISPLAY_ON);   //��ʾ����
                powerdel_flag = 1;
                valve_triggered();
            }
        }
        g_run_params.PowerDownFlag = 0;         //�ϵ�
    }
    else
    {
        if(g_run_params.PowerDownFlag == 1)
        {
            if(powerdel_flag == 1)
            {
                disp_battery_state(E_DISPLAY_OFF);  //��ʾ�ϵ�
                powerdel_flag = 0;
                soft_reset();//ϵͳ��λ
                //valve_triggered();
            }
        }
        g_run_params.PowerDownFlag = 1;     //�ϵ�
        
    }
    
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
}
#endif

//LCD ��ѯ��ʾ
void lcd_disp_menu(void)
{
    //��ʾ
    static u8  disp_num = 0;           //��ʾʱ�����
    static u8  disp_menu = 0;          //��ʾ�˵�
    u8  temp_array[9] = {0};    //��ʱ����
    
    /* ѭ��Һ����ʾ����ʾ����: 
        1.����״̬��ʾ����2.ͨ�ŵ�ַ��3.ͨ�ŵ�ַ��4.��ʾ���ڡ�5.��ʾʱ�䡢6.��ʾ����¶ȡ�
        7.��ʾ�����¶ȡ�8.��ʾ�²9.��ͨ�ۼ�ʱ�䡢10���ۼ����� 
    */
                        
    //����ʵʱ��ʾ
    if(g_run_params.rf_comm_flag == 0)
        disp_signal_state(E_DISPLAY_OFF);   //�ź���ʾ�ر�
    else
        disp_signal_state(E_DISPLAY_ON);    //�ź���ʾ����

    //��ʾϵͳ�����־
    disp_sys_fault_flag(g_run_params.DevErrState);
    
    //�˵���ѯ
    switch(disp_menu)
    {
        case 0://��������״̬
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
        case 1://��ʾ����sn���
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
        case 2://��ʾ����GN������
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
        case 3://��ʾ����
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
        case 4://��ʾʱ��
            disp_systime_hour_min_sec();
            if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
#if IN_OUT_TEMP_SWITCH
        case 5://��ʾ��ˮ�¶�
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
        case 6://��ʾ��ˮ�¶�
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

        case 7://��ʾ��ˮ�¶Ⱥͻ�ˮ�¶�֮��
            if(disp_num == 1)
            {
                //�����ж�g_run_params.DevErrState �н���ˮ�¶ȱ�־�д�����ֱ����ʾ --.- ��
                disp_temp_difference(g_run_params.Input_Temp-g_run_params.Output_Temp, (((g_run_params.DevErrState>>5)&0x03) != 0) ? 1:0);
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
#else
        case 5://������ʾ��ˮ�¶�
            disp_num = 0;
            disp_menu++;
            disp_menu++;
            disp_menu++;
        break;
#endif
        case 8://��ʾ���ſ����ۼ�ʱ��
            if(disp_num == 1)
            {
                //disp_connect_sum_time(i);
                
                disp_connect_sum_time(g_sys_params.Cycle_OpTim/(u32)36);//��ʾ��λ����ΪСʱ����ʾ��λС����
            }
            else if(disp_num>=LCD_DISP_DELAY)
            {
                disp_num = 0;
                disp_menu++;
            }
            break;
        case 9://��ʾ��̯����
            if(disp_num == 1)
            {
                //�û���ʹ���������߷�̯����(KWH) ��2λС��
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

#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
//�¶ȼ��
void collect_temp(void)
{
    //����¶�
    static u8 collect_temp_num = 0;
    
    //�¶ȼ��
    collect_temp_num++;
    if(collect_temp_num >= 5)
    {
        collect_temp_num = 0;
        
        g_run_params.Input_Temp  = read_temp(E_ADC_TEMP_INLET)  + g_sys_params.Offect_Input;
        g_run_params.Output_Temp = read_temp(E_ADC_TEMP_RETURN) + g_sys_params.Offect_Output;
        
        //��ˮ�¶ȴ�����
        if(g_run_params.Output_Temp>1000 || g_run_params.Output_Temp < -100)
            error_set(SYS_ERROR_OUTPUT_TEMP);
        else
            error_clr(SYS_ERROR_OUTPUT_TEMP);
        
        //��ˮ�¶ȴ�����
        if(g_run_params.Input_Temp>1000 || g_run_params.Input_Temp < -100)
            error_set(SYS_ERROR_INPUT_TEMP);
        else
            error_clr(SYS_ERROR_INPUT_TEMP);

    }
}
#endif      //#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���



//���е���˸
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


#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
//����һ��¶�
u16 query_temp_by_resistance(u32 r)
{
    u16 temp = 0;

    //temp = (r - 1000000)*10.00/3862.1;
	temp = (r - 1000000)*10.00/3850;

    return temp;
}


//��ȡ�¶�
s16 read_temp(E_ADC_NUM_Idx adc_num_idx)
{
    u32 adc_temp = 0;
    u32 r = 0;
    u16 read_temp = 0;
    
    adc_init(adc_num_idx);
    adc_temp = adc_sample(adc_num_idx);
    adc_close(adc_num_idx);
    
    r = (u32)((10*adc_temp+79852.5) / (120802.5-10*adc_temp) * 1000.00 * 1000.00);
    if(r <= 1000000)        //С��0�棬Ĭ��Ϊ0��
        r = 1000000;
    
//    MYLOG_DEBUG("\r\n R = %u\r\n ", r);
//    MYLOG_DEBUG("\r\n adc_temp = %u\r\n ", adc_temp);

    read_temp = query_temp_by_resistance(r);

    return read_temp;
}
#endif  //#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���

