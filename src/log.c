#include "log.h"

void log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stdout, "[%s:%d] [INFO] ", __FILE__, __LINE__);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
}

void log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s:%d] [ERROR] ", __FILE__, __LINE__);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void log_warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s:%d] [WARN] ", __FILE__, __LINE__);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}