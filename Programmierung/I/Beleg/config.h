/*
Author: Franz Rehschuh
Date: 2025-02-25

Description: Configuration file containing paths and settings for the project.
*/

#ifndef CONFIG_H
#define CONFIG_H

    // uncomment next line to enable debug mode permanently for all compilations
    // #define DEBUG

    // change paths for specific OS if needed
    #ifdef _WIN32
        #define media_path ".\\media.csv"
        #define html_path ".\\index.html"
    #elif __linux__
        #define media_path "./media.csv"
        #define html_path "./index.html"
    #endif

    // media.csv file delimiter
    #define DELIMITER ","

    // Choose negative value to disable limit for user inputs
    #define MAX_INPUT_LENGTH 800

    // Maximum length of the read in post data, to avoid buffer overflow. Choose negative value to disable limit
    #define MAX_POSTD_LENGTH 32000

#endif