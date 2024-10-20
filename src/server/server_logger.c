/***************************************************************************
 *   FILENAME : server_logger.c
 *   Owner : Group 3            Date : 15/10/24
 *
 *   DESCRIPTION: This file implements the log_message function, which logs messages to a file with a timestamp, 
 *                log level (DEBUG, INFO, WARN, FATAL), and custom message format.
 *				  It ensures error handling for file opening and null filename inputs. 
 *
 *   REVISION HISTORY:
 *
 *   Name : Shaista Parveen, Priyanka Solanki             Date : 17/10/24
 *   Reason : integrating logger file and error handling.
 *
 ***************************************************************************/

#include "/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/include/logger.h"

void log_message(LogLevel level, const char *filename, const char *fmt, ...) {
 // Check if the filename is NULL
    if (filename == NULL) {
        fprintf(stderr, "Error: NULL filename passed to log_message\n");
        return;
    }
	 // Open the log file
 FILE *logfile = fopen("/home2/user15/GROUP3_SPRINT_EXECUTION/GROUP3_SPRINT_EXECUTION/log/server.log", "a");
     if (!logfile) {
        fprintf(stderr, "Error: Could not open log file for writing\n");
        return;
    }
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
