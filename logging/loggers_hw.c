/**
 * @author Andrei Lunjov, Raido Pahtma
 * @license MIT
*/

#include "__loggers.h"

//#include <message.h>

int printfflush();
uint32_t _getLocalTimeRadio();


void putsr( PGM_P str, uint8_t len )
{
	uint8_t slen = strlen_P( str );

	while( len > slen ) {
		putchar(' ');
		--len;
	}

	while( len-- )
		putchar( pgm_read_byte(str++) );
}

void __loghead( uint16_t severity, PGM_P _file_, unsigned _line_ )
{
	PGM_P file;

	#ifdef SD_CARD_LOGGER
		printf_P( PSTR("%08lX|"), _getLocalTimeRadio() );
	#endif

	if( severity & LOG_MASK_ERR )
		putchar( 'E' );
	else if( severity & LOG_MASK_WARN )
		putchar( 'W' );
	else if( severity & LOG_MASK_INFO )
		putchar( 'I' );
	else if( severity & LOG_MASK_DEBUG )
		putchar( 'D' );

	putchar( '|' );

	file = strrchr_P( _file_, '/' );
	if( file )
		++ file;
	else
		file = _file_;
	putsr( file, MODULE_NAME_LENGTH );

	putchar( ':' );
	printf_P( PSTR("%4d"), _line_ );
	putchar( '|' );
}

void __logmem( const void* _data, uint8_t len )
{
	uint8_t* data = (uint8_t*)_data;
	int i;
	for( i=0; i<len; ++i ) {
		if( ! (i%4) )
			putchar(' ');
		printf_P( PSTR("%02X"), *data++ );
	}
}

void __vloggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a )
{
	__loghead( severity, moduul, __line__ );
	vfprintf_P( stdout, format, a );
}

void __loggernnl( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... )
{
	va_list a;

	va_start( a, format );
	__vloggernnl( severity, moduul, __line__, format, a );
	va_end( a );
}


void __vlogger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, va_list a )
{
	__vloggernnl( severity, moduul, __line__, format, a );

	putchar('\n');
	printfflush();
}

void __logger( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P format, ... )
{
	va_list a;

	va_start( a, format );
	__vlogger( severity, moduul, __line__, format, a );
	va_end( a );
}

void __nanologger(PGM_P format, ... )
{
	va_list a;

	va_start( a, format );
	vfprintf_P( stdout, format, a );
	putchar('\n');
	printfflush();
	va_end( a );
}

void __vloggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, void* data, uint8_t len, va_list a)
{
	__loghead( severity, moduul, __line__ );

	vfprintf_P( stdout, str, a );

	__logmem(data, len);
	putchar('\n');
	printfflush();
}

void __loggerb( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str, void* data, uint8_t len, ...)
{
	va_list a;

	va_start( a, len );
	__vloggerb( severity, moduul, __line__, str, data, len, a );
	va_end( a );
}

void __loggerb2( uint16_t severity, PGM_P moduul, uint16_t __line__, PGM_P str1, const void* data1, uint8_t len1, PGM_P str2, const void* data2, uint8_t len2, ...)
{
	va_list a;

	va_start( a, len2 );
	__loghead( severity, moduul, __line__ );

	vfprintf_P( stdout, str1, a );
	__logmem(data1, len1);
	vfprintf_P( stdout, str2, a );
	__logmem(data2, len2);

	putchar('\n');
	printfflush();
	va_end( a );
}
