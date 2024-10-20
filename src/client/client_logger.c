/***************************************************************************
 *   FILENAME : client_logger.c
 *   Owner : Group 3            Date : 17/10/24
 *
 *   DESCRIPTION: This file defines the log_message function, which logs messages to the client.log file with a timestamp 
                  and severity level (DEBUG, INFO, WARN, FATAL). 
 *
 *   REVISION HISTORY:
 *
 *   Name : Pranavya Deepthi Dachepalli, Shrishti Maheshwari             Date : 17/10/24
 *   Reason : For displaying log mesages
 *
 *   Name : Priyanka Solanki, Shaista Parveen       Date : 18/10/24
 *   Reason : modified code
 *
 ***************************************************************************/

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
