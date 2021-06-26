#include <parser.h>
#include <arch/instruction.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char** get_line_array(char* line){
    uint64_t line_len = strlen(line);
    uint64_t whitespace_count = 0;

    for(uint64_t i = 0; i < line_len; i++){
        if(line[i] == ' ') whitespace_count++;
    }

    char** line_array = (char**)malloc(whitespace_count * sizeof(char*));

    uint64_t word_index = 0;
    uint64_t whitespace_index = 0;
    for(uint64_t i = 0; i < line_len; i++){
        if(line[i] != ' ' && line[i] != '\0') word_index++;
        else{
            char* word = (char*)malloc(word_index * sizeof(char) + 1);
            for(uint64_t j = 0; j < word_index; j++){
                word[j] = line[i - word_index + j];
            }
            word[word_index] = '\0';
            line_array[whitespace_index] = word;
            word_index = 0;
            whitespace_index++;
        }
    }

    printf("%d\n", (int)whitespace_count);

    for(uint64_t i = 0; i < whitespace_count; i++){
        printf("%s\n", line_array[i]);
    }

    return line_array;

}

void parse_line(char* line){
    

    
    for(uint64_t i = 0; i < INSTRUCTIONS_COUNT; i++){
        
    }
}