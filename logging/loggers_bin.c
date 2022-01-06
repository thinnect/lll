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

static uint16_t m_log_level;
static uint8_t m_buffer[512];

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

    uint32_t pos = 0;
    uint32_t len = (numargs * 32) / 8; // Need argument size in bytes
    uint32_t max_len = len + 4 + 2;

    va_list args;
    va_start(args, fmt);

    log_mutex_acquire();
    //start HDLC formating
    m_buffer[pos++] = HDLC_START_FLAG;

    uint8_t *current = (uint8_t*)&modid;
    for (int i = 1; i >= 0; i--)
    {
        //*(current + i)
        if ((*(current + i) == HDLC_CONTROL_ESCAPE) || (*(current + i) == HDLC_START_FLAG) || (*(current + i) == HDLC_STOP_FLAG))
        {
            m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
            *(current + i) ^= 0x20;
        }
        m_buffer[pos++] = *(current + i);
    }
    current = (uint8_t*)&__line__;
    for (int i = 1; i >= 0; i--)
    {
        //*(current + i)
        if ((*(current + i) == HDLC_CONTROL_ESCAPE) || (*(current + i) == HDLC_START_FLAG) || (*(current + i) == HDLC_STOP_FLAG))
        {
            m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
            *(current + i) ^= 0x20;
        }
        m_buffer[pos++] = *(current + i);
    }

    if (numargs != 0)
    {
        uint32_t arg = 0;
        for (int i = 0; i < numargs; i++)
        {
            arg = va_arg(args, int);
            uint8_t *p = (uint8_t*)&arg;
            for (int k = 0; k < 4; k++)
            {
                if ((*(p + k) == HDLC_CONTROL_ESCAPE) || (*(p + k) == HDLC_START_FLAG) || (*(p + k) == HDLC_STOP_FLAG))
                {
                    m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
                    *(p + k) ^= 0x20;
                }
                m_buffer[pos++] = *(p + k);
            }
        }
    }

    m_buffer[pos++] = HDLC_STOP_FLAG;

    mf_log_put((const char*)&m_buffer[0], max_len);

    log_mutex_release();
}

void __loggerb(uint16_t severity, uint16_t modid, uint16_t __line__,
               const char *fmt, const void *data, uint8_t len, uint16_t numargs, ...)
{
    if (!(m_log_level & severity))
    {
        return;
    }

    uint32_t pos = 0;
    uint32_t len_arg = (numargs * 32) / 8; // Need argument size in bytes
    uint32_t max_len = len_arg + len + 4 + 2;

    va_list args;
    va_start(args, fmt);
    log_mutex_acquire();
    //start HDLC formating
    m_buffer[pos++] = HDLC_START_FLAG;

    uint8_t *current = (uint8_t*)&modid;
    for (int i = 1; i >= 0; i--)
    {
        //*(current + i)
        if ((*(current + i) == HDLC_CONTROL_ESCAPE) || (*(current + i) == HDLC_START_FLAG) || (*(current + i) == HDLC_STOP_FLAG))
        {
            m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
            *(current + i) ^= 0x20;
        }
        m_buffer[pos++] = *(current + i);
    }
    current = (uint8_t*)&__line__;
    for (int i = 1; i >= 0; i--)
    {
        //*(current + i)
        if ((*(current + i) == HDLC_CONTROL_ESCAPE) || (*(current + i) == HDLC_START_FLAG) || (*(current + i) == HDLC_STOP_FLAG))
        {
            m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
            *(current + i) ^= 0x20;
        }
        m_buffer[pos++] = *(current + i);
    }

    if (numargs != 0)
    {
        uint32_t arg = 0;
        for (int i = 0; i < numargs; i++)
        {
            arg = va_arg(args, int);
            uint8_t *p = (uint8_t*)&arg;
            for (int k = 0; k < 4; k++)
            {
                if ((*(p + k) == HDLC_CONTROL_ESCAPE) || (*(p + k) == HDLC_START_FLAG) || (*(p + k) == HDLC_STOP_FLAG))
                {
                    m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
                    *(p + k) ^= 0x20;
                }
                m_buffer[pos++] = *(p + k);
            }
        }
    }

    if (len != 0)
    {
        uint8_t *ptr = NULL;
        for (int i = 0; i < len; i++)
        {
            ptr = (uint8_t *)data;
            if ((*(ptr + i) == HDLC_CONTROL_ESCAPE) || (*(ptr + i) == HDLC_START_FLAG) || (*(ptr + i) == HDLC_STOP_FLAG))
            {
                m_buffer[pos++] = HDLC_CONTROL_ESCAPE;
                *(ptr + i) ^= 0x20;
            }
            m_buffer[pos++] = *(ptr + i);
        }
    }

    m_buffer[pos++] = HDLC_STOP_FLAG;

    mf_log_put((const char*)&m_buffer[0], max_len);

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
