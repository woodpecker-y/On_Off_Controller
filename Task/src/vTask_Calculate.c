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

//��⹩ů��
u8 heating_period(void);

//RTC��ϵͳʱ�� �Ա� ʱ��
void Check_RTC(void);

/************************************************
�������� �� vAppTask1
��    �� �� Ӧ������2 LED��˸��ϵͳ����״̬�ƣ�����˼���¶�����
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� yujh_empty@foxmail.com
*************************************************/
void vTask_Calculate(void *pvParameters)
{
    
    portTickType xLastExecutionTime;
    xLastExecutionTime = xTaskGetTickCount();
    u32 rf433_init_timer = 0;
	struct tm t;
    
    //�Զ�RTC��ϵͳʱ�� �Ա� ʱ��
    rtc_read(&t);
    g_run_params.Rtc_check_sec = t.tm_hour*60*60+t.tm_min*60+t.tm_sec;
    
    for(;;)
    {
//�¿���ʧ���Զ�����
#if RF_LOSS_OPEN_VALVE_SWITCH
        {
            //���߼�ʱ  ����60����û�к��¿�����������ʾʧ��
            g_run_params.rf_comm_timer++;

            //һ��Сʱû�к��¿��������������ʾʧ�����������û�����ģʽ���Զ��򿪣�������ʾʧ��
            if(g_run_params.rf_comm_timer >= UNIT_HOUR)//  UNIT_HOUR
            {
                g_run_params.rf_comm_timer = 0;   //���ź��¿���ʧ����ʱ
                g_run_params.rf_comm_flag = 0;  //�¿���ʧ����־
                
                //�ط�����  ǿ�ƿ���  �ڹ�ů��
                if(g_sys_params.ControlType != 0xAA && g_sys_params.ForceCtrl == 0xFF && g_run_params.WorkFlg == 0xAA)//δ���ÿط�����,����ǿ�ƴ򿪻���ǿ�ƹر�����²������Զ���
                {
                    valve_triggered();
                    g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                }
            }
        }
#endif
        //ÿ�� ��⹩ů��
        {
            g_run_params.heating_timer++;
            if(g_run_params.heating_timer >= UNIT_DAY)//һ����һ�ι�ů��
            {
                g_run_params.heating_timer = 0;
                if(heating_period() == 0)
                    g_run_params.WorkFlg = 0xAA;//�����Ƿ��ڹ�ů����־ //��ů����־��¼  0XAA �ڹ�ů��   0X00 ���ڹ�ů��
                else
                    g_run_params.WorkFlg = 0x00;//�����Ƿ��ڹ�ů����־ //��ů����־��¼  0XAA �ڹ�ů��   0X00 ���ڹ�ů��
                
                //ÿ����һ���Ƿ�Ƿ�ѣ����Ƿ����رշ���
                if(g_sys_params.ControlType == 0xAA)
                {
                    valve_triggered();
                    g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                }
                
                //��ȡRTCʵʱʱ��
                rtc_read(&g_sys_params.sys_t);
                //��������
                save_params(SYS_SAVE_RECORD_PARAMS);//��Ҫ��¼�ۼƿ���ʱ��
            }
        }
        
        //�ڹ�ů�� 10���Ӽ�ʱ���� �ۼƿ���ʱ��
        {
            if(g_run_params.WorkFlg == 0xAA)
            {
                if(g_run_params.ValveState == VALVE_OPEN)
                {
                    //��ʱ��10���Ӵ洢һ���ۼƿ���ʱ��
                    g_run_params.save_cycle_optim_timer++;
                    if(g_run_params.save_cycle_optim_timer >= 10*UNIT_MIN)
                    {
                        g_run_params.save_cycle_optim_timer = 0;
                        
                        //��ȡRTCʵʱʱ��
                        rtc_read(&g_sys_params.sys_t);
                        //��������
                        save_params(SYS_SAVE_RECORD_PARAMS);//��Ҫ��¼�ۼƿ���ʱ��
                    }
                    //�ۼƿ���ʱ�� �Լ�1��
                    g_sys_params.Cycle_OpTim++;
                }
            }
        }
        
//һ�� ��ת����
#if ONE_WEEK_SWITCH
        {
            g_run_params.one_week_turn_timer++;
            if(g_run_params.one_week_turn_timer >= ONE_WEEK_TURN_TIME)
            {
                //�� g_run_params.one_week_turn_timer == 7*UNIT_DAY��ִ��һ��
                if(g_run_params.one_week_turn_timer == ONE_WEEK_TURN_TIME)
                {
                    if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)
                        g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                    else
                        g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                    valve_triggered();
                }
                
                //�ٴλָ�֮ǰ����״̬
                if(g_run_params.one_week_turn_timer >= ONE_WEEK_TURN_TIME+18)
                {
                    g_run_params.one_week_turn_timer = 0;
                    if(g_sys_params.ValveCtrlDemandFlg == VALVE_OPEN)
                        g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
                    else
                        g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
                    valve_triggered();
                }
            }
        }
#endif
        //һСʱ��ʱ
        {
            //�Զ�RTC��ϵͳʱ�� �Ա� ʱ��
            if(g_run_params.heating_timer < UNIT_HOUR)
            {
                //���ݹ�ů�ڶ�ʱ����ʵ�ַ�������һСʱǰ��5����һ��RTC����
                g_run_params.Check_time = 5;
                g_run_params.Deviation = 1;//5��֮�ڵ����
            }
            else
            {
                g_run_params.Check_time = 10*UNIT_MIN;
                g_run_params.Deviation = 3;//10����֮�ڵ����
            }
            
            if(g_run_params.Rtc_check_timer >= g_run_params.Check_time)
            {
                g_run_params.Rtc_check_timer = 0; 
                Check_RTC();
            }
            g_run_params.Rtc_check_timer++;
        }
        
        //�Զ���ʼ������ÿ35���ӳ�ʼ��һ������ģ��
        {
            rf433_init_timer++;
            if(rf433_init_timer >= 35*UNIT_MIN)
            {
                rf433_init_timer = 0;
                rf433_init();//��ʼ������
            }
        }
        
        //�ȴ���ʱ
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );//1000 ��λms
    }
    
    //while(1);//��������ִ�е�����
}

//RTC��ϵͳʱ�� �Ա� ʱ��
void Check_RTC(void)
{
    int check_rtc_sec = 0;
    int Deviation = 0;

    //��ȡRTCʱ��
    rtc_read(&g_sys_params.sys_t);
    
    //RTC��ϵͳʱ�� �Ա� RTC
    if(g_sys_params.sys_t.tm_hour < 23)  //��ֹ��24���м�⣬���¼���ʱ����������ȷ
    {
        //�����ܹ�����
        check_rtc_sec = (int)( (u32)(g_sys_params.sys_t.tm_hour*60*60+g_sys_params.sys_t.tm_min*60+g_sys_params.sys_t.tm_sec) - (u32)g_run_params.Rtc_check_sec );
        
        //�������
        Deviation = abs(check_rtc_sec) - g_run_params.Check_time;
        
        if( abs(Deviation) > g_run_params.Deviation )//����� g_run_params.Deviation �뷶Χ��
            error_set(SYS_ERROR_RTC);
        else
            error_clr(SYS_ERROR_RTC);
    }

    g_run_params.Rtc_check_sec = g_sys_params.sys_t.tm_hour*60*60+g_sys_params.sys_t.tm_min*60+g_sys_params.sys_t.tm_sec;
    
    //��������
    save_params(SYS_SAVE_RECORD_PARAMS);//��Ҫ��¼�ۼƿ���ʱ��
    
}

//��ѯ�Ƿ��ڹ�ů��
//return 0 ��
//return 1 ����
u8 heating_period(void)
{
    u8 rv = 0;
    u16 cur = 0, st = 0, et = 0;
    struct tm t;
    
    rtc_read(&t);
    
    cur = (t.tm_mon+1)*100 + t.tm_mday;
    
    st  = g_sys_params.Start_Time.Month*100 + g_sys_params.Start_Time.Day;
    et  = g_sys_params.Over_Time.Month*100 + g_sys_params.Over_Time.Day;
    
    //printf("Cur:%d\r\n St:%d\r\n Et:%d\r\n", cur, st, et);
    //�ж��Ƿ��ڹ�ů���ڣ������Ƿ����ڹ�ů�����
    if((cur>=st || cur<=et) && (g_sys_params.Start_Time.Year == t.tm_year+1900 || g_sys_params.Over_Time.Year == t.tm_year+1900))
    {
        rv = 0;
        //printf("#### �ڹ�ů��000 ####\r\n");
    }
    else
    {
        rv = 1;
        //printf("#### ���ڹ�ů��111 ####\r\n");
    }
    return rv;
}


