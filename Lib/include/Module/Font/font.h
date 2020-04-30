#ifdef HAVE_FONT

#ifndef _FONT_H
#define _FONT_H

typedef struct _FontCfg{
	unsigned char	width;
	unsigned char	height;
	unsigned char	size;
	unsigned char*	addr;
}FontCfg_t;

/*! \brief
*       加载字体信息
* \param font_list[IN]		- 字体配置列表
* \param font_cnt[OUT]		- 字体数量
*/
void font_load_config(void *font_list, u8 font_cnt);

/*! \brief
*       获得字体属性
* \param type[IN]		- 字体类型
* \param width[OUT]		- 字体宽度
* \param height[OUT]	- 字体高度
* \param size[OUT]		- 字体数据大小
*/
void font_getattr(u8 type, u8 *width, u8 *height, u8 *size);

/*! \brief
*       获得字体显示编码数据
* \param msk[OUT]		- 字体编码数据
* \param len[IN]		- 字体编码数据大小
* \param type[IN]		- 字体类型
* \param idx[IN]		- 字体编码
* \return 
*		0		- 成功
*/
u8  font_get_msk_data(u8 *msk, u8 len, u8 type, u16 idx);

#endif

#endif
