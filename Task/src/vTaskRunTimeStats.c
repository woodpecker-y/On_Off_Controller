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
�������� �� vTaskRunTimeStats
��    �� �� Ӧ������3   ϵͳ����״̬����
��    �� �� pvParameters --- ��ѡ����
�� �� ֵ �� ��
��    �� �� yujh_empty@foxmail.com
*************************************************/

void vTaskRunTimeStats(void *pvParameters)
{
    uint8_t CPU_RunInfo[400];		            //������������ʱ����Ϣ
//    portTickType xLastExecutionTime;
//	xLastExecutionTime = xTaskGetTickCount();
    printf("%s",foryon);
    vTaskSuspend( NULL );//��������
    
    while (1)
    {
        if(g_run_params.auto_test_flag != 1)
        {
            memset(CPU_RunInfo,0,400);				//��Ϣ����������

            vTaskList((char *)&CPU_RunInfo);        //��ȡ��������ʱ����Ϣ
            
            printf("***********************************************************************\r\n");
            printf("������\t      ����״̬\t���ȼ�\t   ʣ��ջ\t�������\r\n");
            printf("***********************************************************************\r\n");
            printf("%s", CPU_RunInfo);


            memset(CPU_RunInfo,0,400);				//��Ϣ����������
            vTaskGetRunTimeStats((char *)&CPU_RunInfo);
            printf("***********************************************************************\r\n");
            printf("������\t         ���м���\t           ʹ����\r\n");
            printf("***********************************************************************\r\n");
            printf("%s", CPU_RunInfo);
            printf("-----------------------------------------------------------------------\r\n\r\n");
        }
        else
        {
            //�Զ�������
            auto_test();
        }
        
        vTaskSuspend( NULL );//��������
       //vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)5000 );   /* ��ʱ500��tick */		
    }
    
    //while(1);//��������ִ�е�����
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
    
    printf("��foryon��->               ....................................................      \r\n");
    printf("��foryon��->            ..........................................................   \r\n");
    printf("��foryon��->         ............... ��ʼ�Զ������ԣ����ע���´�ӡ��Ϣ ............... \r\n");
    printf("��foryon��->            ..........................................................   \r\n");
    printf("��foryon��->               ....................................................      \r\n\r\n");
    
    //Һ��������
#if LCD_SWITCH//                                    ��������Ŀ��1��  Һ�������ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��1�� Һ�������ԡ�---------------------------r\n");//��Ŀ1
    printf("��foryon��-> Һ���� ��ʼ���� ");
    disp_clear();//�����ʾ
    disp_poll();//ȫ����ʾ

    for(loop=0; loop<=9; loop++)//ѭ����ʾ
    {
        sprintf(disp_num, "%d%d%d%d%d%d%d%d",loop,loop,loop,loop,loop,loop,loop,loop);
        disp_str(1, 8, disp_num);
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)500 );
        printf(".");
    }
    disp_clear();//�����ʾ
    printf("\r\n");
    printf("��foryon��-> Һ����������ɣ����¼�Ƿ��й��ϣ�\r\n\r\n");
#endif
    
    
    
    
    //LED����                                       ��������Ŀ��2��  LED�Ʋ��ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��2�� LED�Ʋ��ԡ�---------------------------r\n");//��Ŀ2
    printf("��foryon��-> LED�� ��ʼ���� ");
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
    printf("��foryon��-> LED�Ʋ�����ɣ����¼�Ƿ��й��ϣ�\r\n\r\n");

    
    
    
    
    //�������                                      ��������Ŀ��3��   ������ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��3��  ������ԡ�---------------------------r\n");//��Ŀ3
    printf("��foryon��-> ���ŵ�� ��ʼ���� ... ...\r\n");
    int valve_timer = 0;
    
    g_sys_params.ValveCtrlDemandFlg = VALVE_OPEN;
    valve_triggered();
    printf("��foryon��-> �������ڴ�");
    while(1)
    {
        valve_timer++;
        if(valve_timer > 5)
        {
            printf("\r\n");
            printf("��Error ��-> ���Ŵ��쳣 ����� ����оƬ ������߹رյ���ĵ�λ�����û�нӺá��� X X X \r\n\r\n");
            valve_error_flag = 1;
           break;
        }
        if(motor_get_state() == MOTOR_STATE_OPENED)
        {
            printf("\r\n");
            printf("��foryon��-> �����Ѵ�\r\n");
            break;
        }
        
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 ); 
        printf(".");
    }
    
    valve_timer = 0;
    g_sys_params.ValveCtrlDemandFlg = VALVE_CLOSE;
    valve_triggered();
    printf("��foryon��-> �������ڹر�");
    while(1)
    {
        valve_timer++;
        if(valve_timer > 5)
        {
            printf("\r\n");
            printf("��Error ��-> ���Źر��쳣������� ����оƬ ������߹رյ���ĵ�λ�����û�нӺá�! X X X \r\n\r\n");
            valve_error_flag = 1;
            break;
        }
        if(motor_get_state() == MOTOR_STATE_CLOSED)
        {
            printf("\r\n");
            printf("��foryon��-> �����ѹر�\r\n");
            break;
        }
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
        printf(".");
    }
    if(valve_error_flag != 1)
        printf("��foryon��-> ���ŵ��������������� ... ...\r\n\r\n");

    
    
    
#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
    //����ˮ�¶Ȳ���                                 ��������Ŀ��4��   ����ˮ�¶Ȳ��ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��4��  ����ˮ�¶Ȳ��ԡ�---------------------r\n");//��Ŀ4
    printf("��foryon��-> ����ˮ�¶ȿ�ʼ���� ... ...\r\n");
#if LCD_SWITCH
    disp_clear();//�����ʾ
    disp_poll();//ȫ����ʾ
#endif
    
    //��ˮ
    printf("��foryon��-> ��ˮ�¶��ǣ�%d.%d��\r\n", g_run_params.Input_Temp/10, g_run_params.Input_Temp%10);

#if LCD_SWITCH
    disp_temp_inlet(g_run_params.Input_Temp);
#endif
    
    if(g_run_params.Input_Temp<0 && g_run_params.Input_Temp*(-1) >= 1000){
        printf("��Error ��-> ��ˮ�¶��쳣 ��������û�к��Ӻã�����̽ͷ���������������! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    else if(g_run_params.Input_Temp > 1000){
        printf("��Error ��-> ��ˮ�¶��쳣 ��������û�к��Ӻã�����̽ͷ���������������! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    
    //��ˮ
    printf("��foryon��-> ��ˮ�¶��ǣ�%d.%d��\r\n", g_run_params.Output_Temp/10, g_run_params.Output_Temp%10);

#if LCD_SWITCH
    disp_temp_return(g_run_params.Output_Temp);
#endif
    
    if(g_run_params.Output_Temp<0 && g_run_params.Output_Temp*(-1) >= 1000){
        printf("��Error ��-> ��ˮ�¶��쳣!��������û�к��Ӻã�����̽ͷ���������������! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    else if(g_run_params.Output_Temp > 1000){
        printf("��Error ��-> ��ˮ�¶��쳣!��������û�к��Ӻã�����̽ͷ���������������! X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    
    //�²�
    printf("��foryon��-> �²� �¶��ǣ�%d.%d��\r\n", (g_run_params.Input_Temp-g_run_params.Output_Temp)/10, (g_run_params.Input_Temp-g_run_params.Output_Temp)%10);
    //�����ж�g_run_params.DevErrState �н���ˮ�¶ȱ�־�д�����ֱ����ʾ --.- ��
    
#if LCD_SWITCH
    disp_temp_difference(g_run_params.Input_Temp-g_run_params.Output_Temp, (((g_run_params.DevErrState>>5)&0x03) != 0) ? 1:0);
#endif
    
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );	
    if(abs(g_run_params.Input_Temp-g_run_params.Output_Temp)>10){
        printf("��Error ��-> ����ˮ�¶Ȳ��ԣ��²�ϴ�X X X \r\n\r\n");
        temp_error_flag = 1;
    }
    if(temp_error_flag != 1)
        printf("��foryon��-> ����ˮ�¶�������������� ... ...\r\n\r\n");
#endif    //#if IN_OUT_TEMP_SWITCH             //����ˮ�¶ȿ���
    
    
    

    //FLASH����                                     ��������Ŀ��5��   �洢оƬ���ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��5��  �洢оƬ���ԡ�-----------------------r\n");//��Ŀ5
    printf("��foryon��-> �洢�� ��ʼ���� ... ...\r\n");
    AutoTest.test_0 = '0';
    AutoTest.test_1 = 11;
    AutoTest.test_2 = -22;
    AutoTest.test_3 = 33;
    AutoTest.test_4 = -44;
    AutoTest.test_5 = 55;
    AutoTest.test_6 = -66;
    AutoTest.test_7 = 77;
    //��������
    rv = fs_write(AUTO_TEST_LOG, 0, (u8*)&AutoTest, sizeof(AutoTestStruct));
    if (rv != E_FS_OK){
        printf("��Error ��-> д�����ݴ洢�쳣�����洢оƬ �쳣�������º��� FLASH��! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    memset(&AutoTest,   0x00, sizeof(AutoTestStruct));
    rv = fs_read(AUTO_TEST_LOG, 0, (u8*)&AutoTest, sizeof(AutoTest));
    if (rv != E_FS_OK){
        printf("��Error ��-> ��ȡ���ݴ洢�쳣�����洢оƬ �쳣�������º��� FLASH��! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    if(AutoTest.test_0!='0' || AutoTest.test_1!=11  || AutoTest.test_2!=-22 || \
       AutoTest.test_3!=33  || AutoTest.test_4!=-44 || AutoTest.test_5!=55  || \
       AutoTest.test_6!=-66 || AutoTest.test_7!=77)
    {
        printf("��Error ��-> �Աȴ洢�����쳣�����洢оƬ �쳣������� FLASH��! X X X \r\n\r\n");
        flash_error_flag = 1;
    }
    if(flash_error_flag != 1)
        printf("��foryon��-> �洢��������������� ... ...\r\n\r\n");

    
    
    
    
    //�ϵ翪������

    
    
    

    //MBUS����                                      ��������Ŀ��6��   MBUS���ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��6��  MBUS���ԡ�---------------------------r\n");//��Ŀ6
    printf("��foryon��-> MBUS���� ��ʼ���� ... ...\r\n");
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
                printf("��Error ��-> MBUS���� �������ݶԱ��쳣���쳣��X X X \r\n\r\n");
                mbus_error_flag = 1;
           }
           else
           {//MBUS���ݶԱ�����
               com_recv_init(COM3);
               break;
           }
           
        }
        
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    if(Mbus_timer>=5)
    {
        printf("��Error ��-> MBUS���� ���ճ�ʱ �쳣��X X X \r\n\r\n");
        mbus_error_flag = 1;
    }
    if(mbus_error_flag != 1)
        printf("��foryon��-> MBUS����������������� ... ...\r\n\r\n");

    
    
    

    //RF����                                        ��������Ŀ��7��  ���߲��ԡ�
    printf("��foryon��-> ---------------------------��������Ŀ��7�� ���߲��ԡ�----------------------------r\n");//��Ŀ7
    printf("��foryon��-> RF���� ��ʼ���� ... ...\r\n");
    u8 rf_timer = 0;
    u8* rf_rcv_ptr;
    u8 rf_rcv_len;
    
    rv = rf_init(RF_FRE_420_5M, RF_DATA_RATE_1200, TXPOW_20dBm, 70);//�����ʼ��Ϊ420.5M  ���Է��ų���ҲҪ����Ϊ���Ƶ�β���ͨѶ�ɹ�
    if (rv != E_RF_OK)
    {
        printf("��Error ��-> ����ģ�� ���÷��� �쳣��X X X \r\n\r\n");
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
                printf("��Error ��-> RF���� �������ݶԱ��쳣�� �����¼������ģ�飡X X X \r\n\r\n");
                rf_error_flag = 1;
           }
           else
           {//RF�������ݶԱ�����
               rf_rcv_init();
               break;
           }

        }
        vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    }
    if(rf_timer>=5)
    {
        printf("��Error ��-> RF���� ���ճ�ʱ�� �����¼������ģ�飡X X X \r\n\r\n");
        rf_error_flag = 1;
    }
    if(rf_error_flag != 1)
        printf("��foryon��-> ����������������� ... ...\r\n\r\n");
    rf433_init();   //���߲�����ɣ����»ָ�֮ǰ����

    
    

    

    //RTC����
    {   //                                              ��������Ŀ��8��   ʱ�Ӳ��ԡ�
        printf("��foryon��-> ---------------------------��������Ŀ��8��  ʱ�Ӳ��ԡ�----------------------------r\n");//��Ŀ8
        printf("��foryon��-> ʵʱʱ�� ��ʼ���� ... ...\r\n");
        if( (g_run_params.DevErrState>>7) == 1)
        {
            printf("��Error ��-> ʵʱʱ�� �쳣��������������ԣ�X X X \r\n\r\n");
            rtc_error_flag = 1;
        }
        if(rtc_error_flag != 1)
            printf("��foryon��-> ʱ��������������� ... ...\r\n\r\n");
    }

    //�������
    vTaskDelayUntil( (TickType_t*)&xLastExecutionTime, (TickType_t)1000 );
    printf("\r\n \r\n \r\n \r\n");
//    printf("��foryon��->         ..............................................................\r\n");
//    printf("��foryon��->             ................... ���ɲ��Խ�� .....................     \r\n");
//    printf("��foryon��->         ..............................................................\r\n\r\n");
    
    printf("��foryon��->               ....................................................      \r\n");
    printf("��foryon��->            ..........................................................   \r\n");
    printf("��foryon��->         ......................... ���ɲ��Խ�� .......................... \r\n");
    printf("��foryon��->            ..........................................................   \r\n");
    printf("��foryon��->               ....................................................      \r\n\r\n");
    
    if(valve_error_flag==1 || temp_error_flag==1 || flash_error_flag==1 ||\
        mbus_error_flag==1 || rf_error_flag==1   || rtc_error_flag==1)
    {
        printf("���ܽ�����-> ");
        if(valve_error_flag == 1)
        {
            printf("�������(3)��");
        }
        if(temp_error_flag == 1)
        {
            printf("����ˮ�¶ȹ���(4)��");
        }
        if(flash_error_flag == 1)
        {
            printf("�洢оƬ����(5)��");
        }
        if(mbus_error_flag == 1)
        {
            printf("MBUS���߹���(6)��");
        }
        if(rf_error_flag == 1)
        {
            printf("���߹���(7)��");
        }
        if(rtc_error_flag == 1)
        {
            printf("ʱ�ӹ���(8)");
        } 
        printf("\r\n");        
    }
    else
    {
        printf("��foryon��-> ȫ��Ӳ��������ȫ������������װ���� ... ...\r\n");   
    }

    g_run_params.auto_test_flag = 0;//�ر��Զ������ԣ���֮ǰ����һЩ���Զ������Գ�ͻ����
    
}

 

 

