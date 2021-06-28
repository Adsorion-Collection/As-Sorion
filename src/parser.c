#include <parser.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static uint32_t line_number;

void parser_error(char* message){
    printf("[ERROR][Parsing_Phase]: %s; On line: %d\n", message, line_number);
    exit(-1);
}

void free_line_array(char** line_array){
    for(uint32_t i = 0; i < 3; i++){
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
            char* word = (char*)malloc(word_index * sizeof(char) + 1);
            for(uint64_t j = 0; j < word_index; j++){
                word[j] = line[i - word_index + j];
            }
            word[word_index] = '\0';
            if(whitespace_index == 0x3){
                parser_error("Too many operands");
                free(word);
                break;
            }
            line_array[whitespace_index] = word;
            word_index = 0;
            whitespace_index++;
        }
    }

    if(whitespace_index == 0x2){
        line_array[2] = NULL;
    }

    return line_array;
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

    printf("%d\n", get_addr_mode(line_array));

    free_line_array(line_array);
}