#ifndef LOGGERS_EXT_H_
#define LOGGERS_EXT_H_

#include <stdint.h>
#include <stdarg.h>

void log_init(uint16_t loglevel, int(*log_fun)(const char*, int));
void log(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, ...);
void logb(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, void *data, uint16_t len, ...);

#endif//LOGGERS_CMSIS_H_
