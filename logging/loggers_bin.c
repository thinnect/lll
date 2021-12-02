#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <inttypes.h>

#include "loggers_bin.h"

#include "__loggers.h"

#include "platform_mutex.h"

#define BIN_FRAME_FLAG_SOF		    0x7e   // Flag
#define BIN_FRAME_CONTROL_ESCAPE 	0x7d   // Control Escape octet
#define BIN_FRAME_ESCAPE_BIT     	0x20   // Transparency modifier octet (XOR bit)


static uint16_t m_log_level;
static uint8_t m_log_buffer[256];

static int(*mf_log_put)(const char*, int);
static uint32_t(*mf_log_time)(void);
static platform_mutex_t m_log_mutex;


static int log_severity_char (uint16_t severity);
static void log_mutex_acquire (void);
static void log_mutex_release (void);


void log_init (uint16_t loglevel, int(*log_fun)(const char*, int), uint32_t(*time_fun)(), platform_mutex_t mutex)
{
	mf_log_put = log_fun;
	mf_log_time = time_fun;
	m_log_level = loglevel;
	m_log_mutex = mutex;

	if (NULL != mf_log_time)
	{
		uint32_t ts = mf_log_time();
		log_mutex_acquire();
		mf_log_put(m_log_buffer, snprintf(m_log_buffer, sizeof(m_log_buffer), "%08"PRIx32" B|BOOT\n", ts));
		log_mutex_release();
	}
}


void __logger (uint16_t severity, uint16_t moduul, uint16_t __line__, const char* fmt, ...)
{
	if ( ! (m_log_level & severity))
	{
		return;
	}

    

	va_list arg;
	va_start(arg, fmt);
	
    while(*fmt)
    {
        switch(*(++fmt)) 
        {
            case 'x':
            {
                break;
            }
            case 'd':
            {
                break;
            }
            case 'u':
            {
                break;
            }
        }
    }

	va_end(arg);

	mf_log_put(m_log_buffer, l+1);
	log_mutex_release();
}


void __loggerb (uint16_t severity, uint16_t moduul, uint16_t __line__,
                const char* fmt, const void *data, uint8_t len, ...)
{
	if ( ! (m_log_level & severity))
	{
		return;
	}



	
	mf_log_put(m_log_buffer, l+1);
	log_mutex_release();
}


static int log_severity_char (uint16_t severity)
{
	if (severity & LOG_ERR1) return ('E');
	if (severity & LOG_ERR2) return ('E');
	if (severity & LOG_ERR3) return ('E');
	if (severity & LOG_ERR4) return ('E');
	if (severity & LOG_WARN1) return ('W');
	if (severity & LOG_WARN2) return ('W');
	if (severity & LOG_WARN3) return ('W');
	if (severity & LOG_WARN4) return ('W');
	if (severity & LOG_INFO1) return ('I');
	if (severity & LOG_INFO2) return ('I');
	if (severity & LOG_INFO3) return ('I');
	if (severity & LOG_INFO4) return ('I');
	if (severity & LOG_DEBUG1) return ('D');
	if (severity & LOG_DEBUG2) return ('D');
	if (severity & LOG_DEBUG3) return ('D');
	if (severity & LOG_DEBUG4) return ('D');
	return('X');
}


static void log_mutex_acquire (void)
{
	if (NULL != m_log_mutex)
	{
		platform_mutex_acquire(m_log_mutex);
	}
}


static void log_mutex_release (void)
{
	platform_mutex_release(m_log_mutex);
}
