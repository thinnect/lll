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

#include "hdlc_tools.h"

#define MAX_ARG_COUNT 8
#define ARG_BUFFER_SIZE (MAX_ARG_COUNT * 8)
#define MAX_DATA_SIZE (ARG_BUFFER_SIZE + 4)
#define MAX_LOG_PACKET_SIZE (MAX_DATA_SIZE + 2)

#define LIST_OF_MODULES {"main","radio"}
#define LIST_OF_MOD_ID {0x0 , 0x1}
#define NUMBER_OF_MODULES 2

static uint16_t m_log_level;
static uint8_t m_log_buffer[256];

static int (*mf_log_put)(const char *, int);
static uint32_t (*mf_log_time)(void);
static platform_mutex_t m_log_mutex;

static int log_severity_char(uint16_t severity);
static void log_mutex_acquire(void);
static void log_mutex_release(void);

typedef enum {
	MAIN_MODULE = 0x0,
	RADIO_MODULE,
} module_id_t;


uint16_t get_module_id(const char* moduul)
{
	char* str_module = LIST_OF_MODULES;
	uint8_t mod_id = LIST_OF_MOD_ID;
	for(int i = 0; i < NUMBER_OF_MODULES; i++)
	{
		if(strcmp(moduul, str_module[i]))
		{
			return mod_id[i];
		}
	}
}

void log_init(uint16_t loglevel, int (*log_fun)(const char *, int), uint32_t (*time_fun)(), platform_mutex_t mutex)
{
	mf_log_put = log_fun;
	mf_log_time = time_fun;
	m_log_level = loglevel;
	m_log_mutex = mutex;

	if (NULL != mf_log_time)
	{
		uint32_t ts = mf_log_time();
		log_mutex_acquire();
		mf_log_put(m_log_buffer, snprintf(m_log_buffer, sizeof(m_log_buffer), "%08" PRIx32 " B|BOOT\n", ts));
		log_mutex_release();
	}
}

void __logger(uint16_t severity, const char* moduul, uint16_t __line__, const char *fmt, ...)
{
	if (!(m_log_level & severity))
	{
		return;
	}

	uint16_t mod_id = get_module_id(moduul);

    va_list arg;
    va_start(arg, fmt);

    uint8_t args[ARG_BUFFER_SIZE] = {0};
    uint8_t pos = 0;

    while (*fmt)
    {
        if (*fmt == '%')
        {
            char str = *fmt;
            bool has_l_modifer = false;
            bool has_ll_modifer = false;
            if (*(fmt + 1) == 'l')
            {
                if (*(fmt + 2) == 'l')
                {
                    has_ll_modifer = true;
                    fmt += 2;
                }
                else
                {
                    has_l_modifer = true;
                    fmt++;
                }
            }
            switch (*(++fmt))
            {
            case 'u':
            case 'x':
            {
                if (has_l_modifer)
                {
                    uint32_t x = va_arg(arg, uint32_t);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                else if (has_ll_modifer)
                {
                    uint64_t x = va_arg(arg, uint64_t);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                else
                {
                    uint16_t x = va_arg(arg, unsigned int);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                break;
            }
            case 'i':
            case 'd':
            {
                if (has_l_modifer)
                {
                    int32_t x = va_arg(arg, long int);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                else if (has_ll_modifer)
                {
                    int64_t x = va_arg(arg, long long int);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                else
                {
                    int16_t x = va_arg(arg, int);
                    memcpy(&args[pos],&x,sizeof(x));
                    pos += sizeof(x);
                }
                break;
            }
            case 'f':
            {
                double x = va_arg(arg, double);
                memcpy(&args[pos],&x,sizeof(x));
                pos += sizeof(x);
                break;
            }
            }
        }
        fmt++;
    }
    va_end(arg);


    uint8_t data[MAX_DATA_SIZE] = {0};
    data[0] = mod_id & 0xFF00; //Module ID 
    data[1] = mod_id & 0x00FF; //Module ID
    data[2] = __line__ & 0xFF00; //line nr 
    data[3] = __line__ & 0x00FF;//line nr
    if(pos != 0) // if no arguments no point of copying the args array
    {
        memcpy(&data[4],&args[0],pos);
    }
    uint8_t total_size = 4 + pos;
    uint8_t log_packet[MAX_LOG_PACKET_SIZE] = {0};
    int status = hdlc_encode(log_packet,2+total_size,data,total_size);
	log_mutex_acquire();
	if(status != -1) 
	{
		mf_log_put(m_log_buffer, 2+total_size);
	}
	log_mutex_release();
}

void __loggerb(uint16_t severity, uint16_t moduul, uint16_t __line__,
			   const char *fmt, const void *data, uint8_t len, ...)
{
	if (!(m_log_level & severity))
	{
		return;
	}

	mf_log_put(m_log_buffer, l + 1);
	log_mutex_release();
}

static int log_severity_char(uint16_t severity)
{
	if (severity & LOG_ERR1)
		return ('E');
	if (severity & LOG_ERR2)
		return ('E');
	if (severity & LOG_ERR3)
		return ('E');
	if (severity & LOG_ERR4)
		return ('E');
	if (severity & LOG_WARN1)
		return ('W');
	if (severity & LOG_WARN2)
		return ('W');
	if (severity & LOG_WARN3)
		return ('W');
	if (severity & LOG_WARN4)
		return ('W');
	if (severity & LOG_INFO1)
		return ('I');
	if (severity & LOG_INFO2)
		return ('I');
	if (severity & LOG_INFO3)
		return ('I');
	if (severity & LOG_INFO4)
		return ('I');
	if (severity & LOG_DEBUG1)
		return ('D');
	if (severity & LOG_DEBUG2)
		return ('D');
	if (severity & LOG_DEBUG3)
		return ('D');
	if (severity & LOG_DEBUG4)
		return ('D');
	return ('X');
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
