#ifndef LOGGERS_EXT_H_
#define LOGGERS_EXT_H_

#include <stdint.h>
#include <stdarg.h>

void log_init(uint16_t loglevel,
              int(*log_fun)(const char*, int),
              uint32_t(*time_fun)());

#endif//LOGGERS_CMSIS_H_
