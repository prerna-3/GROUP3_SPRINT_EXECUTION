#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"

void log_message(LogLevel level, const char *filename, const char *fmt, ...) {
    FILE *logfile = fopen("/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/log/client.log", "a");
    if (!logfile) return;

    time_t now = time(NULL);
    struct tm *tstruct = localtime(&now);
    char time_buffer[80];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %X", tstruct);

    const char *level_strings[] = { "DEBUG", "INFO", "WARN", "FATAL" };

    fprintf(logfile, "[%s] [%s] ", time_buffer, level_strings[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);

    fprintf(logfile, "\n");
    fclose(logfile);
}

