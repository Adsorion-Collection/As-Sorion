#include <arch/instruction.h>
#include <stddef.h>

extern uint16_t* gen_STORE_opcode_str(char* line[]);

void register_instruction(char* name, uint8_t opcode, uint8_t addr_mode_bitmap, uint16_t*(*gen_opcode_str)(char* line[])){
    instruction_t i = {.gen_opcode_str = gen_opcode_str, .name = name, .opcode = opcode, .supported_addr_modes = addr_mode_bitmap};
    instruction_set[opcode] = i;
}

void register_instructions(){
    instruction_t null_instruction = {.name = NULL};
    for(uint64_t i = 0; i < INSTRUCTIONS_COUNT; i++){
        instruction_set[i] = null_instruction;
    }
    register_instruction(STORE_STR, 0x1, (MEM_MEM | MEM_REG| REG_MEM | REG_REG), gen_STORE_opcode_str);
}