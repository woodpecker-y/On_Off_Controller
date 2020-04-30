#ifndef _ST7565R_H
#define _ST7565R_H

#ifdef HAVE_LCD_ST7565R

#include "bsp_gpio.h"

#define LCD_MAX_ROW	4
#define LCD_MAX_COL	16

#define LCD_X_MAX	(128)
#define LCD_Y_MAX	(64)

/*! \brief LCD显示开关 */
#define LCD_DISPLAY_ON				0xaf	/*!< \brief 显示开 */
#define LCD_DISPLAY_OFF				0xae	/*!< \brief 显示关 */

#define	LCD_POWER_CTRL_SET			0x20	/*!< \brief 电源控制设定 */
#define LCD_CONTRAST_SET			0x81	/*!< \brief 设置对比度 */ 
#define LCD_V0_CTRL_SET				0x20	/*!< \brief 内部电阻比率*/
#define LCD_BIAS_SET				0xa0	/*!< \brief 偏压设置 */
#define LCD_PAGE_ADDR_SET			0xb0	/*!< \brief 页设定 */
#define LCD_ROW_ADDR_SET			0x40	/*!< \brief 行地址设定 */
#define LCD_COL_ADDR_SET_H			0x10	/*!< \brief 列地址设定-高字节 */	
#define LCD_COL_ADDR_SET_L			0x00	/*!< \brief 列地址设定-低字节 */	

/*! \brief Com 扫描方式 */
#define	LCD_COM_NOR					0xc0	/*!< \brief 正常 */	
#define	LCD_COM_REV					0xc8	/*!< \brief 反相 */

/*! \brief Segment方向选择 */
#define	LCD_SEG_NOR					0xa0	/*!< \brief 正常 */
#define	LCD_SEG_REV					0xa1	/*!< \brief 反相 */

/*! \brief 显示方式设定 */ 
#define	LCD_DISPLAY_NOR				0xa6	/*!< \brief 正相 */
#define	LCD_DISPLAY_REV				0xa7	/*!< \brief 反相 */

/*! \brief 背光设定 */ 
#define	LCD_BACKLIGHT_ON			0x01	/*!< \brief 背光开 */
#define	LCD_BACKLIGHT_OFF			0x00	/*!< \brief 背光关 */

typedef struct _LCDConfig{
	PortTypeDef	nss;
	PortTypeDef	sck;
	PortTypeDef	dat;
	PortTypeDef	rst;
	PortTypeDef	a0;
	PortTypeDef backlight;
}LCDLatticeConfig;

/*! \brief
*       加载LCD管脚配置参数
* \param cfg[IN]		- 配置句柄
*/
void BSP_LCD_LoadConfig(void* cfg);

/*! \brief
*       LCD初始化
*/
void BSP_LCD_Init(void);

/*! \brief
*       LCD关闭
*/
void BSP_LCD_Close(void);

/*! \brief
*       设置LCD显示坐标
* \param x[IN]			- X坐标：0~127
* \param y[IN]			- Y坐标：0~63
*/
void BSP_LCD_SetPos(u8 x, u8 y);

/*! \brief
*       设置LCD显示对比度
* \param level[IN]		- 对比度：1~7
*/
void BSP_LCD_SetContrast(u8 level);

/*! \brief
*       设置LCD背光
* \param sts[IN]		- 背光：LCD_BACKLIGHT_OFF-关；LCD_BACKLIGHT_ON-开
*/
void BSP_LCD_SetBackLight(u8 sts);

/*! \brief
*       LCD模拟SPI写命令
* \param cmd[IN]		- 命令
*/
void BSP_LCD_WriteCommand(u8 cmd);

/*! \brief
*       LCD模拟SPI写数据
* \param data[IN]		- 数据
*/
void BSP_LCD_WriteData(u8 data);

#endif

#endif
