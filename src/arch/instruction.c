#include <arch/instruction.h>
#include <stddef.h>

char* REG_strings[REGISTER_COUNT];

instruction_t instruction_set[INSTRUCTIONS_COUNT];
const instruction_t null_instruction = {.name = NULL_STR, .gen_opcode_str = NULL, .opcode = 0, .supported_addr_modes = 0};

extern uint16_t* gen_STORE_opcode_str(char* line[]);
extern uint16_t* gen_ADD_opcode_str(char* line[]);
extern uint16_t* gen_SUB_opcode_str(char* line[]);
extern uint16_t* gen_JMPEQ_opcode_str(char* line[]);
extern uint16_t* gen_JMPMR_opcode_str(char* line[]);
extern uint16_t* gen_CMP_opcode_str(char* line[]);
extern uint16_t* gen_NOT_opcode_str(char* line[]);
extern uint16_t* gen_AND_opcode_str(char* line[]);
extern uint16_t* gen_OR_opcode_str(char* line[]);

void register_instruction(char* name, uint8_t opcode, uint8_t addr_mode_bitmap, uint16_t*(*gen_opcode_str)(char* line[])){
    instruction_t i = {.gen_opcode_str = gen_opcode_str, .name = name, .opcode = opcode, .supported_addr_modes = addr_mode_bitmap};
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
    register_instruction(STORE_STR, 0x1, (MEM_MEM | MEM_REG| REG_MEM | REG_REG), gen_STORE_opcode_str);
}