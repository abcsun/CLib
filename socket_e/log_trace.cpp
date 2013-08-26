/***************************************************************************************
****************************************************************************************
* FILE		: log_trace.cc
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


#include "include/log_trace.h"
#include <stdio.h>
#include <syslog.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

void log_printf(int line,
	const char *file,
	const char *func,
	const char *format,
	...)
{
	char *tmpStr;
	int rc;
	
	struct timeval tv ;
	
	va_list ap;
	va_start(ap, format);
	rc = vasprintf(&tmpStr, format, ap);
	va_end(ap);
	
	if(rc < 0)
	{
		syslog(LOG_ERR, "PID:%d;file:%s[%d],FUN:%s; vasprintf failed!!!", 
						getpid(), file, line, func);
		return;
	}
	
	gettimeofday(&tv, NULL);
	
	syslog(LOG_ERR, "%ld.%ld;PID:%d;file:%s[%d],FUN:%s; %s", 
					tv.tv_sec, tv.tv_usec, getpid(), file, line, func, tmpStr);
         //printf仅仅是我调试使用，直接打到屏幕，可以去掉
	printf("%ld.%ld;PID:%d;file:%s[%d],FUN:%s; %s\n", 
					tv.tv_sec, tv.tv_usec, getpid(), file, line, func, tmpStr);
	
	free(tmpStr);
	
	return;
}

