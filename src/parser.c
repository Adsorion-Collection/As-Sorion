#include <parser.h>
#include <arch/instruction.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void free_line_array(char** line_array){
    for(uint32_t i = 0; i < 3; i++){
        free(line_array[i]);
    }
    free(line_array);
}

addr_modes_e get_addr_mode(char** line_array){
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

    char** line_array = (char**)malloc(whitespace_count * sizeof(char*));

    uint64_t word_index = 0;
    uint64_t whitespace_index = 0;
    for(uint64_t i = 0; i <= line_len; i++){
        if(line[i] != ' ' && line[i] != '\0') word_index++;
        else {
            if(!word_index){
                continue;
            }
            char* word = (char*)malloc(word_index * sizeof(char) + 1);
            for(uint64_t j = 0; j < word_index; j++){
                word[j] = line[i - word_index + j];
            }
            word[word_index] = '\0';
            if(whitespace_index == 0x3){
                printf("Too many operands, removing operand: %s\n", word);
                free(word);
                break;
            }
            line_array[whitespace_index] = word;
            word_index = 0;
            whitespace_index++;
        }
    }

    return line_array;
}

void parse_line(char* line){
    
    char** line_array = get_line_array(line);
    
    instruction_t line_instruction = null_instruction;

    for(uint64_t i = 0; i < INSTRUCTIONS_COUNT; i++){
        if(!strcmp(line_array[0], instruction_set[i].name)){
            line_instruction = instruction_set[i];
            break;
        }
    }

    if(!strcmp(line_instruction.name, null_instruction.name)){
        printf("Unknown instruction: %s\n", line_array[0]);
    }

    free(line_array);
}