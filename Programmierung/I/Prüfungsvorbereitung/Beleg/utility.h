/*
* Author: Franz Rehschuh
* Date: 2025-02-25
* Description: Header file for utility functions used in multiple files.
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef UTILITY_H
#define UTILITY_H

int hex_to_int(char c);
char *format_string(char *format,...);
void error_msg(char *msg);

#endif