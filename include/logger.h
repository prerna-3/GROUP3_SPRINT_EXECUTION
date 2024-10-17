#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

typedef enum {
    DEBUG,
    INFO,
    WARN,
    FATAL
} LogLevel;

void log_message(LogLevel level, const char *filename, const char *fmt, ...);

#endif

