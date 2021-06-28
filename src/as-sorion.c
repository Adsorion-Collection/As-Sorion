#include <stdio.h>
#include <stdlib.h>
#include <common.h>
#include <parser.h>
#include <arch/instruction.h>

void assemble(string file_name){
    
}

int main(void){
   register_instructions();
   parse_line("STORE $x3299 #x3300", 0);
   assemble("FFFF");
}