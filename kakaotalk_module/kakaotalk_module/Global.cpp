#include "Global.h"

void write_log(const char* format, ...) {
#ifdef DEBUG_MODE
    time_t curr_time = time(NULL);
    struct tm* pLocal = localtime(&curr_time);

    char sDateTime[1024];
    sprintf_s(sDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
        pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
        pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);

    fprintf(log_out, "[%s] ", sDateTime);

    va_list ap;
    va_start(ap, format);
    vfprintf(log_out, format, ap);
    va_end(ap);
#else
#endif
}

void write_log(const wchar_t* format, ...) {
#ifdef DEBUG_MODE
    time_t curr_time = time(NULL);
    struct tm* pLocal = localtime(&curr_time);

    char sDateTime[1024];
    sprintf_s(sDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
        pLocal->tm_year + 1900, pLocal->tm_mon + 1, pLocal->tm_mday,
        pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);

    fprintf(log_out, "[%s] ", sDateTime);

    va_list ap;
    va_start(ap, format);
    vfwprintf(log_out, format, ap);
    va_end(ap);
#else
#endif
}