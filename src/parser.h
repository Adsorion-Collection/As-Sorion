#ifndef __PARSER_H
#define __PARSER_H

#include <stdint.h>
#include <arch/instruction.h>
#include <stdbool.h>

void parser_error(char* message);

void free_line_array(char** line_array);
char** get_line_array(char* line);

uint16_t* gen_opcode_str(uint8_t opcode, addr_modes_e addr_mode, char** line_array);

bool is_string_register(char* str);
addr_modes_e get_addr_mode(char** line_array);
void parse_line(char* line, uint32_t line_number);

#endif