#include "display.h"
#include "board.h"

/*
* 显示内容: 显示日期、显示时间、显示入口温度、显示出口温度、显示温差、接通累计时间、错误状态表示符、通信地址、通信地址、总累计热量、

* 天线符号（信号通讯故障）、电池电量低符号、开阀门符号、关阀门符号、堵转标志、系统错误圆圈加X符号，

* 摄氏度℃、KW.h、d h

*/

static u8  disp_buffer[10] = {0};

/*! \brief   刷新显示*/
void disp_update(void)
{
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief   清除所有显示*/
void disp_clear(void)
{
    u8 temp_H = 0, temp_L = 0;      //保存系统显示标志  “开” “关” “圆圈X” “堵阀” “信号” “天线” “X” "电池"
    
    temp_H = disp_buffer[0] & 0x0F;     //“开” “关” “圆圈X” “堵阀”
    temp_L = disp_buffer[9] & 0xF0; //“信号” “天线” “X” "电池"
    
    memset(disp_buffer, 0, sizeof(disp_buffer));
    
    //重新写入
    disp_buffer[0] = temp_H;
    disp_buffer[9] = temp_L;
    
    disp_update();
}

/*! \brief   全显示*/
void disp_full(void)
{
	unsigned char disp_buf[10] = {0};
		
    memset(disp_buf, 0xFF, sizeof(disp_buf));
	disp_update();
}

/*! \brief   轮询显示*/
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
*  显示数字
* \param idx         -     数码管位置
* \param val         -     此数码管数字 0-21
* \param disp_sts    -     打开或者关闭此段
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
*  显示数值
* \param start         -     数码管显示开始位置
* \param end           -     数码管显示结束位置
* \param val           -     u32类型数值
* 
* \note  比如2-6 显示 12345 例如：disp_number(2, 6, 12345)
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
    
    // 清空数码
    for (i=start; i<end+1; ++i)
    {
        disp_digit(i, 21, E_DISPLAY_ON);         
    }
    
    // 计算输入的数值是几位
    val_temp = val;
    while(val_temp != 0)
    {
        val_num++;
        val_temp/=10;
    }
    
    //printf("val_num %d \r\n", val_num);
    
    // 计算每一个位置对应的数字放到 num数组中
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
    
    // 得到i的值
    for (i=0; i<end-start+1; ++i)
    {
        disp_digit(start+i, num[i], E_DISPLAY_ON); 
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer)); 
}

/*! \brief 
*  显示字符
* \param start         -     显示的位置
* \param ch            -     显示字符
* 
* \note  比如2  显示 C disp_char(2, C)
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
*  显示字符串
* \param start         -     数码管显示开始位置
* \param end           -     数码管显示结束位置
* \param val           -     显示的字符串
* 
* \note  比如显示 abcdefgH disp_char(1, abcdefgH)
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
* 显示系统时间 年月日
*/
void disp_systime_year_mon_day(void)
{
    struct tm t;
    
    disp_clear();
    rtc_read(&t);
    //MYLOG_DEBUG("【1】System time:%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    
    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_ON);   //时间标识
    disp_number(1, 2, (t.tm_year+1900)%2000);//年
    disp_str(3, 3, "-");
    disp_number(4, 5, t.tm_mon+1);           //月
    disp_str(6, 6, "-");
    disp_number(7, 8, t.tm_mday);            //日
}

/*! \brief 
* 显示系统时间 时分秒
*/
void disp_systime_hour_min_sec(void)
{
    struct tm t;
    
    disp_clear();
    rtc_read(&t);
    //MYLOG_DEBUG("【1】System time:%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

    lcd_write_seg(disp_buffer, SEG_TIME, E_DISPLAY_ON);   //时间标识
    disp_number(1, 2, t.tm_hour);           //时
    disp_str(3, 3, "-");
    disp_number(4, 5, t.tm_min);            //分
    disp_str(6, 6, "-");
    disp_number(7, 8, t.tm_sec);            //秒
}

/*! \brief 
*  显示进水温度
* \param temp_inlet[IN]     进水温度值
* 
* \note   此值为放大10倍数值
* 
*/
void disp_temp_inlet(long temp_inlet)
{    
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_INLET, E_DISPLAY_ON);       //入口符号
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);  //℃符号
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);        //小数点符号
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
* 显示出水温度
* \param temp_return[IN]     出水温度值
* 
* \note  此值为放大10倍数值
* 
*/
void disp_temp_return(long temp_return)
{    
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_RETURN, E_DISPLAY_ON);       //出口符号
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);  //℃符号
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);        //小数点符号
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
* 显示温差
* \param temp_dif[IN]         进水温度值-出水温度值 
* 
* \note  此值为放大10倍数值
* 
*/
void disp_temp_difference(long temp_dif, u8 flag)
{
    char buf[13] = {0};

    disp_clear();
    
    lcd_write_seg(disp_buffer, SEG_TEMP_DIFFERENCE, E_DISPLAY_ON);      //温差符号
    lcd_write_seg(disp_buffer, SEG_DEGREES_CELSIUS, E_DISPLAY_ON);      //℃符号
    lcd_write_seg(disp_buffer, SEG_7DP_POINT, E_DISPLAY_ON);            //小数点符号
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
* 显示开阀累计时间
* \param heat_quantity[IN]         - 
*/
void disp_connect_sum_time(u32 connect_time)
{
    u32 val_temp = 0;
    unsigned char val_num = 0;
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_CONNECT, E_DISPLAY_ON);      //接通
    lcd_write_seg(disp_buffer, SEG_6DP_POINT, E_DISPLAY_ON);    //倒数第二个小数点
    lcd_write_seg(disp_buffer, SEG_HOUR, E_DISPLAY_ON);         //“时”符号  累计开阀时间
    
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
* 显示系统错误状态标志
* \param heat_quantity[IN]         - 
*/
void disp_sys_fault_flag(u8 code)
{
    if(code!=0)
        lcd_write_seg(disp_buffer, SEG_SYS_FAULT, E_DISPLAY_ON); //显示系统故障标识
    else
        lcd_write_seg(disp_buffer, SEG_SYS_FAULT, E_DISPLAY_OFF); //清除系统故障标识
}

/*! \brief 
* 显示系统错误状态
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
* 显示设备编码（ID）
* \param heat_quantity[IN]         - 
*/
void disp_id(const unsigned char *id, u8 len)
{
    char buf[13] = {0};
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_ON);//显示ID
    
    strcpy(buf, (char*)id);
    disp_str(1, 8, buf);
}

/*! \brief 
* 显示通讯编码（GN）
* \param heat_quantity[IN]         - 
*/
void disp_gn_id(const unsigned char *id, u8 len)
{
    char buf[13] = {0};
    
    disp_clear();
    lcd_write_seg(disp_buffer, SEG_COMM_ID, E_DISPLAY_ON);//显示ID
    
    disp_str(1, 3, "GN-");
    
    strcpy(buf, (char*)id);
    disp_str(5, 8, buf);
}

/*! \brief 
* 显示总热量
* \param heat_quantity[IN]         - 热量
* 
* \note  heat_quantity 热量是乘以100的
* 
*/
void disp_heat_quantity(u32 heat_quantity)
{
    unsigned long val_temp = 0;
    unsigned char val_num = 0;
    
    disp_clear();   
    lcd_write_seg(disp_buffer, SEG_HEAT_QUANTITY, E_DISPLAY_ON);    //“热量”
    lcd_write_seg(disp_buffer, SEG_KWH, E_DISPLAY_ON);              //KW.h
    lcd_write_seg(disp_buffer, SEG_6DP_POINT, E_DISPLAY_ON);        //倒数第二个小数点

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
*  显示阀门状态
* \param start         -     阀门状态
* 
* \note
*/
void disp_valve_state(E_DispValveMode sts)
{
    if (sts == E_DISPLAY_VALVE_OPEN)        //开阀
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_ON);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_OFF); 
    }
    else if (sts == E_DISPLAY_VALVE_CLOSE)  //关阀
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_ON); 
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_OFF);         
    }
    else if(sts == E_DISPLAY_VALVE_PLUG)    //堵阀
    {
        lcd_write_seg(disp_buffer, SEG_VALVE_OPEN, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_VALVE_CLOSE, E_DISPLAY_OFF); 
        lcd_write_seg(disp_buffer, SEG_VALVE_STOPE, E_DISPLAY_ON); 
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}


/*! \brief 
*  显示信号
* \param start         -     阀门状态
* 
* \note
*/
void disp_signal_state(E_DispMode sts)
{
    
    lcd_write_seg(disp_buffer, SEG_ANT, E_DISPLAY_ON);
    
    if (sts == E_DISPLAY_ON)        //开阀
    {
        lcd_write_seg(disp_buffer, SEG_ANT_FULAT, E_DISPLAY_OFF);
        lcd_write_seg(disp_buffer, SEG_SIGNAL, E_DISPLAY_ON);
    }
    else if (sts == E_DISPLAY_OFF)  //关阀
    {
        lcd_write_seg(disp_buffer, SEG_ANT_FULAT, E_DISPLAY_ON);
        lcd_write_seg(disp_buffer, SEG_SIGNAL, E_DISPLAY_OFF);        
    }
    
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief 
*  显示电池
* \param start         -     阀门状态
* 
* \note
*/
void disp_battery_state(E_DispMode sts)
{
    
    lcd_write_seg(disp_buffer, SEG_BATTERY, sts);     
    lcd_update_screen(disp_buffer, sizeof(disp_buffer));
}

/*! \brief 
*  显示版本号
* \param start         -     阀门状态
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
//		if(r <= 1000000)        //小于0℃，默认为0℃
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








