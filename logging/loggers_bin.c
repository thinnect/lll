/**
 * Debug logger log formatter.
 *
 * Copyright Thinnect Inc. 2021
 * @author Enari Saar
 * @license MIT
*/

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>
#include <string.h>

#include "loggers_bin.h"

#include "__loggers.h"

#include "platform_mutex.h"
#include "platform.h"

#include "hdlc_tools.h"

#include "modules_bin_list.h"

#define MAX_ARG_COUNT 8
#define ARG_BUFFER_SIZE (MAX_ARG_COUNT * 8)
#define MAX_DATA_SIZE (ARG_BUFFER_SIZE + 4)
#define MAX_LOG_PACKET_SIZE (MAX_DATA_SIZE + 2)

static uint16_t m_log_level;
static uint16_t m_buffer[512];

static int (*mf_log_put)(const char *, int);
static uint32_t (*mf_log_time)(void);
static platform_mutex_t m_log_mutex;

static void log_mutex_acquire(void);
static void log_mutex_release(void);

void log_init(uint16_t loglevel, int (*log_fun)(const char *, int), uint32_t (*time_fun)(), platform_mutex_t mutex)
{
    mf_log_put = log_fun;
    mf_log_time = time_fun;
    m_log_level = loglevel;
    m_log_mutex = mutex;
}

void __logger(uint16_t severity, uint16_t modid, uint16_t __line__, const char *fmt, uint16_t numargs, ...)
{
    if (!(m_log_level & severity))
    {
        return;
    }

    uint8_t args[ARG_BUFFER_SIZE] = {0};
    uint32_t pos = 0;

    va_list arg;
    va_start(arg, fmt);
    read_format(fmt,arg,&args[0],&pos);
    va_end(arg);

    uint8_t data[MAX_DATA_SIZE] = {0};
    data[0] = modid & 0xFF00;   //Module ID
    data[1] = modid & 0x00FF;   //Module ID
    data[2] = __line__ & 0xFF00; //line nr
    data[3] = __line__ & 0x00FF; //line nr
    if (pos != 0)                // if no arguments no point of copying the args array
    {
        memcpy(&data[4], &args[0], pos);
    }
    uint8_t total_size = 4 + pos;
    uint8_t log_packet[MAX_LOG_PACKET_SIZE] = {0};
    int status = hdlc_encode(log_packet, 2 + total_size, data, total_size);
    log_mutex_acquire();
    if (status != -1)
    {
        mf_log_put(&log_packet[0], 2 + total_size);
    }
    log_mutex_release();
}

void __loggerb(uint16_t severity, uint16_t modid, uint16_t __line__,
               const char *fmt, const void *data, uint8_t len, uint16_t numargs, ...)
{
    if (!(m_log_level & severity))
    {
        return;
    }

    uint8_t args[ARG_BUFFER_SIZE] = {0};
    uint8_t pos = 0;

    va_list arg;
    va_start(arg, fmt);
    read_format(fmt,arg,&args[0],&pos);
    va_end(arg);

    uint8_t _data[MAX_DATA_SIZE] = {0};
    _data[0] = modid & 0xFF00;   //Module ID
    _data[1] = modid & 0x00FF;   //Module ID
    _data[2] = __line__ & 0xFF00; //line nr
    _data[3] = __line__ & 0x00FF; //line nr
    if (pos != 0)                 // if no arguments no point of copying the args array
    {
        memcpy(&_data[4], &args[0], pos);
    }
    if (len != 0)
    {
        //Needs to be changed. Limits buffer size quite a lot since you cant allocate 512bytes of stack everytime.
        memcpy(&_data[4+pos], &data[0], len);
        pos += len;
    }
    uint8_t total_size = 4 + pos;
    uint8_t log_packet[MAX_LOG_PACKET_SIZE] = {0};
    int status = hdlc_encode(log_packet, 2 + total_size, _data, total_size);
    log_mutex_acquire();
    if (status != -1)
    {
        mf_log_put(log_packet, 2 + total_size);
    }
    log_mutex_release();
    
}

static void log_mutex_acquire(void)
{
    if (NULL != m_log_mutex)
    {
        platform_mutex_acquire(m_log_mutex);
    }
}

static void log_mutex_release(void)
{
    platform_mutex_release(m_log_mutex);
}
