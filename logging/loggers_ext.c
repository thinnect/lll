/**
 * @author Veiko Rütter, Raido Pahtma
 * @license MIT
*/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>

#include "loggers_ext.h"

#include "__loggers.h"

static int(*log_put_func)(const char*, int);
static uint32_t(*log_time_func)();
static uint16_t log_level;

static int log_severity_char(uint16_t severity);

void log_init(uint16_t loglevel, int(*log_fun)(const char*, int), uint32_t(*time_fun)()) {
	log_put_func = log_fun;
	log_time_func = time_fun;
	log_level = loglevel;

	if(log_time_func != NULL) {
		char buffer[17];
		log_put_func(buffer, snprintf(buffer, 17, "%08"PRIx32" B|BOOT\n", log_time_func()));
	}
}

void __logger(uint16_t severity, const char* moduul, uint16_t __line__, const char* fmt, ...) {
	char buffer[256];
	va_list arg;
	int l;
	if(!(log_level & severity))return;
	l = 0;
	if(log_time_func != NULL) {
		l += snprintf(&buffer[l], (256 - l), "%08"PRIx32" %c|%s:%4u|", log_time_func(),
		              log_severity_char(severity), moduul, (unsigned int)__line__);
	}
	else {
		l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u|",
			          log_severity_char(severity), moduul, (unsigned int)__line__);
	}
	va_start(arg, fmt);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
	//if(l && (buffer[l - 1] == '\n'))buffer[l - 1] = 0;
	buffer[l] = '\n';
	//buffer[255] = 0;

	log_put_func(buffer, l+1);
}


void __loggerb(uint16_t severity, const char* moduul, uint16_t __line__, const char* fmt, void *data, uint8_t len, ...) {
	char buffer[256];
	va_list arg;
	int i, l;
	if(!(log_level & severity))return;
	l = 0;
	if(log_time_func != NULL) {
		l += snprintf(&buffer[l], (256 - l), "%08"PRIx32" %c|%s:%4u|", log_time_func(),
		              log_severity_char(severity), moduul, (unsigned int)__line__);
	}
	else {
		l += snprintf(&buffer[l], (256 - l), "%c|%s:%4u|",
		              log_severity_char(severity), moduul, (unsigned int)__line__);
	}
	va_start(arg, len);
	l += vsnprintf(&buffer[l], (256 - l), fmt, arg);
	va_end(arg);
//	if(l && (buffer[l - 1] == '\n')){
//		buffer[l - 1] = 0;
//		l--;
//	}
	for(i = 0; i < len; i++){
		if(l < 255) {
			if(!(i % 4)){
				l += snprintf(&buffer[l], (256 - l), " %02X", (unsigned int)((uint8_t *)data)[i]);
			}else{
				l += snprintf(&buffer[l], (256 - l), "%02X", (unsigned int)((uint8_t *)data)[i]);
			}
		}
	}
	buffer[l] = '\n';
	//buffer[255] = 0;

	log_put_func(buffer, l+1);
}


static int log_severity_char(uint16_t severity) {
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
