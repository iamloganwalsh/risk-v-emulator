#include "virtual_routines.h"

int check_virtual_routine(uint32_t instruction, uint32_t *address, uint32_t *value, VM *vm) {
    switch(*address) {
        case 0x0800:
            if (*value < 128) {
                char ascii_char = (char)(*value);
                printf("%c", ascii_char);
            }
            break;
        case 0x0804:
            int32_t val = *value;
            printf("%d", val);
            break;
        case 0x0808:
            printf("%x", *value);
        case 0x080C:
            printf("CPU Halt Requested\n");
            exit(0);
        case 0x0812:
            char ascii_char;
            scanf("%c", ascii_char);

            if (&ascii_char != 1) {
                printf("Error with input\n");
                exit(0);
            }
            
            if (0 <= ascii_char <= 127) {
                uint32_t ascii_value = (uint32_t)ascii_char;
                *value = ascii_value;
            }
            else {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(0);
            }
            break;
        case 0x0816:
            int32_t val;
            scanf("%d", val);
            if (2147483647 > val || val < -2147483648) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                exit(0);
            }

            *value = val;
            break;
        case 0x0820:
            printf("%x", vm->pc);
            break;
        case 0x0824:
            register_dump(vm);
            break;
        case 0x0828:
            uint32_t v = *value;
            if (INST_MEM_START <= v <= INST_MEM_END) {
                uint32_t val = (vm->inst_mem[v]) | (vm->inst_mem[v + 1] << 8) | (vm->inst_mem[v + 2] << 16) | (vm->inst_mem[v + 3] << 24);
            }
            else if (DATA_MEM_START <= v <= DATA_MEM_END) {
                uint32_t val = (vm->data_mem[v]) | (vm->data_mem[v + 1] << 8) | (vm->data_mem[v + 2] << 16) | (vm->data_mem[v + 3] << 24);
            }
            else if (HEAP_BANKS_START <= v <= HEAP_BANKS_END) {
                
            }
            else {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(0);
            }
            break;

        case 0x0830:
            int memory_size = *value;
            int required_banks;
            if (memory_size > HEAP_SIZE) {
                required_banks = (memory_size / HEAP_SIZE) + 1;
            }
            else {
                required_banks = 1;
            }
            int num_available_banks = NUM_HEAP_BANK - vm->first_free_heap;
            if (required_banks > num_available_banks) {
                vm->reg[28] = 0;
                return 0;
            }

            uint32_t new_starting_address = 0xb700 + (vm->first_free_heap * HEAP_SIZE);
            if (new_starting_address <= 0xd700) {
                vm->reg[28] = new_starting_address;
            }
            else {
                vm->reg[28] = 0;
            }
            vm->heap_banks[vm->first_free_heap].is_head = true;

            for (int i = vm->first_free_heap; i < required_banks - 1; i++) {
                vm->heap_banks[i].is_free = false;
                if (memory_size >= HEAP_SIZE) {
                    vm->heap_banks[i].size = HEAP_SIZE;
                    memory_size -= vm->heap_banks[i].size;
                }
                else {
                    vm->heap_banks[i].size = memory_size;
                }
            }
            break;
        case 0x0834:
            uint32_t starting_address = *value;
            if (starting_address <= HEAP_BANKS_START || starting_address >= HEAP_BANKS_END) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(0);
            }

            int index = (starting_address - HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[index].is_free == true || vm->heap_banks[index].is_head == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(0);
            }
            else {
                int i = 0;
                while (true) {




                    break;
                }
            }
            break;
    }
    return 0;
}
