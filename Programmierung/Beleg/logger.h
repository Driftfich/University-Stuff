/*
* Author: Franz Rehschuh
* Date: 2025-02-25
* Description: Header file for a simple logging system, allowing to log strings,
* integers, and floats to a file together with metadata (file name, line number, function name and time).
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// if DEBUG flag is defined, logging is enabled
#ifdef DEBUG 
    extern FILE *ProtFile;

    #define FNAME "./log.txt"

    // Macro to get the current time as a string
    #define TIMESTR asctime(localtime(&(time_t){time(NULL)}))

    #define DEBUG_STR(Str) { \
        if (!ProtFile) ProtFile=fopen(FNAME, "w"); \
        fprintf(ProtFile, "[%s:%d] %s() - %s:\t%s", __FILE__, __LINE__, __func__, TIMESTR, Str); \
        fclose(ProtFile); \
        ProtFile=fopen(FNAME, "a"); \
    }

    #define DEBUG_INT(Int) { \
        if (!ProtFile) ProtFile=fopen(FNAME, "w"); \
        fprintf(ProtFile, "[%s:%d] %s() - %s:\t%d", __FILE__, __LINE__, __func__, TIMESTR, Int); \
        fclose(ProtFile); \
        ProtFile=fopen(FNAME, "a"); \
    }

    #define DEBUG_FLOAT(Float) { \
        if (!ProtFile) ProtFile=fopen(FNAME, "w"); \
        fprintf(ProtFile, "[%s:%d] %s() - %s:\t%f", __FILE__, __LINE__, __func__, TIMESTR, Float); \
        fclose(ProtFile); \
        ProtFile=fopen(FNAME, "a"); \
    }
#else
    #define DEBUG_STR(Str)
    #define DEBUG_INT(Int)
    #define DEBUG_FLOAT(Float)
#endif

