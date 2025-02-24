#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include "utility.h"
#include "logger.h"

int hex_to_int(char c) {
    // Convert hex to int based on ascii table
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
    // format a string with the given format and arguments
    char *result = NULL;
    // creating argument list
    va_list args;
    va_start(args, format);
    // creating the string
    if (vasprintf(&result, format, args) == -1) {
        return NULL;
    }
    va_end(args);
    return result;
}

void error_msg(char *msg) {
    // print out the error message to stdout and to the log file and exit
    if (msg) {
        char *err_msg = format_string("Error: %s", msg);
        DEBUG_STR(err_msg);
        puts(err_msg);
        free(err_msg);
    }
    exit(1);
}