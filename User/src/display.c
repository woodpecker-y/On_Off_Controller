#include "display.h"
#include "board.h"

/*
* ��ʾ����: ��ʾ���ڡ���ʾʱ�䡢��ʾ����¶ȡ���ʾ�����¶ȡ���ʾ�²��ͨ�ۼ�ʱ�䡢����״̬��ʾ����ͨ�ŵ�ַ��ͨ�ŵ�ַ�����ۼ�������

* ���߷��ţ��ź�ͨѶ���ϣ�����ص����ͷ��š������ŷ��š��ط��ŷ��š���ת��־��ϵͳ����ԲȦ��X���ţ�

* ���϶ȡ桢KW.h��d h

*/

static u8  disp_buffer[10] = {0};

/*! \brief   ˢ����ʾ*/
void disp_update(void)
{
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief   ���������ʾ*/
void disp_clear(void)
{
    u8 temp_H = 0, temp_L = 0;      //����ϵͳ��ʾ��־  ������ ���ء� ��ԲȦX�� ���·��� ���źš� �����ߡ� ��X�� "���"
    
    temp_H = disp_buffer[0] & 0x0F;     //������ ���ء� ��ԲȦX�� ���·���
    temp_L = disp_buffer[9] & 0xF0; //���źš� �����ߡ� ��X�� "���"
    
    memset(disp_buffer, 0, sizeof(disp_buffer));
    
    //����д��
    disp_buffer[0] = temp_H;
    disp_buffer[9] = temp_L;
    
    disp_update();
}

/*! \brief   ȫ��ʾ*/
void disp_full(void)
{
	unsigned char disp_buf[10] = {0};
		
    memset(disp_buf, 0xFF, sizeof(disp_buf));
	disp_update();
}

/*! \brief   ��ѯ��ʾ*/
void disp_poll(void)
{
	unsigned char i = 0, j = 0;
	unsigned char disp_buf[10] = {0};
		
	for (i=0; i<sizeof(disp_buf); ++i)
	{
        for(j=0; j<8; j++)
        {
            disp_buf[i] = (disp_buf[i] << 1) | 0x01;
            lcd_update_screen(disp_buf, sizeof(disp_buf));
            DELAY_MS(8);
        }
	}
    disp_clear();
}


/*! \brief 
*  ��ʾ����
* \param idx         -     �����λ��
* \param val         -     ����������� 0-21
* \param disp_sts    -     �򿪻��߹رմ˶�
* 
* \note  
* 
*/
void disp_digit(u8 idx, u8 val, u8 disp_sts)
{
    /*! \brief                     0,     1,    2,    3,    4,    5,    6,    7,    8,    9,    A,    B,   C,     D,    E,    F,    G     H     U     N    -,    '' */ 
    unsigned char tab_digits[] = {0xD7, 0x50, 0xB5, 0xF1, 0x72, 0xE3, 0xE7, 0x51, 0xF7, 0xF3, 0x77, 0xE6, 0xA4, 0xF4, 0xA7, 0x27, 0xC7, 0x76, 0xC4, 0x64, 0x20, 0x00};  
        
    if (disp_sts == E_DISPLAY_ON)
    {
        disp_buffer[idx] = disp_buffer[idx] & 0xF0;
        disp_buffer[idx-1] = disp_buffer[idx-1] & 0x8F;
        
        disp_buffer[idx] = disp_buffer[idx] | (tab_digits[val]>>4);
        disp_buffer[idx-1] = disp_buffer[idx-1] | (tab_digits[val]<<4);
        
        //printf("disp_buffer[%d] = %02X\r\n", idx, disp_buffer[idx]);
        //printf("disp_buffer[%d] = %02X\r\n", idx-1, disp_buffer[idx-1]);
    }
    else
    {
        disp_buffer[idx] = disp_buffer[idx] & 0xF0;
        disp_buffer[idx-1] = disp_buffer[idx-1] & 0x8F;
    }
}

/*! \brief 
*  ��ʾ��ֵ
* \param start         -     �������ʾ��ʼλ��
* \param end           -     �������ʾ����λ��
* \param val           -     u32������ֵ
* 
* \note  ����2-6 ��ʾ 12345 ���磺disp_number(2, 6, 12345)
* 
*/
void disp_number(u8 start, u8 end, u32 val)
{  
    unsigned long fator = 1;
    unsigned char i=0, j=0;
    unsigned char num[9] = {0};
    unsigned char val_num = 0;
    u32           val_temp = 0;
    u8            n = 0;
    
    // �������
    for (i=start; i<end+1; ++i)
    {
        disp_digit(i, 21, E_DISPLAY_ON);         
    }
    
    // �����������ֵ�Ǽ�λ
    val_temp = val;
    while(val_temp != 0)
    {
        val_num++;
        val_temp/=10;
    }
    
    //printf("val_num %d \r\n", val_num);
    
    // ����ÿһ��λ�ö�Ӧ�����ַŵ� num������
    if(end-start+1 > val_num)
        i = end-start+1 - val_num;
    else
        i=0;
    
    for (; i<end-start+1; ++i)
    {
        fator = 1;
        for (j=0; j<val_num-n-1; ++j)
        {
            fator *= 10;
        }
        
        n++;
        //printf("val = %d\r\n", val);
        num[i] = val / fator;
        val %= fator;
    }
    
    //printf("fator =%ld\r\n", fator);
    //MYLOG_DEBUG_HEXDUMP("num[i]:", num, 9);
    
    // �õ�i��ֵ
    for (i=0; i<end-start+1; ++i)
    {
        disp_digit(start+i, num[i], E_DISPLAY_ON); 
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer)); 
}

/*! \brief 
*  ��ʾ�ַ�
* \param start         -     ��ʾ��λ��
* \param ch            -     ��ʾ�ַ�
* 
* \note  ����2  ��ʾ C disp_char(2, C)
*  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, G, H, U, N, -, '' 
*/
void disp_char(u8 start, u8 ch)
{
    u8 val = 0;
        
    switch(ch)
    {
    case 'A':
    case 'a':        
        val = 10;
        break;        
    case 'B':
    case 'b':        
        val = 11;
        break;        
    case 'C':
    case 'c':        
        val = 12;
        break;        
    case 'D':
    case 'd':        
        val = 13;
        break;        
    case 'E':
    case 'e':        
        val = 14;
        break;  
    case 'F':
    case 'f':        
        val = 15;
        break;     
    case 'G':
    case 'g':        
        val = 16;
        break;   
    case 'H':
    case 'h':        
        val = 17;
        break;   
    case 'U':
    case 'u':        
        val = 18;
        break;  
    case 'N':
    case 'n':        
        val = 19;
        break;  
    case '-':
        val = 20;
        break;
    case ' ':
        val = 21;
        break;    
    default:
        break;
    }
//    DEBUG1("val = %d\r\n", val);
    disp_digit(start, val, E_DISPLAY_ON);
}


/*! \brief 
*  ��ʾ�ַ���
* \param start         -     �������ʾ��ʼλ��
* \param end           -     �������ʾ����λ��
* \param val           -     ��ʾ���ַ���
* 
* \note  ������ʾ abcdefgH disp_char(1, abcdefgH)
*  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, G, H, U, N, -, '' 
*/
void disp_str(u8 start, u8 end, const char *str)
{
    u8 i = 0;
    u8 len = 0;
    
    assert_param(str != NULL);
    assert_param(start<=8 && end<=8 && end>=start);
    
    len = strlen(str);
    if (len > (end-start+1))
        len = end-start+1;
    
    for (i=0; i<len; ++i)
    {
        if (str[i]>='0' && str[i]<='9')
        {
            disp_number(start+i, start+i, str[i]-'0');    
        }
        else
        {
            disp_char(start+i, str[i]);
        }
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer)); 
}

/*! \brief 
* ��ʾϵͳʱ�� ������
*/
void disp_systime_year_mon_day(void)
{
    struct tm t;
    
    disp_clear();
    rtc_read(&t);
    //MYLOG_DEBUG("��1��System time:%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    
    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_ON);   //ʱ���ʶ
    disp_number(1, 2, (t.tm_year+1900)%2000);//��
    disp_str(3, 3, "-");
    disp_number(4, 5, t.tm_mon+1);           //��
    disp_str(6, 6, "-");
    disp_number(7, 8, t.tm_mday);            //��
}

/*! \brief 
* ��ʾϵͳʱ�� ʱ����
*/
void disp_systime_hour_min_sec(void)
{
    struct tm t;
    
    disp_clear();
    rtc_read(&t);
    //MYLOG_DEBUG("��1��System time:%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_ON);   //ʱ���ʶ
    disp_number(1, 2, t.tm_hour);           //ʱ
    disp_str(3, 3, "-");
    disp_number(4, 5, t.tm_min);            //��
    disp_str(6, 6, "-");
    disp_number(7, 8, t.tm_sec);            //��
}

/*! \brief 
*  ��ʾ��ˮ�¶�
* \param temp_inlet[IN]     ��ˮ�¶�ֵ
* 
* \note   ��ֵΪ�Ŵ�10����ֵ
* 
*/
void disp_temp_inlet(long temp_inlet)
{    
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_INLET, E_DISPLAY_ON);       //��ڷ���
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);  //�����
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);        //С�������
    //DEBUG1("temp_inlet = %d\r\n", temp_inlet*(-1));
    if(temp_inlet < 0)
    {
        if (temp_inlet*(-1) >= 100)
        {
            if(temp_inlet*(-1) >= 1000)
            {
                disp_str(6, 8, "---");
            }
            else
            {
                disp_str(5, 5, "-");
                sprintf(buf, "%ld", temp_inlet*(-1));
                disp_str(6, 8, buf); 	
            }                
        }
        else
        {
            disp_str(6, 6, "-");
            sprintf(buf, "%02ld", temp_inlet*(-1));
            disp_str(7, 8, buf); 
        }
    }
    else if(temp_inlet>=0 && temp_inlet<1000)
    {
        if(temp_inlet<100)//1
        {
            disp_number(7, 8, temp_inlet);
        }
        else
        {
            disp_number(6, 8, temp_inlet);
        }
        
    }
    else
    {
        disp_str(6, 8, "---");
    }
    
}

/*! \brief 
* ��ʾ��ˮ�¶�
* \param temp_return[IN]     ��ˮ�¶�ֵ
* 
* \note  ��ֵΪ�Ŵ�10����ֵ
* 
*/
void disp_temp_return(long temp_return)
{    
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_RETURN, E_DISPLAY_ON);       //���ڷ���
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);  //�����
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);        //С�������
    //DEBUG1("temp_inlet = %d\r\n", temp_inlet*(-1));
    if(temp_return < 0)
    {
        if (temp_return*(-1) >= 100)
        {
            if(temp_return*(-1) >= 100)
            {
                disp_str(6, 8, "---");
            }
            else
            {
                disp_str(5, 5, "-");
                sprintf(buf, "%ld", temp_return*(-1));
                disp_str(6, 8, buf); 	
            }                
        }
        else
        {
            disp_str(6, 6, "-");
            sprintf(buf, "%02ld", temp_return*(-1));
            disp_str(7, 8, buf); 
        }
    }
    else if(temp_return>=0 && temp_return<1000)
    {
        if(temp_return<100)//1
        {
            disp_number(7, 8, temp_return);
        }
        else
        {
            disp_number(6, 8, temp_return);
        }
        
    }
    else
    {
        disp_str(6, 8, "---");
    }
}


/*! \brief 
* ��ʾ�²�
* \param temp_dif[IN]         ��ˮ�¶�ֵ-��ˮ�¶�ֵ 
* 
* \note  ��ֵΪ�Ŵ�10����ֵ
* 
*/
void disp_temp_difference(long temp_dif, u8 flag)
{
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_DIFFERENCE, E_DISPLAY_ON);      //�²����
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);      //�����
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);            //С�������
    //DEBUG1("temp_inlet = %d\r\n", temp_inlet*(-1));
    if(flag == 1)
    {
        disp_str(6, 8, "---");
        return;
    }
    
    if(temp_dif < 0)
    {
        if (temp_dif*(-1) >= 100)
        {
            if(temp_dif*(-1) >= 1000)
            {
                disp_str(6, 8, "---");
            }
            else
            {
                disp_str(5, 5, "-");
                sprintf(buf, "%ld", temp_dif*(-1));
                disp_str(6, 8, buf); 	
            }                
        }
        else
        {
            disp_str(6, 6, "-");
            sprintf(buf, "%02ld", temp_dif*(-1));
            disp_str(7, 8, buf); 
        }
    }
    else if(temp_dif>=0 && temp_dif<1000)
    {
        if(temp_dif<100)//1
        {
            disp_number(7, 8, temp_dif);
        }
        else
        {
            disp_number(6, 8, temp_dif);
        }
        
    }
    else
    {
        disp_str(6, 8, "---");
    }
}


/*! \brief 
* ��ʾ�����ۼ�ʱ��
* \param heat_quantity[IN]         - 
*/
void disp_connect_sum_time(u32 connect_time)
{
    u32 val_temp = 0;
    unsigned char val_num = 0;
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_CONNECT, E_DISPLAY_ON);      //��ͨ
    lcd_write_seg(disp_buffer, SEG_6DP_POINT, E_DISPLAY_ON);    //�����ڶ���С����
    lcd_write_seg(disp_buffer, SEG_HOUR, E_DISPLAY_ON);         //��ʱ������  �ۼƿ���ʱ��
    
    val_temp = connect_time;
    while(val_temp != 0)
    {
        val_num++;
        val_temp/=10;
    }
    
    switch(val_num)
    {
        case 1:
            disp_number(6, 8, connect_time);
            break;
        case 2:
            disp_number(6, 8, connect_time);
            break;
        case 3:
            disp_number(6, 8, connect_time);
            break;
        case 4:
            disp_number(5, 8, connect_time);
            break;
        case 5:
            disp_number(4, 8, connect_time);
            break;
        case 6:
            disp_number(3, 8, connect_time);
            break;
        case 7:
            disp_number(2, 8, connect_time);
            break;
        case 8:
            disp_number(1, 8, connect_time);
            break;
        default:
            disp_number(1, 8, connect_time);
            break;
    } 
    
}

/*! \brief 
* ��ʾϵͳ����״̬��־
* \param heat_quantity[IN]         - 
*/
void disp_sys_fault_flag(u8 code)
{
    if(code!=0)
        lcd_write_seg(disp_buffer, SEG_SYS_FAULT, E_DISPLAY_ON); //��ʾϵͳ���ϱ�ʶ
    else
        lcd_write_seg(disp_buffer, SEG_SYS_FAULT, E_DISPLAY_OFF); //���ϵͳ���ϱ�ʶ
}

/*! \brief 
* ��ʾϵͳ����״̬
* \param heat_quantity[IN]         - 
*/
void disp_sys_fault(u8 code)
{
    disp_clear();
    
    disp_sys_fault_flag(code);

    disp_number(1, 1, (code>>7 & 0x01));
    disp_number(2, 2, (code>>6 & 0x01));
    disp_number(3, 3, (code>>5 & 0x01));
    disp_number(4, 4, (code>>4 & 0x01));
    disp_number(5, 5, (code>>3 & 0x01));
    disp_number(6, 6, (code>>2 & 0x01));
    disp_number(7, 7, (code>>1 & 0x01));
    disp_number(8, 8, (code>>0 & 0x01));
    
    return;
}

/*! \brief 
* ��ʾ�豸���루ID��
* \param heat_quantity[IN]         - 
*/
void disp_id(const unsigned char *id, u8 len)
{
    char buf[13] = {0};
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_ON);//��ʾID
    
    strcpy(buf, (char*)id);
    disp_str(1, 8, buf);
}

/*! \brief 
* ��ʾͨѶ���루GN��
* \param heat_quantity[IN]         - 
*/
void disp_gn_id(const unsigned char *id, u8 len)
{
    char buf[13] = {0};
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_ON);//��ʾID
    
    disp_str(1, 3, "GN-");
    
    strcpy(buf, (char*)id);
    disp_str(5, 8, buf);
}

/*! \brief 
* ��ʾ������
* \param heat_quantity[IN]         - ����
* 
* \note  heat_quantity �����ǳ���100��
* 
*/
void disp_heat_quantity(u32 heat_quantity)
{
    unsigned long val_temp = 0;
    unsigned char val_num = 0;
    
    disp_clear();   
    lcd_write_seg(disp_buffer, SEG_HEAT_QUANTITY, E_DISPLAY_ON);    //��������
    lcd_write_seg(disp_buffer, SEG_KWH, E_DISPLAY_ON);              //KW.h
    lcd_write_seg(disp_buffer, SEG_6DP_POINT, E_DISPLAY_ON);        //�����ڶ���С����

    val_temp = heat_quantity;
    
    while(val_temp != 0)
    {
        val_num++;
        val_temp/=10;
    }
    
    switch(val_num)
    {
        case 1:
            disp_number(6, 8, heat_quantity);
            break;
        case 2:
            disp_number(6, 8, heat_quantity);
            break;
        case 3:
            disp_number(6, 8, heat_quantity);
            break;
        case 4:
            disp_number(5, 8, heat_quantity);
            break;
        case 5:
            disp_number(4, 8, heat_quantity);
            break;
        case 6:
            disp_number(3, 8, heat_quantity);
            break;
        case 7:
            disp_number(2, 8, heat_quantity);
            break;
        case 8:
            disp_number(1, 8, heat_quantity);
            break;
        default:
            disp_number(1, 8, heat_quantity);
            break;
    } 

}





/*! \brief 
*  ��ʾ����״̬
* \param start         -     ����״̬
* 
* \note
*/
void disp_valve_state(E_DispValveMode sts)
{
    if (sts == E_DISPLAY_VALVE_OPEN)        //����
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_ON);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_OFF); 
    }
    else if (sts == E_DISPLAY_VALVE_CLOSE)  //�ط�
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_ON); 
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_OFF);         
    }
    else if(sts == E_DISPLAY_VALVE_PLUG)    //�·�
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_OFF); 
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_ON); 
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}


/*! \brief 
*  ��ʾ�ź�
* \param start         -     ����״̬
* 
* \note
*/
void disp_signal_state(E_DispMode sts)
{
    
    lcd_write_seg(disp_buffer, SEG_ANT, E_DISPLAY_ON);
    
    if (sts == E_DISPLAY_ON)        //����
    {
        lcd_write_seg(disp_buffer, SEG_ANT_FULAT, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_SIGNAL, E_DISPLAY_ON);
    }
    else if (sts == E_DISPLAY_OFF)  //�ط�
    {
        lcd_write_seg(disp_buffer, SEG_ANT_FULAT, E_DISPLAY_ON);
        lcd_write_seg(disp_buffer, SEG_SIGNAL, E_DISPLAY_OFF);        
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief 
*  ��ʾ���
* \param start         -     ����״̬
* 
* \note
*/
void disp_battery_state(E_DispMode sts)
{
    
    lcd_write_seg(disp_buffer, SEG_BATTERY, sts);     
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief 
*  ��ʾ�汾��
* \param start         -     ����״̬
* 
* \note
*/
void disp_sys_version(u8 version_h, u8 version_s)
{
    
    disp_str(1, 2, "u-");
    disp_str(5, 6, "--");
    disp_number(3, 4, version_h);
    disp_number(7, 8, version_s);
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}


//u16 task_query_temp_by_resistance(u32 r)
//{
//    u16 temp = 0;

////    temp = (r - 1000000)*10.00/3862.1;
//	temp = (r - 1000000)*10.00/3850;
////		DEBUG1("temp:%u\r\n", temp); 
//    return temp;
//}

//u16 task_sample_read_temp(u8 idx)
//{
//    u32 r = 0;
//    u16 ad = 0;

//    ad = adc_read(&SingleChannelADCConfig[idx], ADC_AVG);
////		DEBUG1("ad:%u\r\n", ad);  
//    r = (u32)((10*ad+79852.5) / (120802.5-10*ad) * 1000.00 * 1000.00);
//		if(r <= 1000000)        //С��0�棬Ĭ��Ϊ0��
//    {
//      r = 1000000;
//    }
////	  DEBUG1("r:%u\r\n", r); 
////		r = (u32)(1000 * ( 1000 * ad )) / ( 4095 - ad );
////		if(ad > 4000)
////		{
////			r = 1000000;
////		}
////	DEBUG1("r:%u\r\n", r); 

//    return task_query_temp_by_resistance(r);
//}

//u8 temp_calibration(s32 *temp_comp_inlet, s32 *temp_comp_return)
//{
//    u16 temp_water_in   = task_sample_read_temp(E_ADC_TEMP_INLET) ;
//    u16 temp_water_out  = task_sample_read_temp(E_ADC_TEMP_RETURN);
//	
////    DEBUG1("temp_water_in:%u\r\n", temp_water_in); 
////		DEBUG1("temp_water_out:%u\r\n", temp_water_out); 
//	
//    *temp_comp_inlet  = (temp_water_in-394)*(-1);
//    *temp_comp_return = (temp_water_out-394)*(-1);
//	
////		DEBUG1("*temp_comp_inlet:%u\r\n", *temp_comp_inlet); 
////		DEBUG1("*temp_comp_return:%u\r\n", *temp_comp_return); 
//    
//    return 0;
//}







//    disp_clear();
//    disp_number(1, 8, 123);
//    disp_update();
//    DELAY_MS(2000);

//    disp_clear();
//    disp_number(4, 5, 1);
//    disp_update();
//    DELAY_MS(2000);
    
//    disp_clear();
//    disp_number(1,2, 12345678);
//    disp_update();
//    DELAY_MS(2000);
//   
//    disp_clear();
//    disp_number(1,8,12345678);
//    disp_update();
//    DELAY_MS(2000);

//    disp_clear();
//    disp_number(1,2,12);
//    disp_update();
//    DELAY_MS(2000);
//    
//    disp_clear();
//    disp_str(1,8,"abcdefgh");
//    disp_update();
//    DELAY_MS(2000);








