#ifndef __PARSER_H
#define __PARSER_H

#include <stdint.h>
#include <arch/instruction.h>
#include <stdbool.h>

typedef struct{
    char* name;
    uint16_t address;
} label_t;

typedef struct{
    char* name;
    uint16_t value;
} definition_t;

extern label_t labels[0x400];
extern definition_t definitions[0x400];

extern uint16_t label_index;
extern uint16_t definition_index;

void parser_error(char* message);
void preprocessor_error(char* message);

void free_line_array(char** line_array);
char** get_line_array(char* line);

uint16_t* gen_opcode_str(uint8_t opcode, addr_modes_e addr_mode, char** line_array);

bool is_string_register(char* str);
addr_modes_e get_addr_mode(char** line_array);

void preprocess_line(char* line, uint32_t line_nmbr);
void parse_line(char* line, uint32_t line_number);

#endif