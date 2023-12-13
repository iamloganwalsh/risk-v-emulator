#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include <stdio.h>
#include "vm.h"
#include "virtual_routines.h"

int add(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int addi(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sub(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int lui(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm);
int xor_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int xori(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int or_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int ori(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int and_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int andi(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sll(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int srl(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int sra(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);

int lb(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lh(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lw(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lbu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lhu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sb(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int sh(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int sw(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);

int slt(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int slti(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sltu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int sltiu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int beq(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bne(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int blt(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bge(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bltu(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bgeu(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int jal(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm);
int jalr(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);

#endif