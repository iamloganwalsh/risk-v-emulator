#ifndef DECODING_H
#define DECODING_H

#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>
#include "vm.h"
#include "register_dump.h"
#include "instructions.h"

int type_r(uint32_t instruction, uint8_t opcode, VM *vm);
int type_i(uint32_t instruction, uint8_t opcode, VM *vm);
int type_s(uint32_t instruction, uint8_t opcode, VM *vm);
int type_sb(uint32_t instruction, uint8_t opcode, VM *vm);
int type_u(uint32_t instruction, uint8_t opcode, VM *vm);
int type_uj(uint32_t instruction, uint8_t opcode, VM *vm);
int get_opcode(uint32_t instruction, VM *vm);

#endif