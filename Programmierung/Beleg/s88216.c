#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdarg.h>
#include <unistd.h>
#define _GNU_SOURCE
#include "list.h"
#include "media.h"
#include "web.h"
#include "logger.h"
#include "utility.h"
#include "config.h"

// Variable for the log file
FILE *ProtFile;

int main (int argc, char *argv[], char*env[]) {
    puts("Content-Type: text/html\r\n\r\n");

    // Check if the html file exists
    if (access(html_path, F_OK) != 0) {
        error_msg("HTML file not found.\n");
    }
    // Check if the html file is readable
    if (access(html_path, R_OK) != 0) {
        error_msg("HTML file not found or not readable.\n");
    }
        
    // Create the media.csv file if it doesnt exist
    if (access(media_path, F_OK) != 0) {
        FILE *file = fopen(media_path, "w+");
        if (file) fclose(file);
    }
    // Check if the media.csv file is readable and writable
    if (access(media_path, R_OK) != 0) {
        error_msg("Media file not found or not readable.\n");
    }
    if (access(media_path, W_OK) != 0) {
        error_msg("Media file not writable.\n");
    }

    char *request_method = getenv("REQUEST_METHOD");
    // char *request_method = strdup("POST");

    if (request_method == NULL) {
        error_msg("No request method found.");
    }

    if (strcmp(request_method, "GET") == 0) {
        handle_get_request();
    }
    else if (strcmp(request_method, "POST") == 0) {
        handle_post_request();
    }
    else {
        error_msg("Error: No valid request method found.\n");
    }

    return 0;
}