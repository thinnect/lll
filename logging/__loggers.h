/**
 * @author Andrei Lunjov, Raido Pahtma
 * @license MIT
*/
#ifndef __LOGGERS_H
#define __LOGGERS_H

#ifndef MODULE_NAME_LENGTH
#define MODULE_NAME_LENGTH 12
#endif // MODULE_NAME_LENGTH

#define LOG_DEBUG4       0x0001
#define LOG_DEBUG3       0x0002
#define LOG_DEBUG2       0x0004
#define LOG_DEBUG1       0x0008

#define LOG_INFO4        0x0010
#define LOG_INFO3        0x0020
#define LOG_INFO2        0x0040
#define LOG_INFO1        0x0080

#define LOG_WARN4        0x0100
#define LOG_WARN3        0x0200
#define LOG_WARN2        0x0400
#define LOG_WARN1        0x0800

#define LOG_ERR4         0x1000
#define LOG_ERR3         0x2000
#define LOG_ERR2         0x4000
#define LOG_ERR1         0x8000

#define LOG_LEVEL_DEBUG  0xFFFF
#define LOG_LEVEL_INFO   0xFFF0
#define LOG_LEVEL_WARN   0xFF00
#define LOG_LEVEL_ERR    0xF000

#define LOG_LEVEL_ASSERT LOG_ERR4

#define LOG_MASK_DEBUG   0x000F
#define LOG_MASK_INFO    0x00F0
#define LOG_MASK_WARN    0x0F00
#define LOG_MASK_ERR     0xF000

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PGM_P const char*
#define PROGMEM
#endif

int printfflush();
uint32_t _getLocalTimeRadio();

void putsr( PGM_P str, uint8_t len );

void __loghead( uint16_t severity, PGM_P _file_, unsigned _line_ );

void __logmem( void* _data, uint8_t len );

void __vloggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a );
void __loggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... );
void __vlogger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a );
void __logger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... );
void __nanologger(PGM_P format, ... );
void __vloggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, const void* data, uint8_t len, va_list a);
void __loggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, const void* data, uint8_t len, ...);
void __loggerb2( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str1, const void* data1, uint8_t len1, PGM_P str2, void* data2, uint8_t len2, ...);

#endif // __LOGGERS_H
