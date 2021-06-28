#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <parser.h>
#include <arch/instruction.h>

void assemble(char* string){
   
}

int main(void){
   register_instructions();
   parse_line("e equ #433", 0);
   assemble("FFFF");

   for(uint32_t i = 0; i < label_index; i++){
      printf("%s\n", labels[i].name);
   }
}