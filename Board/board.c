#include <stdio.h>
#include "adf.h"
#include "board.h"
#include "adc.h"

void DELAY_MS(int ms)
{
    for(int i=0; i<ms; i++)
    {
        delay_us(2880);
    }
}

//////////////////////////////// COM ÅäÖÃ ////////////////////////////
uint8_t com_global_buffer[500] = {0};
uint8_t rf_global_buffer[80] = {0};

COMConfig com_cfg[3] = {
	{COM1, {PORTA, PIN9}, 	{PORTA, PIN10},	{0, 0}, 		com_global_buffer+0, 	299},
	{COM2, {PORTA, PIN2}, 	{PORTA, PIN3},	{0, 0},  		com_global_buffer+299, 	1},
	{COM3, {PORTB, PIN10}, 	{PORTB, PIN11}, {0, 0}, 		com_global_buffer+300, 	200},
//	{COM4, {PORTC, PIN10}, 	{PORTC, PIN11}, {PORTD, PIN0}, 	com_global_buffer+256, 	64},
//	{COM5, {PORTC, PIN12}, 	{PORTD, PIN2},  {PORTD, PIN1},	com_global_buffer+320, 	64},
    
};

/////////////////////////////////RFID/////////////////////////////////
//RFIDConfig rfid_config = {
//    {0, 0},						/*!< \brief pwr */
//    {PORTC, PIN3},				/*!< \brief rst */
//    {PORTA, PIN4},				/*!< \brief nss */
//    {PORTA, PIN5},				/*!< \brief sck */
//    {PORTA, PIN7},				/*!< \brief mosi */
//    {PORTA, PIN6},				/*!< \brief miso */
//};

//////////////////////////////// LED //////////////////////////////////
PortTypeDef led_config[3] = {
	{PORTB, PIN7},
    {PORTB, PIN8},
	{PORTB, PIN9},
};

//////////////////////////////// LCD //////////////////////////////////
//typedef struct _HT1621Config{
//    PortTypeDef pwr;        /*!< \brief HT1621 Power Pin */
//    PortTypeDef cs;         /*!< \brief HT1621 CS Pin */
//    PortTypeDef rd;         /*!< \brief HT1621 RD Pin */
//    PortTypeDef wr;         /*!< \brief HT1621 WR Pin */
//    PortTypeDef dat;        /*!< \brief HT1621 DATA Pin */
//}HT1621Config;	
HT1621Config lcd_config = {
	{(Port)0, (Pin)0},      //Power Pin
    {PORTA, PIN15},         //CS
	{PORTB, PIN3},          //RD
	{PORTB, PIN4},          //WR
    {PORTB, PIN5}           //DATA
};
/////////////////////////////////BEEP/////////////////////////////////
//PortTypeDef beep_config = {
//	PORTC, PIN4
//};

/////////////////////////////////KEY//////////////////////////////////
//PortTypeDef key_config[1] = {
//	{PORTE, PIN5},
//};

////////////////////////////////Íâ²¿ÖÐ¶Ï///////////////////////////////
EXTIConfig exti_config[1] = {
	{{PORTC, PIN13}, EXTI_Trigger_Rising_Falling}, 
};

///////////////////////////////FLASH//////////////////////////////////
FlashConfig flash_config = {
    {PORTB, PIN12},				/*!< \brief nss */
    {PORTB, PIN13},				/*!< \brief sck */
    {PORTB, PIN15},				/*!< \brief mosi */
    {PORTB, PIN14},				/*!< \brief miso */
};

////////////////////////////si4432(RF)////////////////////////////////
//typedef struct _RFConfig{
//    PortTypeDef pwr;            /*!< \brief rfid power control pin */
//    PortTypeDef mosi;           /*!< \brief rfid mosi pin */
//    PortTypeDef miso;           /*!< \brief rfid miso pin */
//    PortTypeDef sck;            /*!< \brief rfid serial clock */
//    PortTypeDef nss;            /*!< \brief rfid chip select */
//    PortTypeDef sdn;            /*!< \brief rfid sdn pin */
//    PortTypeDef nirq;           /*!< \brief rfid nirq pin */
//    u8*         rcv_ptr;        /*!< \brief rfid recv data pointer */
//    u16         max_rcv_size;   /*!< \brief rfid max recv data buffer size */
//}RFConfig;

RFConfig rf_config = {
	{(Port)0, (Pin)0},
    {PORTA, PIN7},
    {PORTA, PIN6},
    {PORTA, PIN5},
    {PORTA, PIN4},
    {PORTB, PIN0},
    {PORTB, PIN1},
    rf_global_buffer+0, 
    80
};


////////////////////////// ADCÅäÖÃ //////////////////////////////////
//typedef struct _ADCConfig{
//    ADC_TypeDef*    adc;
//    PortTypeDef     ctrl;
//    PortTypeDef     ai;
//    u8              pwr_le;
//}ADCConfig;
ADCConfig adc_config[2] = {
    {
        E_ADC1,             //ADC
        {(Port)0, (Pin)0},  //Ctrl
        {PORTA, PIN0},      //ai
        E_LE_LOW            //pwr_le
    },
    {
        E_ADC1,             //ADC
        {(Port)0, (Pin)0},  //Ctrl
        {PORTA, PIN1},      //ai
        E_LE_LOW            //pwr_le
    }
};

///////////////////////// MotorÅäÖÃ ////////////////////////////////
//typedef struct _MotorConfig{
//    PortTypeDef pwr;        /*!< \brief motor power */
//    PortTypeDef fin;        /*!< \brief motor fin pin */
//    PortTypeDef rin;        /*!< \brief motor rin pin */
//    PortTypeDef state_close;/*!< \brief motor close state */
//    PortTypeDef state_open; /*!< \brief motor open state */
//    PortTypeDef ai;         /*!< \brief motor valve position check */
//    u8          adc;        /*!< \brief motor valve position check adc idx */
//}MotorConfig;
MotorConfig motor_config = {
    {(Port)0, (Pin)0},
    {PORTA, PIN11},
    {PORTA, PIN12},
    {PORTA, PIN8},
    {PORTA, PIN2},
    {(Port)0, (Pin)0},
    0
};
