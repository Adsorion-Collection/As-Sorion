#ifndef __PARSER_H
#define __PARSER_H

#include <stdint.h>

void free_line_array(char** line_array);
char** get_line_array(char* line);

addr_modes_e get_addr_mode(char** line_array);
void parse_line(char* line);

#endif