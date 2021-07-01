#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include <as-arch/instruction.h>
#include <as-sorion.h>
#include <string.h>

uint32_t line_count_without_defs = 0;

assembler_info_t assemble(char* file_name){
   FILE* file = fopen(file_name, "r");

   uint64_t line_count = 0;
   uint64_t file_length = 0;
   char c = fgetc(file);
   while(c != EOF){
      file_length++;
      if(c == '\n'){
         line_count++;
      }
      c = fgetc(file);
   }
   fclose(file);
   line_count++;
   
   char* file_buffer = (char*)malloc(file_length * sizeof(char) + 1);
   char** file_line_buffer = (char**)malloc(line_count * sizeof(char*));

   file = fopen(file_name, "r");
   c = fgetc(file);
   for(uint64_t i = 0; i < file_length; c = fgetc(file), i++){
      file_buffer[i] = c;   
   }

   file_buffer[file_length] = '\0';

   uint64_t line_number = 0;
   uint64_t line_index = 0;
   for(uint64_t i = 0; i <= file_length; i++){
      if(file_buffer[i] == '\0' || file_buffer[i] == '\n'){
         char* line = (char*)malloc(line_index * sizeof(char) + 1);
         memcpy(line, file_buffer + i - line_index, line_index);
         line[line_index] = '\0';
         file_line_buffer[line_number] = line;
         line_index = 0;
         line_number++;
      }else{
         line_index++;
      }
   }

   fclose(file);
   for(uint64_t i = 0; i < line_count; i++){
      preprocess_line(file_line_buffer[i], i); 
   }

   uint16_t* bytecode_buffer = (uint16_t*)calloc(line_count_without_defs, sizeof(uint16_t));

   uint64_t bytecode_buffer_index = 0;
   for(uint64_t i = 0; i < line_count; i++){
      uint16_t* line_bytecode = parse_line(file_line_buffer[i], i); 
      if(line_bytecode){
         memcpy((void*)bytecode_buffer + bytecode_buffer_index * 6, line_bytecode, 6);
         bytecode_buffer_index++;
         free(line_bytecode);
      }
   }
   free(file_buffer);
   free(file_line_buffer);

   assembler_info_t info = {.bytecode_buffer = bytecode_buffer, .bytecode_buffer_length = line_count_without_defs * 6};
   return info;
}

int main(void){
   register_instructions();
   assemble("test.asm");
}