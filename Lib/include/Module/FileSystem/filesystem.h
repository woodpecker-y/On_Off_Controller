/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--filesystem driver
* \author
*    yujh_empty@foxmail.com
* \date
*    2014-09-16
* \version
*    v1.0
* \notes
*       1.�ļ�ϵͳʹ��ǰ�᣺�����ʼ���ⲿFLASH
*       �������̣�
*       |-->fs_mount()
*           |-->fs_load()
*               |-->fs_read()
*               |-->fs_write()
*               |-->fs_delete()
* Copyright (c) 2010-2014, yujh_empty@foxmail.com Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifdef HAVE_FILESYSTEM

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "bsp.h"

// ���֧���ļ���
#define MAX_FILE_CNT	10	

// �ļ�����
typedef enum _FileType{
	E_FILE_TYPE_SYSINF,							   // ϵͳ�����ļ�		
	E_FILE_TYPE_LOG,							   // ��־�ļ�
}FileType;

// �ļ���Ϣ
typedef struct _FileInfo{
	u32 file_name;		 						   // �ļ�����
	u32 file_size;		 						   // �ļ���С
	u32 record_size;	   						   // ��¼��С	
	s32 pos;									   // ƫ����:����дƫ����=��ƫ����+1
	u8  file_type;								   // �ļ�����
}FileInfo;

// �ļ�ϵͳ������
typedef enum _eFSRet{
	E_FS_OK = 0,							  		// �ɹ�
	E_FS_ERR_FILE_NUM_EXCEED_MAX,			   		// �ļ����������
	E_FS_ERR_HARDWARE,						   		// �ײ�Ӳ���ӿ�
	E_FS_ERR_NOT_EXSIT,		  						// �ļ�������
	E_FS_ERR_READ_FAIL,					   			// ��ʧ��
	E_FS_ERR_WRITE_FAIL,							// дʧ��
	E_FS_ERR_NO_RECORD,				   				// �޼�¼
	E_FS_ERR_EXCEED_FILE_SIZE,						// �����ļ���С
	E_FS_ERR_INVALID_PARAMS,						// �Ƿ��Ĳ���
	E_FS_ERR_ALREADY_WRITED,						// �ļ���д��
	E_FS_ERR_NOT_MATCH_READ_AFTER_WRITE,			// ��ȡ��д������ݺ�ƥ��
}eFSRet;

/*! \brief
*       �����ļ�ϵͳ
*/
void fs_mount(void);

/*! \brief
*       �����ļ���ϵͳ
* \param file_name[IN]		- �ļ���
* \param file_size[IN]		- �ļ���С
* \param file_type[IN]		- �ļ�����
* \param record_size[IN]	- ��¼��С
* \return
*		E_FS_OK				- �ɹ�
*		E_FS_ERR_READ_FAIL	- ʧ��
* \notes
*		1�������ļ���Ϣʱ������ͬʱ�����ļ���С�͵�����¼��С
*		2�������ļ�����ǰ��4���ֽڲ���Ϊ0xFF,0xFF,0xFF,0xFF
*/
u8 fs_load(u8 file_type, u32 file_name, u32 file_size, u32 record_size);

/*! \brief
*       ��ȡ�ļ�
* \param file_name[IN]		- �ļ���
* \param offset[IN]			- ƫ����
* \param dat[OUT]			- ��ȡ������
* \param size[IN]			- ��ȡ�����ݳ���
* \return
*		E_FS_OK						- �ɹ�
*		E_FS_ERR_NO_RECORD			- �޼�¼
*		E_FS_ERR_INVALID_PARAMS		- ��������
*		E_FS_ERR_READ_FAIL			- ʧ��
*		E_FS_ERR_EXCEED_FILE_SIZE	- ƫ���������ļ���С
*/
u8 fs_read(u32 file_name, u32 offset, u8 *dat, u32 size);

/*! \brief
*       д���ݵ��ļ�
* \param file_name[IN]		- �ļ���
* \param offset[IN]			- ƫ����
* \param dat[IN]			- д�������
* \param size[IN]			- д������ݳ���
* \return
*		E_FS_OK						- �ɹ�
*		E_FS_ERR_NOT_EXSIT			- �ļ�������
*		E_FS_ERR_INVALID_PARAMS		- ��������
*		E_FS_ERR_WRITE_FAIL			- дʧ��
*		E_FS_ERR_EXCEED_FILE_SIZE	- ƫ���������ļ���С
*/
u8 fs_write(u32 file_name, u32 offset, u8 *dat, u32 size);

/*! \brief
*       ɾ���ļ�
* \param file_name[IN]		- �ļ���
* \return
*		E_FS_OK						- �ɹ�
*		E_FS_ERR_NOT_EXSIT			- �ļ�������
*/
u8 fs_delete(u32 file_name);

#endif

#endif
