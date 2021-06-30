#ifndef __AS_SORION_H
#define __AS_SORION_H

#include <stdint.h>

typedef struct{
    uint16_t* bytecode_buffer;
    uint32_t bytecode_buffer_length;
} assembler_info_t;

assembler_info_t assemble(char* file_name);

#endif