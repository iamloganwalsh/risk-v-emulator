#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdbool.h>

#define INST_MEM_SIZE 1024
#define INST_MEM_START 0x0000
#define INST_MEM_END 0x3ff

#define DATA_MEM_SIZE 1024
#define DATA_MEM_START 0x0400
#define DATA_MEM_END 0x7ff

#define VIRTUAL_ROUTINES_START 0x0800
#define VIRTUAL_ROUTINES_END 0x8ff

#define HEAP_BANKS_START 0xb700
#define HEAP_BANKS_END 0xd700

#define NUM_REGISTERS 32
#define FILE_SIZE 2048
#define MEMORY_SIZE 1024

#define NUM_HEAP_BANK 128
#define HEAP_SIZE 64

typedef struct {
    uint32_t size;
    uint8_t data[HEAP_SIZE];
    bool is_free;
    bool is_head;
} HeapBank;

typedef struct {
    uint32_t pc;
    uint8_t inst_mem[INST_MEM_SIZE];
    uint8_t data_mem[DATA_MEM_SIZE];
    uint32_t reg[NUM_REGISTERS];
    HeapBank heap_banks[NUM_HEAP_BANK];
    int first_free_heap;
} VM;

#endif