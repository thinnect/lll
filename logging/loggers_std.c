/**
 * @author Raido Pahtma
 * @license MIT
*/

#ifndef PRINTF_BUFFER_SIZE
#define PRINTF_BUFFER_SIZE 255
#endif

#ifndef MODULE_NAME_LENGTH
#define MODULE_NAME_LENGTH 13
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <printf.h>
#include <time.h>
#include <stdint.h>

#include "logger.h"

#define PGM_P const char*
#define PROGMEM
#define PSTR(s) s
#define strlen_P(s) strlen(s)
#define strrchr_P(s, c) strrchr(s, c)
#define vfprintf_P(stream, format, arg) vfprintf(stream, format, arg)
#define printf_P(format, args...) printf(format, ##args)

void __logger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... );

uint8_t pgm_read_byte(uint8_t* byte) {
	return *byte;
}

char sev(uint16_t severity) {
	if( severity & LOG_MASK_ERR ) return 'E';
	else if( severity & LOG_MASK_WARN ) return 'W';
	else if( severity & LOG_MASK_INFO ) return 'I';
	else if( severity & LOG_MASK_DEBUG ) return 'D';
	return '?';
}

PGM_P module_name(PGM_P _file_) {
	PGM_P file;

	file = strrchr( _file_, '/' );
	if( file )
		++ file;
	else
		file = _file_;

	return file;
}

void __getTimestamp(char* ts, int max_len)
{
	int len;
	time_t rawtime;
	struct tm rawtm;

	time(&rawtime);
	rawtm = *localtime(&rawtime);
	len = strftime(ts, max_len, "%Y-%m-%d %H:%M:%S ", &rawtm);
	//len = snprintf(&ts[len], max_len - len, "%016lld", sim_time());
	//len += sim_print_time2(&ts[len], max_len - len, sim_time());
	if(len == 0) {
		ts[0] = 0;
	}
}

void __loghead( uint16_t severity, PGM_P _file_, unsigned _line_ )
{
	char timestr[80];
	__getTimestamp(timestr, sizeof(timestr));

	printf("%s %c|%13s:%4d|__loghead\n", timestr, sev(severity), _file_, _line_);
}

void __vlogger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a )
{
	char timestr[80];
	__getTimestamp(timestr, sizeof(timestr));

	printf("%s %c|%13s:%4d|__vlogger\n", timestr, sev(severity), module_name(moduul), __line__);
}

void __vloggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a )
{
	//printf("%s #%04X %c|%13s:%4d|__vlogger\n", timestr, TOS_NODE_ID, sev(severity), module_name(moduul), __line__);
	__logger( severity, moduul, __line__, format, a );
}

void __loggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... )
{
	va_list a;

	//dbg("__loggernnl\n");
	va_start( a, format );
	__logger( severity, moduul, __line__, format, a );
	va_end( a );

}

void __logger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... )
{
	char timestr[80];
	va_list a;
	int len = 0;

	__getTimestamp(timestr, sizeof(timestr));

	va_start(a, format);
	len = vsnprintf(NULL, 0, format, a) + 1;
	va_end(a);

	if(len > 0) {
		char* buf = (char*)malloc(len);
		va_start(a, format);
		vsnprintf(buf, len, format, a);
		va_end(a);

		printf("%s %c|%13s:%4d|%s\n", timestr, sev(severity), module_name(moduul), __line__, buf);
		free(buf);
	}
}

void __vloggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, const void* data, uint8_t len, va_list a)
{
	char timestr[80];
	__getTimestamp(timestr, sizeof(timestr));

	printf("%s %c|%13s:%4d|__vloggerb\n", timestr, sev(severity), module_name(moduul), __line__);
}

void __loggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, const void* data, uint8_t len, ...)
{
	char timestr[80];
	va_list a;
	char* mBuf = NULL;
	int mBufLen = 0;
	char* dBuf = NULL;
	int dBufLen = 0;
	int i;
	int loc;

	__getTimestamp(timestr, sizeof(timestr));

	if(len > 0)
	{
		dBufLen = len*2 + (len-1)/4 + 1; // groups of 4, spaces and \0
		dBuf = (char*)malloc(dBufLen);
		for(i=0;i<len;i++) {
			loc = 2*i + i/4;
			sprintf(&dBuf[loc], "%02x", ((uint8_t*)data)[i]);
			if((i > 0)&&(i%4 == 0)) dBuf[loc -1] = ' ';
		}
		if(strlen(dBuf) != dBufLen - 1)
		{
			printf("LOGGER FAIL %u %u", (int)strlen(dBuf), dBufLen);
			free(dBuf);
			return;
		}
	}
	else
	{
		dBuf = (char*)malloc(1);
		*dBuf = '\0';
	}

	va_start(a, len);
	mBufLen = vsnprintf(NULL, 0, format, a) + 1;
	va_end(a);

	if(mBufLen > 0) {
		mBuf = (char*)malloc(mBufLen);
		va_start(a, len);
		vsnprintf(mBuf, mBufLen, format, a);
		va_end(a);
	}

	printf("%s %c|%13s:%4d|%s %s\n", timestr, sev(severity), module_name(moduul), __line__, mBuf, dBuf);
	free(dBuf);
	free(mBuf);
}
