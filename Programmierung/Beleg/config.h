#ifndef CONFIG_H
#define CONFIG_H

    // #define DEBUG

    #ifdef _WIN32
        #define media_path ".\\media.csv"
        #define html_path ".\\index.html"
    #elif __linux__
        #define media_path "./media.csv"
        #define html_path "./index.html"
    #endif

    #define DELIMITER ";"

#endif