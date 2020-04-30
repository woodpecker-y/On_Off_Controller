#ifndef _ST7565R_H
#define _ST7565R_H

#ifdef HAVE_LCD_ST7565R

#include "bsp_gpio.h"

#define LCD_MAX_ROW	4
#define LCD_MAX_COL	16

#define LCD_X_MAX	(128)
#define LCD_Y_MAX	(64)

/*! \brief LCD��ʾ���� */
#define LCD_DISPLAY_ON				0xaf	/*!< \brief ��ʾ�� */
#define LCD_DISPLAY_OFF				0xae	/*!< \brief ��ʾ�� */

#define	LCD_POWER_CTRL_SET			0x20	/*!< \brief ��Դ�����趨 */
#define LCD_CONTRAST_SET			0x81	/*!< \brief ���öԱȶ� */ 
#define LCD_V0_CTRL_SET				0x20	/*!< \brief �ڲ��������*/
#define LCD_BIAS_SET				0xa0	/*!< \brief ƫѹ���� */
#define LCD_PAGE_ADDR_SET			0xb0	/*!< \brief ҳ�趨 */
#define LCD_ROW_ADDR_SET			0x40	/*!< \brief �е�ַ�趨 */
#define LCD_COL_ADDR_SET_H			0x10	/*!< \brief �е�ַ�趨-���ֽ� */	
#define LCD_COL_ADDR_SET_L			0x00	/*!< \brief �е�ַ�趨-���ֽ� */	

/*! \brief Com ɨ�跽ʽ */
#define	LCD_COM_NOR					0xc0	/*!< \brief ���� */	
#define	LCD_COM_REV					0xc8	/*!< \brief ���� */

/*! \brief Segment����ѡ�� */
#define	LCD_SEG_NOR					0xa0	/*!< \brief ���� */
#define	LCD_SEG_REV					0xa1	/*!< \brief ���� */

/*! \brief ��ʾ��ʽ�趨 */ 
#define	LCD_DISPLAY_NOR				0xa6	/*!< \brief ���� */
#define	LCD_DISPLAY_REV				0xa7	/*!< \brief ���� */

/*! \brief �����趨 */ 
#define	LCD_BACKLIGHT_ON			0x01	/*!< \brief ���⿪ */
#define	LCD_BACKLIGHT_OFF			0x00	/*!< \brief ����� */

typedef struct _LCDConfig{
	PortTypeDef	nss;
	PortTypeDef	sck;
	PortTypeDef	dat;
	PortTypeDef	rst;
	PortTypeDef	a0;
	PortTypeDef backlight;
}LCDLatticeConfig;

/*! \brief
*       ����LCD�ܽ����ò���
* \param cfg[IN]		- ���þ��
*/
void BSP_LCD_LoadConfig(void* cfg);

/*! \brief
*       LCD��ʼ��
*/
void BSP_LCD_Init(void);

/*! \brief
*       LCD�ر�
*/
void BSP_LCD_Close(void);

/*! \brief
*       ����LCD��ʾ����
* \param x[IN]			- X���꣺0~127
* \param y[IN]			- Y���꣺0~63
*/
void BSP_LCD_SetPos(u8 x, u8 y);

/*! \brief
*       ����LCD��ʾ�Աȶ�
* \param level[IN]		- �Աȶȣ�1~7
*/
void BSP_LCD_SetContrast(u8 level);

/*! \brief
*       ����LCD����
* \param sts[IN]		- ���⣺LCD_BACKLIGHT_OFF-�أ�LCD_BACKLIGHT_ON-��
*/
void BSP_LCD_SetBackLight(u8 sts);

/*! \brief
*       LCDģ��SPIд����
* \param cmd[IN]		- ����
*/
void BSP_LCD_WriteCommand(u8 cmd);

/*! \brief
*       LCDģ��SPIд����
* \param data[IN]		- ����
*/
void BSP_LCD_WriteData(u8 data);

#endif

#endif
