/**
 * @author Veiko Rütter, Raido Pahtma
 * @license MIT
*/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "cmsis_os2.h"

#include "loggers_cmsis.h"

#include "__loggers.h"

extern int fileno(FILE *);
extern int _write(int file, const char *ptr, int len);
static int(*log_puts)(const char *);
static uint16_t log_level;
osMutexId_t log_mutex;


static int log_severity_char(uint16_t severity);

void log_init(int(*puts)(const char *)){
	log_puts = puts;
	log_level = 0xFFFF;
	log_mutex = osMutexNew(NULL);
}

/*
void log(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, ...){
	char buffer[256];
	va_list arg;
	int l;
	if(!(log_level & severity))return;
	l = 0;
	l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u| ", log_severity_char(severity), moduul, (unsigned int)__line__);
	va_start(arg, fmt);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
	//if(l && (buffer[l - 1] == '\n'))buffer[l - 1] = 0;
	buffer[255] = 0;
	while(osMutexAcquire(log_mutex, 1000) != osOK);
	_write(fileno(stdout), buffer, l);
	osMutexRelease(log_mutex);
}
*/

void __logger(uint16_t severity, const char *moduul, uint16_t __line__, const char *fmt, ...) {
	char buffer[256];
	va_list arg;
	int l;
	if(!(log_level & severity))return;
	l = 0;
	l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u| ", log_severity_char(severity), moduul, (unsigned int)__line__);
	va_start(arg, fmt);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
	//if(l && (buffer[l - 1] == '\n'))buffer[l - 1] = 0;
	buffer[l] = '\n';
	buffer[255] = 0;
	while(osMutexAcquire(log_mutex, 1000) != osOK);
	_write(fileno(stdout), buffer, l+1);
	osMutexRelease(log_mutex);
}


void __loggerb(uint16_t severity, const char *moduul, uint16_t __line__, const char *fmt, void *data, uint8_t len, ...){
	char buffer[256];
	va_list arg;
	int i, l;
	if(!(log_level & severity))return;
	l = 0;
	l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u| ", log_severity_char(severity), moduul, (unsigned int)__line__);
	va_start(arg, len);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
//	if(l && (buffer[l - 1] == '\n')){
//		buffer[l - 1] = 0;
//		l--;
//	}
	for(i = 0; i < len; i++){
		if(!(i % 4)){
			l += snprintf(&buffer[l], (256 - l), " %02X", (unsigned int)((uint8_t *)data)[i]);
		}else{
			l += snprintf(&buffer[l], (256 - l), "%02X", (unsigned int)((uint8_t *)data)[i]);
		}
	}
	buffer[255] = 0;
	while(osMutexAcquire(log_mutex, 1000) != osOK);
	_write(fileno(stdout), buffer, l);
	osMutexRelease(log_mutex);
}


void logb(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, void *data, uint16_t len, ...){
	char buffer[256];
	va_list arg;
	int i, l;
	if(!(log_level & severity))return;
	l = 0;
	l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u| ", log_severity_char(severity), moduul, (unsigned int)__line__);
	va_start(arg, len);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
//	if(l && (buffer[l - 1] == '\n')){
//		buffer[l - 1] = 0;
//		l--;
//	}
	for(i = 0; i < len; i++){
		if(!(i % 4)){
			l += snprintf(&buffer[l], (256 - l), " %02X", (unsigned int)((uint8_t *)data)[i]);
		}else{
			l += snprintf(&buffer[l], (256 - l), "%02X", (unsigned int)((uint8_t *)data)[i]);
		}
	}
	buffer[255] = 0;
	while(osMutexAcquire(log_mutex, 1000) != osOK);
	_write(fileno(stdout), buffer, l);
	osMutexRelease(log_mutex);
}


static int log_severity_char(uint16_t severity){
	if(severity & LOG_ERR1)return('E');
	if(severity & LOG_ERR2)return('E');
	if(severity & LOG_ERR3)return('E');
	if(severity & LOG_ERR4)return('E');
	if(severity & LOG_WARN1)return('W');
	if(severity & LOG_WARN2)return('W');
	if(severity & LOG_WARN3)return('W');
	if(severity & LOG_WARN4)return('W');
	if(severity & LOG_INFO1)return('I');
	if(severity & LOG_INFO2)return('I');
	if(severity & LOG_INFO3)return('I');
	if(severity & LOG_INFO4)return('I');
	if(severity & LOG_DEBUG1)return('D');
	if(severity & LOG_DEBUG2)return('D');
	if(severity & LOG_DEBUG3)return('D');
	if(severity & LOG_DEBUG4)return('D');
	return('X');
}
