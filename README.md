# LowLevelLogging

The LLL library provides a set of logging functions (macros) for use in mostly
embedded C development that allow fine-grained selection of active logging
messages at compile time. In each C file it allows the use of 4 individually
selectable logging levels in 4 categories - debug, info, warning, error.

## Using the logging functions

To use the logging functions the C file needs to define the loglevel and module
name. Commonly the following setup is used:
```
#include "loglevels.h"
#define __MODUUL__ "module_name"
#define __LOG_LEVEL__ ( LOG_LEVEL_filename & BASE_LOG_LEVEL )
#include "log.h"
```

Loglevels are set in a project-specific loglevels.h file, the file-specific
loglevel is named LOG_LEVEL_filename, where filename is the name of the file
without the .c extension. The __MODUUL__ is a short name for the file, it should
be relatively unique and recognizable, 4-6 characters usually. If __MODUUL__ is
not defined, the name of the file is used, but this is usually rather long and
therefore inefficient or trunkated (depending on configuration). Additionally
a global BASE_LOG_LEVEL is defined that allows filtering based on the 4x4 log
levels.

Newlines are added automatically by all logging functions.

## Log level
The severity argument of logging functions sets the log level of the message:
```
LOG_DEBUG4
LOG_DEBUG3
LOG_DEBUG2
LOG_DEBUG1

LOG_INFO4
LOG_INFO3
LOG_INFO2
LOG_INFO1

LOG_WARN4
LOG_WARN3
LOG_WARN2
LOG_WARN1

LOG_ERR4
LOG_ERR3
LOG_ERR2
LOG_ERR1
```

## logger

The principal logging function is `logger(severity, fmt, args...)`, taking a
printf format string and a matching number of variable arguments.

The following convenience functions have been defined:
```
debug1, debug2, debug3, debug4
info1, info2, info3, info4
warn1, warn2, warn3, warn4
err1, err2, err3, err4
```

## loggerb

A logging function `loggerb(severity, fmt, data[], len, args...)` and
corresponding convenience functions exist for logging hex buffers. The buffer
pointed to by data and len is logged out after the logging message in sets of
4 bytes (8 symbols).

## loglevels.h
Loglevels are usually set in a project-specific loglevels.h file, specifying
a log-level for each file. If a file is not listed, then any logging messages
in the file are ignored.

See the example in [test/std/loglevels.h](test/std/loglevels.h).

## Supported platforms

### TinyOS native

`loggers_hw.c`

This implementation writes the logging message into the output stream as it is
processed using putchar and printf. Assumes that the platform has been set up
so that putchar and printf already work. Logging is assumed to happen from
task context so no synchronization is implemented. When used with an AVR
microcontroller, the log strings are automatically stored in PROGMEM to minimize
RAM usage.

### TinyOS simulator TOSSIM

`loggers_sim.c`

This implementation prepares the whole logging message using vsnprintf and
passes the final message to the TOSSIM dbg system with the channel name
"dbgchannel".

### EXT

`loggers_ext.c`

This implementation prepares the whole logging message using vsnprintf and
passes the final message to a user provided function that is expected to be
thread safe. Intended for use on top of RTOS (CMSIS) and LDMA loggers, but can
be easily connected to fwrite and such.

### Linux

`loggers_std.c`
This implementation prepares the whole logging message using vsnprintf and
passes the final message to printf. It uses C stdlib functions to obtain the
current time and prepend it to each message.

## Building

A platform specific implementation of the logger needs to be compiled and linked
into the final binary, see supported platforms and the example in the
[test/std](test/std) directory.
