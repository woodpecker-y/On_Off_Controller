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
*       ����������Ϣ
* \param font_list[IN]		- ���������б�
* \param font_cnt[OUT]		- ��������
*/
void font_load_config(void *font_list, u8 font_cnt);

/*! \brief
*       �����������
* \param type[IN]		- ��������
* \param width[OUT]		- ������
* \param height[OUT]	- ����߶�
* \param size[OUT]		- �������ݴ�С
*/
void font_getattr(u8 type, u8 *width, u8 *height, u8 *size);

/*! \brief
*       ���������ʾ��������
* \param msk[OUT]		- �����������
* \param len[IN]		- ����������ݴ�С
* \param type[IN]		- ��������
* \param idx[IN]		- �������
* \return 
*		0		- �ɹ�
*/
u8  font_get_msk_data(u8 *msk, u8 len, u8 type, u16 idx);

#endif

#endif
