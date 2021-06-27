#include <stdio.h>
#include <common.h>
#include <parser.h>
#include <arch/instruction.h>

void assemble(string file_name){
    
}

int main(void){
   register_instructions();
   parse_line("STORE EEE EE");
   assemble("FFFF"); 
}