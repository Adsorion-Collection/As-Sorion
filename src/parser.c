#include <parser.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

__attribute__((unused))static uint32_t line_number;

void parser_error(char* message){
    printf("[ERROR][Parsing_Phase]: %s; On line: %d\n", message, line_number);
    exit(-1);
}

void free_line_array(char** line_array){
    for(uint32_t i = 0; i < 0x3; i++){
        free(line_array[i]);
    }
    free(line_array);
}

char** get_line_array(char* line){

    uint64_t line_len = strlen(line);
    uint64_t whitespace_count = 0;

    for(uint64_t i = 0; i <= line_len; i++){
        if(line[i] == ' ' || line[i] == '\0') whitespace_count++;
    }

    if(whitespace_count > 0x3){
        whitespace_count = 0x3;
    }

    char** line_array = (char**)malloc(0x3 * sizeof(char*));

    uint64_t word_index = 0;
    uint64_t whitespace_index = 0;
    for(uint64_t i = 0; i <= line_len; i++){
        if(line[i] != ' ' && line[i] != '\0' && line[i] != '\n') word_index++;
        else {
            if(line[i] == '\0' && whitespace_index == 0){
                line_array[0] = line;
                line_array[1] = NULL;
                line_array[2] = NULL;
                return line_array;
            }

            if(!word_index){
                continue;
            }
            line_array[whitespace_index] = (char*)malloc(word_index * sizeof(char) + 1);
            for(uint64_t j = 0; j < word_index; j++){
                line_array[whitespace_index][j] = line[i - word_index + j];
            }
            line_array[whitespace_index][word_index] = '\0';
            if(whitespace_index == 0x3){
                parser_error("Too many operands");
                free(line_array[whitespace_index]);
                break;
            }
            word_index = 0;
            whitespace_index++;
        }
    }

    if(whitespace_index == 0x2){
        line_array[2] = NULL;
    }

    return line_array;
}

uint16_t* gen_opcode_str(uint8_t opcode, addr_modes_e addr_mode, char** line_array){

    char* operand1_value_str;
    char* operand2_value_str;

    uint64_t operand1_value;
    uint64_t operand2_value;

    if(line_array[1] == NULL){
        operand1_value_str = NULL;
    }else{
        operand1_value_str = line_array[1] + 1;
    }

    if(line_array[2] == NULL){
        operand2_value_str = NULL;
    }else{
        operand2_value_str = line_array[2] + 1;
    }

    if(operand1_value_str != NULL){
        if(operand1_value_str[0] == 'x'){
            operand1_value_str++;
            operand1_value = strtol(operand1_value_str, NULL, 16);
        }else{
            operand1_value = atoi(operand1_value_str);
        }
    }
    if(operand1_value > UINT16_MAX){
        parser_error("Operand value too large to be supported");
    }

    if(operand2_value_str != NULL){
        if(operand2_value_str[0] == 'x'){
            operand2_value_str++;
            operand2_value = strtol(operand2_value_str, NULL, 16);
        }else{
            operand2_value = atoi(operand2_value_str);
        }
    }
    if(operand2_value > UINT16_MAX){
        parser_error("Operand value too large to be supported");
    }

    uint16_t* opcode_str = (uint16_t*)malloc(3 * sizeof(uint16_t));
    opcode_str[0] = (opcode << 8) | addr_mode;
    opcode_str[1] = (uint16_t)operand1_value;
    opcode_str[2] = (uint16_t)operand2_value;
    return opcode_str;
}

bool is_string_register(char* str){
    for(uint32_t i = 0; i < REGISTER_COUNT; i++){
        if(!strcmp(REG_strings[i], str)){
            return true;
        }
    }
    return false;
}

addr_modes_e get_addr_mode(char** line_array){

    if(line_array[1] == NULL && line_array[2] == NULL){
        return NOTHING;
    }

    if(line_array[2] == NULL){
        if(is_string_register(line_array[1])){
            return REG;
        }
    }

    switch(line_array[1][0]){
        case '$':{
            if(line_array[2][0] == '$'){
                return MEM_MEM;
            }else if(line_array[2][0] == '#'){
                return MEM_IMMEDIATE;
            }else if(is_string_register(line_array[2])){
                return MEM_REG;
            }
            break;
        }
    };

    if(is_string_register(line_array[1])){
        if(line_array[2][0] == '$'){
            return REG_MEM;
        }else if(line_array[2][0] == '#'){
            return REG_IMMEDIATE;
        }else if(is_string_register(line_array[2])){
            return REG_REG;
        }
    }

    parser_error("Couldn't get addressing mode");
    return NOTHING;
}

void parse_line(char* line, uint32_t line_nmbr){

    line_number = line_nmbr;

    char** line_array = get_line_array(line);

    instruction_t line_instruction = null_instruction;

    for(uint64_t i = 0; i < INSTRUCTIONS_COUNT; i++){
        if(!strcmp(line_array[0], instruction_set[i].name)){
            line_instruction = instruction_set[i];
            break;
        }
    }

    if(!strcmp(line_instruction.name, null_instruction.name)){
        parser_error("Unknown instruction");
    }

    addr_modes_e addr_mode = get_addr_mode(line_array);

    if(!(line_instruction.supported_addr_modes & addr_mode)){
        parser_error("Referenced addressing mode not supported by instruction");
    }

    uint16_t* opcode_str = gen_opcode_str(line_instruction.opcode, addr_mode, line_array);
    printf("%d\n", opcode_str[0]);
    printf("%d\n", opcode_str[1]);
    printf("%d\n", opcode_str[2]);

    free_line_array(line_array);

}