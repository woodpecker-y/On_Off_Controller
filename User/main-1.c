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
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
#include "adf.h"
#include "com.h"
#include "rfid.h"
#include "printf.h"
#include "rtc.h"
#include "dbg.h"
#include "led.h"
#include "beep.h"
#include "exti.h"
#include "key.h"
#include "systimer.h"
#include "flash.h"
#include "bsp_flash.h"
#include "board.h"
#include "bsp.h"
#include "adc.h"
#include "rf.h"
#include "si4432.h"
#include "motor.h"
#include "board.h"
#include "ba6289.h"

//#include "board_type_oncentrated_charge_device.h"

///* Private function prototypes -----------------------------------------------*/
//#ifdef __GNUC__
///* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//   set to 'Yes') calls __io_putchar() */
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

#define TEST_COM				1
#define TEST_RFID				0
#define TEST_RTC				0
#define TEST_LED				0
#define TEST_BEEP				0
#define TEST_KEY				0
#define TEST_EXTI				0
#define TEST_SYSTIMER			0
#define TEST_TIMER_BLOCK		0
#define TEST_FLASH				0
#define TEST_FLASH_EXT			0
#define TEST_LCD				0
#define TEST_FILESYSTEM			0
#define	TEST_FILESYSTEM_SYSINF	0
#define	TEST_FILESYSTEM_LOG		0
#define TEST_DEVMGR				0
#define TEST_RF                 1
#define TEST_PT1000             0
#define TEST_HT1621             0
#define TEST_MOTOR              0

#define TEST_ADDR			FLASH_ADDR_START+0x400*63


#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define MAX_SAMPLE_CNT 30


#if	TEST_FLASH 	|| TEST_FLASH_EXT
//u8 test_data[5120] = {0};
//u8 read_data[5120] = {0};
u8 test_data[4] = {0};
u8 read_data[4] = {0};
#endif

typedef struct _test
{
	u32 b;
	u16 size;
	u8  data;
	char s[5];
}Test;

#if TEST_FILESYSTEM
typedef struct _SysParams{
	u32			records_wr;
	u32			records_rd;
	u8			passwd_ori[16];
	u8			dev_addr[7];			
}SysParams;

typedef struct _RechargeLog{
	u8  dev_addr[7];					// éè±?μ???
	u8	room_id[3];						// ????o?
    u8  datetime[7];                    // 3??μê±??
    u16 recharge_cnt;                   // 3??μDòo?
    u32 recharge_heat_total;            // à???3??μèèá?
    u32 recharge_heat;                  // 3??μèèá?
}RechargeLog;

SysParams	g_sys_params1, g_sys_params2;
RechargeLog	g_recharge_log1, g_recharge_log2;

#define TEST_FILE_SYSINF				(0x0000) 
#define TEST_FILE_SYSINF_SIZE			(0x2000/sizeof(SysParams)*sizeof(SysParams))
#define TEST_FILE_RECHARGE_LOG			(0x2000)
#define TEST_FILE_RECHARGE_LOG_SIZE	   	(0x2000/sizeof(RechargeLog)*sizeof(RechargeLog))

#endif


#if TEST_PT1000 

static __IO u16 dma_adc_value1 = 0;

int BSP_ADC_Init1(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* 使能ADC1外设时钟 */

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;      // 定义DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&dma_adc_value1;     // 定义DMA内存基地址
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // 外设作为数据传输的来源（外设传到内存） DMA_DIR_PeripheralDST――内存传到外设
    DMA_InitStructure.DMA_BufferSize = 1;               // 连续转化8个AD值
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 外设寄存器地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;            // 内存寄存器地址不递增, 连续采集需要设置成递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据宽度32bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     // 内存数据宽度32bit
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMA模式是循环
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // DMA优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;    // DMA没有设置内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ADC1 ADC2 在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // ENABLE-ADC多通道扫描, DISABLE-ADC单通道扫描
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // ENABLE--ADC连续转化模式 DISABLE--ADC单次转化模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 由软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 数据向右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // 连续转化1个AD通道值
    ADC_Init(ADC1, &ADC_InitStructure);

//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
    ADC_Cmd(ADC1, ENABLE);

    // Enable ADC1 reset calibaration register(重置ADC1校准寄存器)
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register(等待ADC1校准重置完成)
    while(ADC_GetResetCalibrationStatus(ADC1)) ;
    // Start ADC1 calibaration(开始ADC1校准)
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration(等待ADC1校准完成)
    while(ADC_GetCalibrationStatus(ADC1)) ;

    return 0;
}

u32 BSP_ADC_SampleOnce1(void)
{
    int i = 0;
    u32 adc_value = 0;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    for (i=0; i<MAX_SAMPLE_CNT; ++i)
    {
        /* wait until end-of-covertion */
        while( !ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) ) ;
        adc_value += dma_adc_value1;
    }

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);

    //printf("[3]idx:%d, s_adc_num:%d\r\n", idx, s_adc_num);

    return adc_value/MAX_SAMPLE_CNT;
}

u16 task_query_temp_by_resistance(u32 r)
{
    u16 temp = 0;

    //temp = (r - 1000000)*10.00/3862.1;
	temp = (r - 1000000)*10.00/3850;

    return temp;
}
#endif

u16 task_query_temp_by_resistance(u32 r)
{
    u16 temp = 0;

    //temp = (r - 1000000)*10.00/3862.1;
	temp = (r - 1000000)*10.00/3850;

    return temp;
}

#if TEST_HT1621

#define HT1621_CS_CLR()			BSP_GPIO_SetLow(PORTA, PIN15)
#define HT1621_CS_SET()         BSP_GPIO_SetHigh(PORTA, PIN15)

#define HT1621_RD_CLR()			BSP_GPIO_SetLow(PORTB, PIN3)
#define HT1621_RD_SET()			BSP_GPIO_SetHigh(PORTB, PIN3)

#define HT1621_WR_CLR()			BSP_GPIO_SetLow(PORTB, PIN4)
#define HT1621_WR_SET()			BSP_GPIO_SetHigh(PORTB, PIN4)

#define HT1621_DATA_CLR()		BSP_GPIO_SetLow(PORTB, PIN5)	
#define HT1621_DATA_SET()		BSP_GPIO_SetHigh(PORTB, PIN5)

#define HT1621_TYPE_CMD		0x04		// 命令类型
#define HT1621_TYPE_DATA	0x05		// 数据类型

#define HT1621_TYPE_LEN		3			// 类型位长

#define HT1621_BITLEN_CMD	8			// 命令位长
#define HT1621_BITLEN_ADDR	6			// 地址位长

#define HT1621_BIAS_1_3		0x28		// 1/3bias 4com
#define HT1621_RC256K		0x18		// 使用系统内部RC震荡器 
#define HT1621_XTAL32K		0x14		// 使用系统晶振		
#define HT1621_SYSDIS		0x00		// 关闭系统震荡及LCD偏振发生器
#define HT1621_WDTDIS		0x05		// 禁止系统看门狗
#define HT1621_SYSEN		0x01		// 打开系统震荡及LCD偏振发生器
#define HT1621_LCDON		0x03		// 打开系统震荡 
#define HT1621_LCDOFF       0x02        // 关闭系统震荡

unsigned char HT1621BitRev(unsigned char dat)
{
    unsigned char i, val=0;
    
    for (i=0; i<8; ++i)
    {
        if (dat&0x80)
        {
            val |= (1<<i);
        }
        else
        {
            val |= 0;
        }
        
        dat <<= 1;
    }
    
    return val;
}

void HT1621Enable(void)
{
    HT1621_CS_CLR();
    HT1621_WR_SET();
    //asm("nop");
    __nop();
    delay_ms(1);
    
    HT1621_WR_CLR();
    //asm("nop");
    __nop();
    delay_ms(1);
}

void HT1621Reset(void)
{
    HT1621_WR_CLR();
    HT1621_WR_SET();
    //asm("nop");
    delay_ms(1);
    __nop();
    HT1621_CS_SET();
}

void HT1621Write(unsigned int bit_data, unsigned char bit_len)
{
    unsigned char i   = 0;
    unsigned int mask = 0;
    
    mask = 1<<(bit_len-1);
    for (i=0; i<bit_len; ++i)
    {
        HT1621_WR_CLR();
        //((bit_data&mask) != 0) ? HT1621_DATA_SET() : HT1621_DATA_CLR();
        if ((bit_data&mask) != 0)
        {
            HT1621_DATA_SET();    
        }
        else
        {
            HT1621_DATA_CLR();    
        }
        
        HT1621_WR_SET();
        
        bit_data <<= 1;
    }
}



void HT1621WriteCmd(unsigned int cmd)
{
    HT1621Enable();
    
    HT1621Write(HT1621_TYPE_CMD, HT1621_TYPE_LEN);
    //delay_us(25);
    delay_ms(1);
    HT1621Write(cmd, HT1621_BITLEN_CMD);
    
    HT1621Reset();
}



void HT1621WriteData(unsigned char addr, const unsigned char *data, unsigned char len)
{
    unsigned char i = 0;
    
    HT1621Enable();
    
    HT1621Write(HT1621_TYPE_DATA, HT1621_TYPE_LEN);
    //delay_us(25);
    delay_ms(1);
    HT1621Write(addr, HT1621_BITLEN_ADDR);
    //delay_us(25);
    delay_ms(1);
    
    for (i=0; i<len; ++i)
    {
        HT1621Write(HT1621BitRev(data[i]), 8);	
    }
    
    HT1621Reset();
}

    /*! \brief HT1621初始化 */
void HT1621Init(void)
{

//    HT1621_CS_DIROUT();
//    HT1621_RD_DIROUT();
//    HT1621_WR_DIROUT();
//    HT1621_DATA_DIROUT();
    
    BSP_GPIO_Init(PORTB, PIN3, GPIO_Mode_Out_PP);//RD
    BSP_GPIO_Init(PORTB, PIN4, GPIO_Mode_Out_PP);//WR
    BSP_GPIO_Init(PORTB, PIN5, GPIO_Mode_Out_PP);//DATA
    BSP_GPIO_Init(PORTA, PIN15, GPIO_Mode_Out_PP);//CS
    
    HT1621_CS_SET();
    HT1621_RD_SET();
    HT1621_WR_SET();
    HT1621_DATA_SET();
    
    HT1621_CS_SET();
    HT1621_WR_SET();
    HT1621_DATA_SET();
      

    
    HT1621WriteCmd(HT1621_BIAS_1_3);
    HT1621WriteCmd(HT1621_RC256K);
	//HT1621WriteCmd(HT1621_XTAL32K);
    HT1621WriteCmd(HT1621_SYSDIS);
    HT1621WriteCmd(HT1621_WDTDIS);
    HT1621WriteCmd(HT1621_SYSEN);
    HT1621WriteCmd(HT1621_LCDON);	
}

void HT1621FillFull(void)
{
	unsigned char i = 0;
	unsigned char disp_buf[19] = {0};
		
	for (i=0; i<sizeof(disp_buf); ++i)
	{
		disp_buf[i] = 0xff;	
	}

	HT1621WriteData(0x00, disp_buf, sizeof(disp_buf));	
}

void HT1621FillZero(void)
{
	unsigned char i = 0;
	unsigned char disp_buf[19] = {0};
		
	for (i=0; i<sizeof(disp_buf); ++i)
	{
		disp_buf[i] = 0x00;	
	}

	HT1621WriteData(0x00, disp_buf, sizeof(disp_buf));	
}
#endif

void HT1621FillFull(void)
{
	unsigned char i = 0;
	unsigned char disp_buf[19] = {0};
		
	for (i=0; i<sizeof(disp_buf); ++i)
	{
		disp_buf[i] = 0xff;	
	}

	lcd_update_screen(disp_buf, sizeof(disp_buf));	
}

void HT1621FillZero(void)
{
	unsigned char i = 0;
	unsigned char disp_buf[19] = {0};
		
	for (i=0; i<sizeof(disp_buf); ++i)
	{
		disp_buf[i] = 0x00;	
	}

	lcd_update_screen(disp_buf, sizeof(disp_buf));	
}

int main(void)
{
	u32 i = 0, j = 0, z = 0;
	Test vable;
	Test read;
	u32  rv = 0;
	uint8_t *rcv_buf = NULL;
	u16 rcv_len = 0;

    u8 *rcv_dat = NULL;
    u8 rcv_rflen = 0;
    
    u32 temp1 = 0;
    u32 temp2 = 0;
    u32 r1 = 0, r2 = 0;

    
#if TEST_RFID
	uint8_t *rcv_buf = NULL;
	u16 rcv_len = 0;
    u8 tmp_buf[4] = {0};
	u8 phyid[4] = {0};
#endif
	u8 date[7] = {0};
//	u8 disp_buf[24] = {0};

#if TEST_DEVMGR
	u8 type = 0;
	char body_data[64] = {0};
	u8 body_size = 0;
	char name[8] = {0};
	char pass[8] = {0};
	u8 macid[7] = {0};
#endif

#if TEST_RTC
	struct tm t;
#endif

	adf_init();

//#if TEST_COM
	com_load_config(com_cfg, sizeof(com_cfg)/sizeof(com_cfg[0]));
	com_init(COM1, 2400, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	//com_init(COM2, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	com_init(COM3, 2400, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	//com_init(COM4, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
	//com_init(COM5, 2400, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx|USART_Mode_Tx, ENABLE, 1);
//#endif 
	printf("Demo Start!\r\n");
	
	memset(&vable,0,sizeof(vable));
	memset(&read,0,sizeof(read));
	
	vable.b = 9600;
	vable.size = 1200;
	vable.data = 100;
	memcpy(vable.s,"yuju",4);
	
//写
	flash_erase_sector(TEST_ADDR, 1);//先清一下flash内容
	flash_read(TEST_ADDR, (u8 *)&read, sizeof(read));
	MYLOG_DEBUG("read.b=%X read.data=%X read.size=%X\r\n",read.b,read.size,read.data);
	
	flash_write(TEST_ADDR, (u8 *)&vable, sizeof(vable));
//读
	flash_read(TEST_ADDR, (u8 *)&read, sizeof(read));
	MYLOG_DEBUG("read.b=%d read.data=%d read.size=%d read.s=%s\r\n",read.b,read.size,read.data, read.s);
		
	MYLOG_DEBUG("sizeof=%d\r\n",sizeof(Test));
		
#if TEST_RTC
	rtc_init();
	t.tm_year = 114;
	t.tm_mon  = 2;
	t.tm_mday = 1;
	t.tm_hour = 0;
	t.tm_min  = 0;
	t.tm_sec  = 0;

	//rtc_write(t);
#endif

//#if TEST_RFID
//   rfid_load_config(&rfid_config);
//   rfid_init();
//#endif

#if TEST_PT1000
//    adc_load_config(adc_config, 2);
//    adc_init(1);
//    adc_init(2);
    BSP_ADC_Init1();
#endif

    adc_load_config(adc_config, 2);
//    adc_init(1);
//    adc_init(2);

#if TEST_LED
   led_load_config(led_config, sizeof(led_config)/sizeof(led_config[0]));
   led_init();
#endif

//#if TEST_BEEP
//	beep_load_config(&beep_config);
//	beep_init();
//#endif

//#if TEST_KEY
//	key_load_config(key_config, sizeof(key_config)/sizeof(key_config[0]));
//	key_init();
//#endif

#if TEST_EXTI
	exti_load_config(exti_config, sizeof(exti_config)/sizeof(exti_config[0]));
	exti_init();
#endif

#if TEST_TIMER_BLOCK || TEST_SYSTIMER
	systimer_init();
#endif
#if TEST_SYSTIMER 
	systimer_config(TIMER_T0, 1000);
#endif	

#if TEST_FLASH_EXT
	flash_ext_load_config(&flash_config);
	flash_ext_init();

	printf("Flash Type:%06x\r\n", flash_ext_read_id());
#endif

#if TEST_LCD
	lcd_load_config(&lcd_config);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
	lcd_init();
    
//	font_load_config(font_list, sizeof(FontCfg_t));
//	
//	lcd_set_font(E_FONT_TYPE_ASCII_6x12, E_FONT_TYPE_ST_12x12);
//	//lcd_set_backlight(LCD_BACKLIGHT_ON);
//	lcd_disp_clrscr();
////	lcd_disp_line_center(0, 0, 20, "北京新城热力");
////	lcd_disp_line_center(2, 0, 20, "集中刷卡器");
//	lcd_set_mode(0);
//	lcd_disp_line_center(0, 0, 20, "充值成功");
//	lcd_set_mode(1);
//	lcd_disp_line(1, 0, 20, 1, "房 间 号:1-201");
//	lcd_disp_line(2, 0, 20, 1, "充值热量:125.75kWh");

#endif

#if TEST_FILESYSTEM
	MYLOG_DEBUG("FileSystem Test!\r\n");

	flash_ext_load_config(&flash_config);
	flash_ext_init();

	flash_ext_erase_4k(0);
	flash_ext_erase_4k(1);
	flash_ext_erase_4k(2);
	flash_ext_erase_4k(3);

	MYLOG_DEBUG("Flash Type:%06x\r\n", flash_ext_read_id());

	fs_mount();

	rv = fs_load(E_FILE_TYPE_SYSINF, TEST_FILE_SYSINF, TEST_FILE_SYSINF_SIZE, sizeof(SysParams));
	if (rv != E_FS_OK)
	{
		MYLOG_DEBUG("Load[TEST_FILE_SYSINF] failed!\r\n");
		//return;
	}

	MYLOG_DEBUG("sizeof(g_sys_params1):%d\r\n", sizeof(g_sys_params1));

	memset(&g_sys_params1, 0, sizeof(g_sys_params1));
	memcpy(g_sys_params1.dev_addr, "\x12\x34\x56\x78\x90\x12\x34", 7);

	rv = fs_load(E_FILE_TYPE_LOG, TEST_FILE_RECHARGE_LOG, TEST_FILE_RECHARGE_LOG_SIZE, sizeof(RechargeLog));
	if (rv != E_FS_OK)
	{
		MYLOG_DEBUG("Load[TEST_FILE_RECHARGE_LOG] failed!\r\n");		  	
		//return;
	}

	MYLOG_DEBUG("sizeof(RechargeLog):%d\r\n", sizeof(RechargeLog));
	memset(&g_recharge_log1, 0, sizeof(RechargeLog));
#endif
	/* Add your application code here	*/
	
	/* Infinite loop */

#if TEST_HT1621
//	BSP_LCD_LoadConfig(LCD_config);
////	BSP_LCD_PowerOn();
//	BSP_LCD_Init();
    BSP_GPIO_Init(PORTB, PIN6, GPIO_Mode_IN_FLOATING);//IRQ
    //BSP_GPIO_SetHigh(PORTB, PIN6);  
    //BSP_GPIO_SetLow(PORTB, PIN6);  
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
    
    HT1621Init();
	HT1621FillFull();
#endif

#if TEST_RF
    rf_load_config(&rf_config);
    rf_close();
    rf_power_off();

    /* si4432无线进行测试 */
    rf_power_on();
    rv = rf_init(RF_FRE_430_5M, RF_DATA_RATE_1200, TXPOW_20dBm, 15);//这里初始化为420.5M  所以阀门程序也要设置为这个频段才能通讯成功
    if (rv != E_RF_OK)
    {
      delay_ms(1);
    }

    rf_rcv_init();
#endif
    
#if TEST_MOTOR
    motor_load_config(&motor_config);
    motor_init();
#endif
	while (1)
	{
        
#if TEST_LCD
    HT1621FillFull();
            com_send_str(COM1, "UART1 Test...\r\n");
    delay_ms(1000);
    HT1621FillZero();
            com_send_str(COM3, "UART3 Test...\r\n");
    delay_ms(1000);
#endif
        
#if TEST_MOTOR
        
    //motor_run(E_MOTOR_SWITCH_CLOSED);
    //delay_ms(10000);
    if(motor_get_state() == MOTOR_STATE_OPENED)
    {
        motor_run(E_MOTOR_SWITCH_CLOSED);

    }
    
    if(motor_get_state() == MOTOR_STATE_CLOSED)
    {
        motor_run(E_MOTOR_SWITCH_OPENED);
    }

#endif

#if TEST_COM
//		printf("UART1 Test...\r\n");
//		MYLOG_DEBUG("UART1 Test...\r\n");
		com_send_str(COM1, "UART1 Test...\r\n");
//		com_send_str(COM2, "UART2 Test...\r\n");
		com_send_str(COM3, "UART3 Test...\r\n");
//		com_send_str(COM4, "UART4 Test...\r\n");
//		com_send_str(COM5, "UART5 Test...\r\n");
		rcv_len = sizeof(rcv_buf);
		
		rv = com_recv_data(COM1, &rcv_buf, &rcv_len);
		if (E_COM_OK == rv)
		{			
			com_send_data(COM1, rcv_buf, rcv_len);	
			com_recv_init(COM1);
		}

//		rtc_read_bcd(date, 7);
//		MYLOG_DEBUG_HEXDUMP("Date:", date, 7);
//		MYLOG_DEBUG("This is dbg module test...\r\n");
        //com_send_str(COM3, "This is dbg module test...\r\n");
		delay_ms(1000);
#endif
        
#if TEST_RF
          rv = rf_rcv_data(&rcv_dat, &rcv_rflen);
          if(rv == 0)
          {
    //        lcd_temp_data_disp(g_run_params.temp);
    //        MYLOG_DEBUG_HEXDUMP("RF Config Recv:", rcv_dat, rcv_len);
    //        disp_temp_room(330);
    //        disp_update();
              //rv = memcmp(rcv_dat, "start-test", 10);
    //          rv = memcmp(rcv_dat, "hello", 6);
    //          if (rv == 0)
    //          {
                /* 发送 */
    //            printf("RECV - rcv_dat = %s\r\n", rcv_dat);
                MYLOG_DEBUG_HEXDUMP("RECV - rcv_dat: ", rcv_dat, rcv_rflen);
                rf_snd_data(rcv_dat, rcv_rflen);
    //          }

              rf_rcv_init();
          }
#endif
    adc_init(0);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    temp1 = adc_sample(0);

    adc_init(1);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
    temp2 = adc_sample(1);
        
    r1 = (u32)((10*temp1+79852.5) / (120802.5-10*temp1) * 1000.00 * 1000.00);
    if(r1 <= 1000000)        //小于0℃，默认为0℃
    {
        r1 = 1000000;
    }
    
    r2 = (u32)((10*temp2+79852.5) / (120802.5-10*temp2) * 1000.00 * 1000.00);
    if(r2 <= 1000000)        //小于0℃，默认为0℃
    {
        r2 = 1000000;
    }
    
    printf("\r\n R1 = %u\r\n R2 = %u\r\n", r1, r2);
    temp1 = task_query_temp_by_resistance(r1);
    temp2 = task_query_temp_by_resistance(r2);
    printf("temp1 = %d\r\ntemp2 = %d\r\n", temp1, temp2);
    delay_ms(2000);

#if TEST_PT1000 
    BSP_ADC_Init1();
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    temp1 = BSP_ADC_SampleOnce1();
        
    BSP_ADC_Init1();
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
    temp2 = BSP_ADC_SampleOnce1();
        
    r1 = (u32)((10*temp1+79852.5) / (120802.5-10*temp1) * 1000.00 * 1000.00);
    if(r1 <= 1000000)        //小于0℃，默认为0℃
    {
        r1 = 1000000;
    }
    
    r2 = (u32)((10*temp2+79852.5) / (120802.5-10*temp2) * 1000.00 * 1000.00);
    if(r2 <= 1000000)        //小于0℃，默认为0℃
    {
        r2 = 1000000;
    }
    
    printf("\r\n R1 = %u\r\n R2 = %u\r\n", r1, r2);
    temp1 = task_query_temp_by_resistance(r1);
    temp2 = task_query_temp_by_resistance(r2);
    printf("temp1 = %d\r\ntemp2 = %d\r\n", temp1, temp2);
    delay_ms(2000);
#endif
//#if TEST_RFID
//	    // 寻卡
//	    rv = rfid_request(0x26, tmp_buf);
//	    if (rv != MI_OK)
//	        continue;
//	
//	    printf("Card Type:%02X%02X\r\n", tmp_buf[0], tmp_buf[1]);
//	
//	    // 防冲突
//	    rv = rfid_anticoll(phyid);
//	    if (rv != MI_OK)
//	    {
//			printf("rfid_anticoll failed!\r\n");
//			continue;
//		}
//	
//	    // 选择卡
//	    memset(tmp_buf, 0, sizeof(tmp_buf));
//	    rv = rfid_select(phyid, tmp_buf);
//	    if (rv != MI_OK)
//		{
//			printf("Select Card Failed!\r\n");
//			continue;
//		}
//	
//	    printf("Select OK!\r\n");
//		printf("Card No:%02X%02X%02X%02X\r\n", phyid[0], phyid[1], phyid[2], phyid[3]);
//#endif

#if TEST_LED
		led_on(0);
		led_off(1);
		led_off(2);
		delay_ms(500);
		
		led_on(1);
		led_off(0);
		led_off(2);
		delay_ms(500);
		
		led_on(2);
		led_off(1);
		led_off(0);
		delay_ms(500);
#endif

//#if TEST_BEEP
//		beep_on();
//		delay_ms(500);
//		beep_off();
//		delay_ms(500);
//#endif

#if TEST_SYSTIMER
		if (E_TIMER_OVF == systimer_get_over_flag(TIMER_T0))
		{
			systimer_clr_over_flag(TIMER_T0);
			systimer_config(TIMER_T0, 1000);
	
			MYLOG_DEBUG("This is timer test...\r\n");
		}
#endif

#if TEST_TIMER_BLOCK
		delay_ms(1000);	
		MYLOG_DEBUG("This is timer block test...\r\n");
#endif

//		printf("Hello!");

//		flash_read(TEST_ADDR, read_data, sizeof(read_data));
		
//		MYLOG_DEBUG_HEXDUMP("Read:", read_data, sizeof(read_data));
//		for(j=0; j<sizeof(read_data); j++)
//		{
//			delay_ms(100);
//			//com_send_data(COM2, read_data+j, 1);
//			printf("read_data[%d]=%d\r\n",j,read_data[j]);
//		}

//	if(i<=3)
//	{
//		memset(test_data, 0+i, sizeof(test_data));//  写Test_data数组

//		MYLOG_DEBUG_HEXDUMP("Write:", test_data, sizeof(test_data));
//		for(z=0; z<sizeof(test_data); z++)
//		{
//			delay_ms(100);
//			//com_send_data(COM2, test_data+j, 1);
//			printf("test_data[%d]=%d\r\n",z,test_data[z]);
//		}
	
		
//		MYLOG_DEBUG_HEXDUMP("Read:", read_data, sizeof(read_data));
//		for(j=0; j<sizeof(read_data); j++)
//		{
//			delay_ms(100);
//			//com_send_data(COM2, read_data+j, 1);
//			printf("read_data[%d]=%d\r\n",j,read_data[j]);
//		}

//		i++;
//	}
//		if (i == 10)
//		{
//			i = 0;
//		}

#if TEST_FLASH_EXT
		flash_ext_erase_4k(0);

		memset(read_data, 0, sizeof(read_data));
		rv = flash_ext_read(0x000000, read_data, sizeof(read_data));
		MYLOG_DEBUG("Return code:%d\r\n", rv);
		MYLOG_DEBUG_HEXDUMP("[0]Read:", read_data, sizeof(read_data));

		memset(test_data, '0'+i, sizeof(test_data));
		MYLOG_DEBUG_HEXDUMP("[0]Write:", test_data, sizeof(test_data));		
		flash_ext_write(0x000000, test_data, sizeof(test_data));

		memset(read_data, 0, sizeof(read_data));
		flash_ext_read(0x000000, read_data, sizeof(read_data));
		MYLOG_DEBUG_HEXDUMP("[1]Read:", read_data, sizeof(read_data));

		i++;
		if (i == 10)
		{
			i = 0;
		}
		delay_ms(1000);
#endif
##################################################################################################
      
#if	TEST_FLASH 	|| TEST_FLASH_EXT
//u8 test_data[5120] = {0};
//u8 read_data[5120] = {0};
u8 test_data[4] = {0};
u8 read_data[4] = {0};
#endif

typedef struct _test
{
	u32 b;
	u16 size;
	u8  data;
	char s[5];
}Test;

#if TEST_FILESYSTEM
typedef struct _SysParams{
	u32			records_wr;
	u32			records_rd;
	u8			passwd_ori[16];
	u8			dev_addr[7];			
}SysParams;

typedef struct _RechargeLog{
	u8  dev_addr[7];					// éè±?μ???
	u8	room_id[3];						// ????o?
    u8  datetime[7];                    // 3??μê±??
    u16 recharge_cnt;                   // 3??μDòo?
    u32 recharge_heat_total;            // à???3??μèèá?
    u32 recharge_heat;                  // 3??μèèá?
}RechargeLog;

SysParams	g_sys_params1, g_sys_params2;
RechargeLog	g_recharge_log1, g_recharge_log2;

#define TEST_FILE_SYSINF				(0x0000) 
#define TEST_FILE_SYSINF_SIZE			(0x2000/sizeof(SysParams)*sizeof(SysParams))
#define TEST_FILE_RECHARGE_LOG			(0x2000)
#define TEST_FILE_RECHARGE_LOG_SIZE	   	(0x2000/sizeof(RechargeLog)*sizeof(RechargeLog))

#endif
################################################################################################## 
#if TEST_FILESYSTEM
		// 填充充值记录
		memcpy(g_recharge_log1.dev_addr, "\x12\x34\x56\x78\x90\x12\x34", 7);
		memcpy(g_recharge_log1.room_id, "\x01\x01\x01", 3);
		g_recharge_log1.recharge_cnt++;
		g_recharge_log1.recharge_heat_total += 200;
		g_recharge_log1.recharge_heat = 200;
		rtc_read_bcd(g_recharge_log1.datetime, 7);

		// 写记录
		rv = fs_write(TEST_FILE_RECHARGE_LOG, g_sys_params1.records_wr*sizeof(g_recharge_log1), (u8*)&g_recharge_log1, sizeof(g_recharge_log1));
		if (rv != E_FS_OK)
		{
			MYLOG_DEBUG("fs_write[E_FILE_TYPE_LOG] failed!Return Code[%d]\r\n", rv);
			//return;
		}

		// 写配置文件
		g_sys_params1.records_wr++;
		g_sys_params1.records_wr = g_sys_params1.records_wr % (TEST_FILE_RECHARGE_LOG_SIZE/sizeof(g_recharge_log1));
		rv = fs_write(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params1, sizeof(g_sys_params1));	
		if (rv != E_FS_OK)
		{
			MYLOG_DEBUG("fs_write[E_FILE_TYPE_SYSINF] failed!Return Code[%d]\r\n", rv);
			//return;
		}

		
		
		// 读配置文件
		rv = fs_read(TEST_FILE_SYSINF, 0, (u8*)&g_sys_params2, sizeof(g_sys_params2));
		if (rv != E_FS_OK)
		{
			MYLOG_DEBUG("fs_read[E_FILE_TYPE_SYSINF] failed!Return Code[%d]\r\n", rv);
			//return;
		}

		MYLOG_DEBUG_HEXDUMP("[INF]DevAddr:", g_sys_params2.dev_addr, 7);
		MYLOG_DEBUG("[INF]Record SN:%d\r\n", g_sys_params2.records_wr);

		// 读日志
		memset(&g_recharge_log2, 0, sizeof(g_recharge_log2));
		rv = fs_read(TEST_FILE_RECHARGE_LOG, g_sys_params1.records_rd*sizeof(g_recharge_log2), (u8*)&g_recharge_log2, sizeof(g_recharge_log2));
		if (rv != E_FS_OK)
		{
			MYLOG_DEBUG("fs_write[E_FILE_TYPE_LOG] failed!Return Code[%d]\r\n", rv);
			//return;
		}
		g_sys_params1.records_rd++;
		g_sys_params1.records_rd = g_sys_params1.records_rd % (TEST_FILE_RECHARGE_LOG_SIZE/sizeof(g_recharge_log2));

		MYLOG_DEBUG_HEXDUMP	("[Log]DevAddr:", 				g_recharge_log2.dev_addr, 	7);
		MYLOG_DEBUG			("[Log]RoomID:%x-%x%02x\r\n", 	g_recharge_log2.room_id[0], g_recharge_log2.room_id[1], g_recharge_log2.room_id[2]);
		MYLOG_DEBUG			("[Log]SN:%d\r\n", 				g_recharge_log2.recharge_cnt);
		MYLOG_DEBUG			("[Log]Heat:%lu\r\n", 			g_recharge_log2.recharge_heat);
		MYLOG_DEBUG			("[Log]Toal Heat:%lu\r\n", 		g_recharge_log2.recharge_heat_total);
		MYLOG_DEBUG_HEXDUMP	("[Log]Time:", 					g_recharge_log2.datetime, 	7);

		delay_ms(1000);
		printf("-------------------------------------------\r\n\r\n");
#endif

//#if TEST_DEVMGR
//		rcv_len = sizeof(rcv_buf);
//		memset(rcv_buf, 0, sizeof(rcv_buf));
//		rv = com_recv_data(COM1, rcv_buf, &rcv_len);
//		if (E_COM_OK == rv)
//		{		
//			MYLOG_DEBUG("%s", rcv_buf);	
//			body_size = sizeof(body_data);
//			rv = dev_mgr_unpack(&type, (char*)body_data, body_size, (char*)rcv_buf, rcv_len);
//			if (E_DEVMGR_OK == rv)
//			{
//				MYLOG_DEBUG("Type:%d\r\n", type);	

//				switch(type)
//				{
//				case E_DEVMGR_TYPE_AT:
//					rcv_len = sizeof(rcv_buf);
//					dev_mgr_response_sucess((char*)rcv_buf, rcv_len);
//					//MYLOG_DEBUG("Send:%s", rcv_buf);	
//					break;
//				case E_DEVMGR_TYPE_LOGIN:
//					rv = dev_mgr_request_login(name, pass, body_data);
//					if (E_DEVMGR_OK == rv)
//					{
//						if (memcmp(name, "root", 4)==0 && memcmp(pass, "123456", 6)==0)
//						{
//							rcv_len = sizeof(rcv_buf);
//							dev_mgr_response_sucess((char*)rcv_buf, rcv_len);
//						}
//						else
//						{
//							rcv_len = sizeof(rcv_buf);
//							dev_mgr_response_failed((char*)rcv_buf, rcv_len);
//						}	
//					}
//					else
//					{
//						rcv_len = 0;
//					}
//				 	break;
//				case E_DEVMGR_TYPE_MACID_SET:
//					rv = dev_mgr_request_set_macid(macid, 7, body_data);
//					if (E_DEVMGR_OK == rv)
//					{
//						MYLOG_DEBUG_HEXDUMP("MacID:", macid, 7);	
//						rcv_len = sizeof(rcv_buf);
//						dev_mgr_response_sucess((char*)rcv_buf, rcv_len);
//					}
//					else
//					{
//						rcv_len = sizeof(rcv_buf);
//						dev_mgr_response_failed((char*)rcv_buf, rcv_len);
//					}
//				 	break;
//				case E_DEVMGR_TYPE_MACID_GET:
//					rcv_len = sizeof(rcv_buf);
//					rv = dev_mgr_response_get_macid((char*)rcv_buf, rcv_len, macid, 7);
//					if (E_DEVMGR_OK != rv)
//					{
//						rcv_len = sizeof(rcv_buf);
//						dev_mgr_response_failed((char*)rcv_buf, rcv_len);
//					}
//					break;
//				case E_DEVMGR_TYPE_DLAD:
//					rcv_len = sizeof(rcv_buf);
//					dev_mgr_response_sucess((char*)rcv_buf, rcv_len);					
//					break;
//				default:
//					break;
//				}
//			}

//			com_send_str(COM1, rcv_buf);	
//			com_recv_init(COM1);
//		}
//#endif
	}
}
//
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//  USART_SendData(USART1, (uint8_t) ch);
//
//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//  {}
//
//  return ch;
//}

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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
