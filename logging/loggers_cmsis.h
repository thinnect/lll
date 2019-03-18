#ifndef LOGGERS_CMSIS_H_
#define LOGGERS_CMSIS_H_

#include <stdint.h>
#include <stdarg.h>

//#include "logger.h"

void log_init(int(*puts)(const char *));
void log(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, ...);
void logb(uint16_t severity, char *moduul, uint16_t __line__, char *fmt, void *data, uint16_t len, ...);

#endif//LOGGERS_CMSIS_H_
