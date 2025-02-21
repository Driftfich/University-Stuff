#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "utility.h"
#include "logger.h"

int hex_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

char *format_string(char *format,...) {
    char *result = NULL;
    va_list args;
    va_start(args, format);
    vasprintf(&result, format, args);
    va_end(args);
    return result;
}

void error_msg(char *msg) {
    if (msg) {
        char *err_msg = format_string("Error: %s", msg);
        DEBUG_STR(err_msg);
        puts(err_msg);
        free(err_msg);
    }
    exit(1);
}