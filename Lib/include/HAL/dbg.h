/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
* 
* \brief 
*    ���Խӿ�
* \author 
*    yujh_empty@foxmail.com 
* \date 
*    2011-09-05
* \version 
*    v1.0
* \note
*    ��������:
*       |-->MYLOG_WARN()
*       |-->MYLOG_ERROR()
*       |-->MYLOG_TRACE()
*       |-->MYLOG_INFO()
*       |-->MYLOG_DEBUG()
*       |-->MYLOG_DEBUG_HEXDUMP()
* Copyright (c) 2011, yujh_empty@foxmail.com Corporation All rights reserved.
******************************************************************************/
#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef HAVE_DBG

#include <stdio.h>

/*! \brief ���Լ��� */
typedef enum _DebugLevel{
	L_NONE  = 0,	/*!< \brief �ر� */
	L_ERROR = 1,	/*!< \brief ���� */
	L_WARN  = 2,	/*!< \brief ���� */
	L_TRACE = 3,	/*!< \brief �켣 */
	L_INFO  = 4,	/*!< \brief ��Ϣ */
	L_DEBUG = 5,	/*!< \brief ���� */
	L_ALL   = 6	    /*!< \brief ȫ�� */
}DebugLevel;

#define DEBUG_PRINT(x)		printf(x)		

#define MAX_DEBUG_LENGTH	128

//#define DEBUG_LEVEL	L_DEBUG

 /*! \brief ���õ�������ȼ� */
void MYLOG_LEVEL(DebugLevel lvl);

/*! \brief ������� */
void MYLOG_WARN(const char *fmt, ...);

/*! \brief ������� */
void MYLOG_ERROR(const char *fmt, ...);

/*! \brief ����켣 */
void MYLOG_TRACE(const char *fmt, ...);

/*! \brief �����Ϣ */
void MYLOG_INFO(const char *fmt, ...);

/*! \brief ���������Ϣ */
void MYLOG_DEBUG(const char *fmt, ...);

/*! \brief ������������� */
void MYLOG_DEBUG_HEXDUMP(char *msg, unsigned char* bytes, unsigned int len);

#endif

#endif
