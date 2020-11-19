/**
 * Debug logger log formatter.
 *
 * Copyright Thinnect Inc. 2020
 * @license MIT
 */
#ifndef LOGGERS_EXT_H_
#define LOGGERS_EXT_H_

#include <stdint.h>
#include <stdarg.h>

#include "platform_mutex.h"

/**
 * Initialize the logger.
 *
 * @param loglevel Restrict loglevel at runtime.
 * @param log_fun Function for outputting a formatted log string.
 * @param time_fun Function for obtaining a timestamp for the log event, optional.
 * @param mutex Mutex for protecting the the shared formatting buffer, optional,
 *              but make sure to provide one in multi-threaded environments!
 */
void log_init(uint16_t loglevel,
              int(*log_fun)(const char*, int),
              uint32_t(*time_fun)(),
              platform_mutex_t mutex);

#endif//LOGGERS_CMSIS_H_
