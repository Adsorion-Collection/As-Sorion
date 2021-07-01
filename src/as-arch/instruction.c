#include <as-arch/instruction.h>
#include <stddef.h>

char* REG_strings[AS_REGISTER_COUNT];

instruction_t instruction_set[AS_INSTRUCTIONS_COUNT];
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
    
    REG_strings[6] = "REG0l";
    REG_strings[7] = "REG0h";
    REG_strings[8] = "REG1l";
    REG_strings[9] = "REG1h";
    REG_strings[10] = "REG2l";
    REG_strings[11] = "REG2h";
    REG_strings[12] = "REG3l";
    REG_strings[13] = "REG3h";
    REG_strings[14] = "REG4l";
    REG_strings[15] = "REG4h";
    REG_strings[16] = "REG5l";
    REG_strings[17] = "REG5h";

    for(uint64_t i = 0; i < AS_INSTRUCTIONS_COUNT; i++){
        instruction_set[i] = null_instruction;
    }

    register_instruction(NULL_STR, 0x0, (NOTHING));
    register_instruction(STORE_STR, 0x1, (MEM_REG| REG_MEM | REG_REG | REG_IMMEDIATE | MEM_IMMEDIATE));
    register_instruction(ADD_STR, 0x2, (REG_REG | REG_MEM | REG_IMMEDIATE | MEM_IMMEDIATE | MEM_REG));
    register_instruction(SUB_STR, 0x3, (REG_REG | REG_MEM | REG_IMMEDIATE | MEM_IMMEDIATE | MEM_REG));
    register_instruction(JMPEQ_STR, 0x4, (REG | MEM));
    register_instruction(JMPMR_STR, 0x5, (REG | MEM));
    register_instruction(CMP_STR, 0x6, (REG_MEM | REG_IMMEDIATE | MEM_REG | MEM_IMMEDIATE | REG_REG));
    register_instruction(NOT_STR, 0x7, (MEM | REG));
    register_instruction(AND_STR, 0x8, (REG_REG | REG_MEM | REG_IMMEDIATE | MEM_IMMEDIATE | MEM_REG));
    register_instruction(OR_STR, 0x9, (REG_REG | REG_MEM | REG_IMMEDIATE | MEM_IMMEDIATE | MEM_REG));
    register_instruction(SHL_STR, 0xA, (REG_REG | REG_MEM | REG_IMMEDIATE));
    register_instruction(SHR_STR, 0xB, (REG_REG | REG_MEM | REG_IMMEDIATE));

}