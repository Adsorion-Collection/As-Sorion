#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <parser.h>
#include <arch/instruction.h>

void assemble(char* string){

}

int main(void){
   register_instructions();
   assemble("FFFF");
   preprocess_line("name:", 1);
   for(uint32_t i = 0; i < label_index; i++){
      printf("%s: %d\n", labels[i].name, labels[i].address);
   }for(uint32_t i = 0; i < definition_index; i++){
      printf("%s: %d\n", definitions[i].name, definitions[i].value);
   }
   parse_line("STORE REG4 REG3", 2);

}