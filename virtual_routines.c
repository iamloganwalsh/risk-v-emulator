#include "virtual_routines.h"

int check_virtual_routines(uint32_t instruction, uint32_t *address, uint32_t *value, VM *vm) {
    uint32_t valoo = *value;

    switch(*address) {
        case 0x0800:
            if (0 <= valoo && valoo < 128) {
                char ascii_char = (char)(valoo);
                printf("%c", ascii_char);
            }
            else {
                printf("Invalid input\n");
                exit(1);
            }
            break;
        case 0x0804:
            printf("%d", (int32_t)valoo);
            break;
        case 0x0808:
            printf("%x", *value);
            break;
        case 0x080C:
            printf("CPU Halt Requested\n");
            exit(1);
        case 0x0812:
            
            if (scanf("%c", &ascii_char) != 1) {
                printf("Error reading input\n");
                exit(1);
            }
            
            if (0 <= ascii_char && ascii_char <= 127) {
                uint32_t ascii_value = (uint32_t)ascii_char;
                *value = ascii_value;
            }
            else {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            break;
        case 0x0816:
            
            if (scanf("%d", &input) != 1) {
                printf("Error reading input.\n");
                exit(1);
            }
            if (2147483647 < input || input < -2147483648) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }

            *value = input;
            break;
        case 0x0820:
            printf("%x", vm->pc);
            break;
        case 0x0824:
            register_dump(vm);
            break;
        case 0x0828:
            tempval = valoo;
            if (INST_MEM_START <= tempval && tempval <= INST_MEM_END) {
                uint32_t val = (vm->inst_mem[tempval]) | (vm->inst_mem[tempval + 1] << 8) | (vm->inst_mem[tempval + 2] << 16) | (vm->inst_mem[tempval + 3] << 24);
                printf("%d", val);
            }
            else if (DATA_MEM_START <= tempval && tempval <= DATA_MEM_END) {
                uint32_t val = (vm->data_mem[tempval - 1024]) | (vm->data_mem[tempval - 1023] << 8) | (vm->data_mem[tempval - 1022] << 16) | (vm->data_mem[tempval - 1021] << 24);
                printf("%d", val);
            }
            else if (HEAP_BANKS_START <= tempval && tempval <= HEAP_BANKS_END) {
                printf("Dump Memory Word: Heap Banks");
            }
            else {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            break;
        case 0x0830:
            memory_size = *value;
            int required_banks;

            if (memory_size == 0) {
                vm->reg[28] = 0x0;
            }
            required_banks = ceil(memory_size / HEAP_SIZE);
            if (required_banks > NUM_HEAP_BANK) {
                vm->reg[28] = 0;
                return 0;
            }

            int index = 0;
            bool exitf = false;

            while (true){
                exitf = false;
                if (index >= HEAP_SIZE - 1) {
                    vm->reg[28] = 0;
                }
                for (int i = 0; i < required_banks; i++) {
                    if (index + i > NUM_HEAP_BANK - 1) {
                        exitf = true;
                        vm->reg[28] = 0x0;
                        return 0;
                    }
                    HeapBank hb = vm->heap_banks[index + i];
                    if (!hb.is_free) {
                        exitf = true;
                    }
                    if (exitf) {
                        break;
                    }
                }
                if (!exitf) {
                    break;
                }
                index++;
            }

            uint32_t malloc_address = 0xb700 + (index * HEAP_SIZE);
            if (malloc_address <= 0xd700) {
                vm->reg[28] = malloc_address;
            } 
            else {
                vm->reg[28] = 0;
            }

            for (int i = 0; i < required_banks; i++) {
                if (vm->heap_banks[index + i].is_free) {
                    vm->heap_banks[index + i].is_free = false;
                    if (i == 0) {
                        vm->heap_banks[index + i].is_head = true;
                    }
                    vm->heap_banks[index + i].tailing_bank_size = required_banks - i - 1;
                    if (memory_size >= HEAP_SIZE) {
                        vm->heap_banks[index + i].size = HEAP_SIZE;
                        memory_size -= vm->heap_banks[index + i].size;
                    }
                    else {
                        vm->heap_banks[index + i].size = memory_size;
                    }
                }
                else {
                    printf("Illegal Operation: 0x%08x\n", instruction);
                    register_dump(vm);
                    exit(1);
                }
            }
            break;
        case 0x0834:
            freeAddress = valoo;
            int data_index = (freeAddress - HEAP_BANKS_START) / HEAP_SIZE;
            if (freeAddress <= HEAP_BANKS_START || freeAddress >= HEAP_BANKS_END) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else if (vm->heap_banks[data_index].is_free == true || vm->heap_banks[data_index].is_head == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else if (freeAddress % 64 != 0) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else {
                int i = 0;
                while (true) {
                    HeapBank hb = vm->heap_banks[data_index + 1];
                    if (hb.tailing_bank_size == 0){
                        break;
                    }
                    else if (i == 0) {
                        hb.is_head = false;
                    }
                    hb.is_free = true;
                    hb.tailing_bank_size = 0;
                    hb.size = 0;
                    for (int j = 0; j < HEAP_SIZE; j++) {
                        hb.data[j] = 0;
                    }
                    i++;
                }
            }
            break;
        default: 
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
            break;
        
    }
    return 0;
}
