#include <parser.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static uint32_t line_number;
label_t labels[0x400];
definition_t definitions[0x400];

uint16_t label_index = 0;
uint16_t definition_index = 0;

void parser_error(char* message){
    printf("[ERROR][Parsing_Phase]: %s; On line: %d\n", message, line_number);
    exit(-1);
}

void preprocessor_error(char* message){
    printf("[ERROR][Preprocessing_Phase]: %s; On line: %d\n", message, line_number);
    exit(-1);
}

void free_line_array(char** line_array){
    for(uint32_t i = 0; i < 0x3; i++){
        if(line_array[i] != NULL){
            free(line_array[i]);
        }
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
                line_array[0] = (char*)malloc(strlen(line));
                strcpy(line_array[0], line);
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
    
    if(is_string_definition(line_array[1])){
        operand1_value = get_definition(line_array[1]).value;
    }else if(is_string_label(line_array[1])){
        operand1_value = get_label(line_array[1]).address;
    }else{
        if(operand1_value_str != NULL){
            if(operand1_value_str[0] == 'x'){
                operand1_value_str++;
                operand1_value = strtol(operand1_value_str, NULL, 16);
            }else{
                operand1_value = atoi(operand1_value_str);
            }
        }else{
            parser_error("Invalid instruction addressing mode");
        }
    
        if(operand1_value > UINT16_MAX){
            parser_error("Operand value too large to be supported");
        }
    }

    if(is_string_definition(line_array[2])){
        operand2_value = get_definition(line_array[2]).value;
    }else if(is_string_label(line_array[2])){
        operand2_value = get_label(line_array[2]).address;
    }else{
        if(operand2_value_str){
            if(operand2_value_str[0] == 'x'){
                operand2_value_str++;
                operand2_value = strtol(operand2_value_str, NULL, 16);
            }else{
                operand2_value = atoi(operand2_value_str);
            }
        }else{
            parser_error("Invalid instruction addressing mode");
        }

        if(operand2_value > UINT16_MAX){
            parser_error("Operand value too large to be supported");
        }
    }


    uint16_t* opcode_str = (uint16_t*)malloc(3 * sizeof(uint16_t));
    opcode_str[0] = (opcode << 8) | addr_mode;
    opcode_str[1] = (uint16_t)operand1_value;
    opcode_str[2] = (uint16_t)operand2_value;
    if(addr_mode == REG || addr_mode == REG_IMMEDIATE || addr_mode == REG_MEM){
        opcode_str[1] = (uint16_t)(line_array[1][strlen(line_array[1]) - 1] - '0');
    }
    if(addr_mode == REG_REG){
        opcode_str[1] = (uint16_t)(line_array[1][strlen(line_array[1]) - 1] - '0');
        opcode_str[2] = (uint16_t)(line_array[2][strlen(line_array[2]) - 1] - '0');
    }
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

bool is_string_label(char* str){
    for(uint32_t i = 0; i < label_index; i++){
        if(!strcmp(labels[i].name, str)){
            return true;
        }
    }
    return false;
}

bool is_string_definition(char* str){
    for(uint32_t i = 0; i < definition_index; i++){
        if(!strcmp(definitions[i].name, str)){
            return true;
        }
    }
    return false;
}

label_t get_label(char* str){
    label_t null_label = {.name = NULL, .address = 0x0};

    for(uint32_t i = 0; i < label_index; i++){
        if(!strcmp(labels[i].name, str)){
            return labels[i];
        }
    }
    return null_label;
}

definition_t get_definition(char* str){
    definition_t null_definition = {.name = NULL, .value = 0x0};

    for(uint32_t i = 0; i < definition_index; i++){
        if(!strcmp(definitions[i].name, str)){
            return definitions[i];
        }
    }
    return null_definition;
}

addr_modes_e get_addr_mode(char** line_array){

    if(line_array[2] == NULL){
        if(line_array[1] == NULL){
            return NOTHING;
        }
        if(is_string_register(line_array[1])){
            return REG;
        }
    }

    switch(line_array[1][0]){
        case '$':{
            if(is_string_label(line_array[2])){
                parser_error("MEM_MEM addressing mode not supported");
            }else if(is_string_definition(line_array[2])){
                return MEM_IMMEDIATE;
            }

            if(line_array[2] == NULL){
                return MEM;
            }
            
            if(line_array[2][0] == '$'){
                parser_error("MEM_MEM addressing mode not supported");
            }else if(line_array[2][0] == '#'){
                return MEM_IMMEDIATE;
            }else if(is_string_register(line_array[2])){
                return MEM_REG;
            }
            break;
        }
    };

    if(is_string_register(line_array[1])){

        if(is_string_label(line_array[2])){
            return REG_MEM;
        }else if(is_string_definition(line_array[2])){
            return REG_IMMEDIATE;
        }

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

void preprocess_line(char* line, uint32_t line_nmbr){
    line_number = line_nmbr;
    char** line_array = get_line_array(line);

    if(line_array[0] == NULL){
        preprocessor_error("Error creating line array");
    }

    uint32_t word1_len = strlen(line_array[0]);

    if(line_array[0][word1_len - 1] == ':'){
        if(line_array[1] != NULL || line_array[2] != NULL){
            preprocessor_error("Too many operands for label definition");
        }
        
        char* string = (char*)malloc(word1_len - 1);
        memcpy(string, line_array[0], word1_len - 1);
        string[word1_len] = '\0';

        for(uint32_t i = 0; i < label_index; i++){
            if(!strcmp(labels[i].name, string)){
                parser_error("Redifined label");
            }
        }

        label_t label = {.address = line_number * 6, .name = string};
        labels[label_index] = label;
        label_index++;
        return;
    }

    if(!strcmp(line_array[1], "equ")){    
        
        uint64_t value;
        char* value_str = NULL;

        if(line_array[2][0] != '#' || line_array[2][0] == '$'){
            preprocessor_error("Couldn't get value from definition");
        }

        if(line_array[2] == NULL){
            preprocessor_error("No value specified for definition");
        }else{
            value_str = line_array[2] + 1;
        }

        if(value_str != NULL){
            if(value_str[0] == 'x'){
                value_str++;
                value = strtol(value_str, NULL, 16);
            }else{
                value = atoi(value_str);
            }
        }
        if(value > UINT16_MAX){
            preprocessor_error("Operand value too large to be supported");
        }

        char* string = (char*)malloc(word1_len - 1);
        memcpy(string, line_array[0], word1_len);
        string[word1_len] = '\0';

        for(uint32_t i = 0; i < definition_index; i++){
            if(!strcmp(definitions[i].name, string)){
                parser_error("Redifined definition");
            }
        }

        definition_t definition = {.value = (uint16_t)value, .name = string};
        definitions[definition_index] = definition;
        definition_index++;
    }
}

void parse_line(char* line, uint32_t line_nmbr){

    line_number = line_nmbr;

    char** line_array = get_line_array(line);

    if(line_array[0] == NULL){
        parser_error("Error creating line array");
    }

    if(line_array[0][strlen(line_array[0]) - 1] == ':'){
        free_line_array(line_array);
        return;
    }
    
    if(line_array[1] != NULL){
        if(!strcmp(line_array[1], "equ")){
            free_line_array(line_array);
            return;
        }
    }

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