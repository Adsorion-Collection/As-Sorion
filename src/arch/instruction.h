#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include <stdint.h>

#define INSTRUCTIONS_COUNT 0x6

typedef enum{
    MEM_MEM = 1 << 0,
    REG_MEM = 1 << 1,
    REG_REG = 1 << 2,
    MEM_REG = 1 << 3,
    IMMEDIATE = 1 << 4,
    REG = 1 << 5,
    NOTHING = 1 << 6
} addr_modes_e;

#define NULL_STR "NULL"
#define STORE_STR "STORE"
#define ADD_STR "ADD"
#define SUB_STR "SUB"
#define JMPEQ_STR "JMPEQ"
#define JMPMR_STR "JMPMR"
#define CMP_STR "CMP"

typedef struct{
    char* name;
    uint8_t opcode;
    uint8_t supported_addr_modes; // Bitmap of supported addressing modes
    uint16_t* (*gen_opcode_str)(char* line[]);
} instruction_t;

extern instruction_t instruction_set[INSTRUCTIONS_COUNT];
extern const instruction_t null_instruction;

void register_instruction(char* name, uint8_t opcode, uint8_t addr_mode_bitmap, uint16_t*(*gen_opcode_str)(char* line[]));
void register_instructions();

#endif