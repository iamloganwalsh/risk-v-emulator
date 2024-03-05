#include "vm.h"
#include "decoding.h"
#include "instructions.h"
#include "register_dump.h"
#include "virtual_routines.h"
#include "decoding.c"


int load_memory(const char *filename, uint8_t *inst_mem, uint8_t *data_mem) {

    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        fprintf(stderr, "File doesn't exist");
        return 1;
    }

    unsigned char buffer[FILE_SIZE];

    size_t bytes_read = fread(buffer, 1, FILE_SIZE, fptr);

    fclose(fptr);

    if (bytes_read != FILE_SIZE) {
        fprintf(stderr, "Error reading bytes from file\n");
        return 1;
    }

    // instruction memory
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        inst_mem[i] = buffer[i];
    }

    // data memory
    for (size_t i = MEMORY_SIZE; i < 2 * MEMORY_SIZE; i++) {
        data_mem[i - MEMORY_SIZE] = buffer[i];
    }
    return 0;
}

int load_heap_banks(VM *vm) {
    for (int i = 0; i < NUM_HEAP_BANK; i++) {
        vm->heap_banks[i].size = HEAP_SIZE;
        vm->heap_banks[i].is_free = true;
        vm->heap_banks[i].is_head = false;
        vm->heap_banks[i].tailing_bank_size = 0;
    }
    return 0;
}


int main (int argc, char *argv[]) {
    argc = 2;
    argv[1] = "/Users/loganwalsh/Documents/C/riskxvii/printing_h.mi";
    

    if (argc != 2) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    const char *binary_file = argv[1];
    VM vm = {0};
    vm.pc = 0;

    if (load_memory(binary_file, vm.inst_mem, vm.data_mem) == 1) {
        exit(1);
    }

    load_heap_banks(&vm);

    while (true){
        uint8_t *ip = (uint8_t*)vm.inst_mem;
        uint32_t instruction = *(uint32_t*)(ip + vm.pc);
        vm.reg[0] = 0;
        get_opcode(instruction, &vm);
    }

    return 0;
    
}
