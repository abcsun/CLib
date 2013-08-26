/***************************************************************************************
****************************************************************************************
* FILE		: log_trace.h
* Description	: 
*			  
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
*            Without permission, shall not be used for any commercial purpose
* 
* History:
* Version		Name       		Date			Description
   0.1		Liu Yanyun		2012/12/04		Initial Version
   
****************************************************************************************
****************************************************************************************/

#ifndef _LOG_TRACE_H_
#define _LOG_TRACE_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include <stdio.h>

void log_printf(int line,
	const char *file,
	const char *func,
	const char *format,
	...);
	
#define logTrace(format, args...) \
	do{log_printf(__LINE__, __FILE__, __FUNCTION__, format, ##args);}while(0)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_LOG_TRACE_H_*/
