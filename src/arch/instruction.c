#include <arch/instruction.h>
#include <stddef.h>

char* REG_strings[REGISTER_COUNT];

instruction_t instruction_set[INSTRUCTIONS_COUNT];
const instruction_t null_instruction = {.name = NULL_STR, .opcode = 0, .supported_addr_modes = 0};

void register_instruction(char* name, uint8_t opcode, uint8_t addr_mode_bitmap){
    instruction_t i = {.name = name, .opcode = opcode, .supported_addr_modes = addr_mode_bitmap};
    instruction_set[opcode] = i;
}

void register_instructions(){

    REG_strings[0] = "REG0";
    REG_strings[1] = "REG1";
    REG_strings[2] = "REG2";
    REG_strings[3] = "REG3";
    REG_strings[4] = "REG4";
    REG_strings[5] = "REG5";
    
    for(uint64_t i = 0; i < INSTRUCTIONS_COUNT; i++){
        instruction_set[i] = null_instruction;
    }

    register_instruction(NULL_STR, 0x0, (NOTHING));
    register_instruction(STORE_STR, 0x1, (MEM_MEM | MEM_REG| REG_MEM | REG_REG | REG_IMMEDIATE | MEM_IMMEDIATE));
    register_instruction(ADD_STR, 0x2, (REG_REG | REG_MEM | REG_IMMEDIATE | MEM_IMMEDIATE | MEM_MEM | MEM_REG));

}