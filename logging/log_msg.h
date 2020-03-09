/**
 * @author Andrei Lunjov, Raido Pahtma
 * @license MIT
*/
#ifndef LOG_MSG_H
#define LOG_MSG_H

#include "lll_log.h"

#if defined(__LOG_LEVEL__) && __LOG_LEVEL__!=0
	#define loggerm(severity, str, msg, args...) 		({	\
		if( (severity) & __LOG_LEVEL__){	\
			STATIC_CONST char moduulROM[] PROGMEM = __MODUUL__;	\
			STATIC_CONST char strROM[] PROGMEM = str;	\
			__loggerb( severity, moduulROM, __LINE__, strROM, msg , sizeof(message_t) , ##args  );	\
		}	\
	})
#else
	#define loggerm(severity, str, msg, args...)
#endif

#define debugm(str, msg, args...)  loggerm( LOG_DEBUG2, str, msg , ##args )
#define debugm1(str, msg, args...) loggerm( LOG_DEBUG1, str, msg , ##args )
#define debugm2(str, msg, args...) loggerm( LOG_DEBUG2, str, msg , ##args )
#define debugm3(str, msg, args...) loggerm( LOG_DEBUG3, str, msg , ##args )
#define debugm4(str, msg, args...) loggerm( LOG_DEBUG4, str, msg , ##args )

#define infom(str, msg, args...)   loggerm( LOG_INFO2, str, msg , ##args )
#define infom1(str, msg, args...)  loggerm( LOG_INFO1, str, msg , ##args )
#define infom2(str, msg, args...)  loggerm( LOG_INFO2, str, msg , ##args )
#define infom3(str, msg, args...)  loggerm( LOG_INFO3, str, msg , ##args )
#define infom4(str, msg, args...)  loggerm( LOG_INFO4, str, msg , ##args )

#define warnm(str, msg, args...)   loggerm( LOG_WARN2, str, msg , ##args )
#define warnm1(str, msg, args...)  loggerm( LOG_WARN1, str, msg , ##args )
#define warnm2(str, msg, args...)  loggerm( LOG_WARN2, str, msg , ##args )
#define warnm3(str, msg, args...)  loggerm( LOG_WARN3, str, msg , ##args )
#define warnm4(str, msg, args...)  loggerm( LOG_WARN4, str, msg , ##args )

#define logerrm(str, msg, args...) loggerm( LOG_ERROR2, str, msg , ##args )
#define errm1(str, msg, args...)   loggerm( LOG_ERROR1, str, msg , ##args )
#define errm2(str, msg, args...)   loggerm( LOG_ERROR2, str, msg , ##args )
#define errm3(str, msg, args...)   loggerm( LOG_ERROR3, str, msg , ##args )
#define errm4(str, msg, args...)   loggerm( LOG_ERROR4, str, msg , ##args )

void __loggerm( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, message_t* msg, ... );

#endif // LOG_MSG_H
