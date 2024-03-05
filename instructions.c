#include "instructions.h"


int add(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] + vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int addi(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] + imm;
    }
    vm->pc += 4;
    return 0;
}

int sub(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] - vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int lui(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = (imm & 0xfffff000);
    }
    vm->pc += 4;
    return 0;
}

int xor_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] ^ vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int xori(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] ^ imm;
    }
    vm->pc += 4;
    return 0;
}

int or_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] | vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int ori(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] | imm;
    }
    vm->pc += 4;
    return 0;
}

int and_func(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] & vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int andi(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] & imm;
    }
    vm->pc += 4;
    return 0;
}

int sll(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (vm->reg[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] << vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int srl(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (vm->reg[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] >> vm->reg[rs2];
    }
    vm->pc += 4;
    return 0;
}

int sra(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm){
    if (vm->reg[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->reg[rd] = vm->reg[rs1] >> (vm->reg[rs2] & 0b11111);
    }
    vm->pc += 4;
    return 0;
}

int lb(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        uint32_t memory_address = vm->reg[rs1] + imm;
        
        if (INST_MEM_START <= memory_address && memory_address <= INST_MEM_END) {
            uint8_t memory_value = vm->inst_mem[memory_address];
            int32_t extended_value = (int32_t)(int8_t)memory_value;
            vm->reg[rd] = extended_value;
        }
        else if (DATA_MEM_START <= memory_address && memory_address <= DATA_MEM_END){
            uint8_t memory_value = vm->data_mem[memory_address - 1024];
            int32_t extended_value = (int32_t)(int8_t)memory_value;
            vm->reg[rd] = extended_value;
        }
        else if (VIRTUAL_ROUTINES_START <= memory_address && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t memory_value;
            check_virtual_routines(instruction, &memory_address, &memory_value, vm);
            int32_t extended_value = (int32_t)(int8_t)memory_value;
            vm->reg[rd] = extended_value;
        }
        else if (HEAP_BANKS_START <= memory_address && memory_address <= HEAP_BANKS_END) {
            int index = (memory_address = HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[index].is_free == true) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else {
                int data_position = (memory_address - (HEAP_BANKS_START + (index * HEAP_SIZE)));
                uint8_t memory_value = vm->heap_banks[index].data[data_position];
                int32_t extended_value = (int32_t)(int8_t)memory_value;
                vm->reg[rd] = extended_value;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lh(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        uint32_t memory_address = vm->reg[rs1] + imm;
        if (INST_MEM_START <= memory_address && memory_address <= INST_MEM_END) {
            uint16_t memory_value = (vm->inst_mem[memory_address]) | (vm->inst_mem[memory_address + 1] << 8);
            int32_t extended_value = (memory_value << 16) >> 16;
            vm->reg[rd] = extended_value;
        }
        else if (DATA_MEM_START <= memory_address && memory_address <= DATA_MEM_END){
            uint16_t memory_value = (vm->data_mem[memory_address - 1024]) | (vm->data_mem[memory_address - 1023] << 8);
            int32_t extended_value = (memory_value << 16) >> 16;
            vm->reg[rd] = extended_value;
        }
        else if (VIRTUAL_ROUTINES_START <= memory_address && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t memory_value;
            check_virtual_routines(instruction, &memory_address, &memory_value, vm);
            int32_t extended_value = (int32_t)(int8_t)memory_value;
            vm->reg[rd] = extended_value;
        }
        else if (HEAP_BANKS_START <= memory_address && memory_address <= HEAP_BANKS_END) {
            int index = (memory_address = HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[index].is_free == true) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else {
                int data_index = (memory_address - (HEAP_BANKS_START + (index * HEAP_SIZE)));
                uint16_t memory_value = (vm->heap_banks[index].data[data_index]) | (vm->heap_banks[index].data[data_index + 1] << 8);
                int32_t extended_value = (int32_t)(int16_t)memory_value;
                vm->reg[rd] = extended_value;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lw(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        uint32_t memory_address = vm->reg[rs1] + imm;
        if (INST_MEM_START <= memory_address && memory_address <= INST_MEM_END) {
            int32_t memory_value = (vm->inst_mem[memory_address]) | (vm->inst_mem[memory_address + 1] << 8) |
                                    (vm->inst_mem[memory_address + 2] << 16) | (vm->inst_mem[memory_address + 3] << 24);
            vm->reg[rd] = memory_value;
        }
        else if (DATA_MEM_START <= memory_address && memory_address <= DATA_MEM_END) {
            int32_t memory_value = (vm->inst_mem[memory_address - 1024]) | (vm->inst_mem[memory_address - 1023] << 8) |
                                    (vm->inst_mem[memory_address - 1022] << 16) | (vm->inst_mem[memory_address - 1021] << 24);
            vm->reg[rd] = memory_value;            
        }
        else if (VIRTUAL_ROUTINES_START <= memory_address && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t memory_value;
            check_virtual_routines(instruction, &memory_address, &memory_value, vm);
            vm->reg[rd] = memory_value;
        }
        else if (HEAP_BANKS_START <= memory_address && memory_address <= HEAP_BANKS_END) {
            int index = (memory_address = HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[index].is_free == true) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else {
                int data_position = (memory_address - (HEAP_BANKS_START + (index * HEAP_SIZE)));
                uint32_t memory_value = (vm->heap_banks[index].data[data_position]) | 
                                        (vm->heap_banks[index].data[data_position + 1] << 8) |
                                        (vm->heap_banks[index].data[data_position + 2] << 16) |
                                        (vm->heap_banks[index].data[data_position + 3] << 24);
                vm->reg[rd] = memory_value;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lbu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        uint32_t memory_address = vm->reg[rs1] + imm;
        
        if (INST_MEM_START <= memory_address && memory_address <= INST_MEM_END) {
            uint8_t memory_value = vm->inst_mem[memory_address];
            vm->reg[rd] = (uint32_t)memory_value;
        }
        else if (DATA_MEM_START <= memory_address && memory_address <= DATA_MEM_END){
            uint8_t memory_value = vm->data_mem[memory_address - 1024];
            vm->reg[rd] = (uint32_t)memory_value;
        }
        else if (VIRTUAL_ROUTINES_START <= memory_address && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t memory_value;
            check_virtual_routines(instruction, &memory_address, &memory_value, vm);
            vm->reg[rd] = memory_value;
        }
        else if (HEAP_BANKS_START <= memory_address && memory_address <= HEAP_BANKS_END) {
            int index = (memory_address = HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[index].is_free == true) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            }
            else {
                int data_position = (memory_address - (HEAP_BANKS_START + (index * HEAP_SIZE)));
                uint8_t memory_value = vm->heap_banks[index].data[data_position];
                vm->reg[rd] = (uint32_t)memory_value;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lhu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        uint32_t memory_address = vm->reg[rs1] + imm;
        if (memory_address >= INST_MEM_START && memory_address <= INST_MEM_END) {
            uint16_t two_byte_val = (vm->inst_mem[memory_address] << 0) | (vm->inst_mem[memory_address + 1] << 8);
            vm->reg[rd] = (uint32_t)two_byte_val;
        }
        else if (memory_address >= DATA_MEM_START && memory_address <= DATA_MEM_END) {
            uint16_t two_byte_val = (vm->data_mem[memory_address- 1024] << 0) | (vm->inst_mem[memory_address - 1023] << 8);
            vm->reg[rd] = (uint32_t)two_byte_val;
        }
        else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            vm->reg[rd] = val;
        }
        else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            int bank_index = (memory_address - HEAP_BANKS_START) / HEAP_SIZE;
            if (vm->heap_banks[bank_index].is_free == true) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +(bank_index * HEAP_SIZE)));
                uint16_t two_byte_val = (vm->heap_banks[bank_index].data[data_index] << 0) |
                                        (vm->heap_banks[bank_index].data[data_index + 1] << 8);
                vm->reg[rd] = (uint32_t)two_byte_val;
            }
        }
    }
    return 0;
}

int sb(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    uint32_t memory_address = vm->reg[rs1] + imm;
    if (memory_address >= INST_MEM_START && memory_address <= INST_MEM_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    else if (memory_address >= DATA_MEM_START && memory_address <= DATA_MEM_END) {
        vm->data_mem[memory_address - 1024] = (uint8_t)vm->reg[rs2];
    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->reg[rs2], vm);
    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {

        int bank_index = (memory_address - HEAP_BANKS_START) / HEAP_SIZE;
        if (vm->heap_banks[bank_index].is_free == true) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_SIZE)));
            vm->heap_banks[bank_index].data[data_index] = (uint8_t)vm->reg[rs2];
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

int sh(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    uint32_t memory_address = vm->reg[rs1] + imm;
    if (memory_address >= INST_MEM_START && memory_address <= INST_MEM_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    else if (memory_address >= DATA_MEM_START && memory_address <= DATA_MEM_END) {
        uint32_t value = vm->reg[rs2];
        uint8_t byte0 = value & 0xFF;
        uint8_t byte1 = (value >> 8) & 0xFF;

        vm->data_mem[memory_address - 1024] = byte0;
        vm->data_mem[memory_address - 1023] = byte1;

    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->reg[rs2], vm);

    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {

        int bank_index = (memory_address - HEAP_BANKS_START) / HEAP_SIZE;
        if (vm->heap_banks[bank_index].is_free == true) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_SIZE)));
            uint32_t value = vm->reg[rs2];
            uint8_t byte0 = value & 0xFF;
            uint8_t byte1 = (value >> 8) & 0xFF;

            vm->heap_banks[bank_index].data[data_index] = byte0;
            vm->heap_banks[bank_index].data[data_index + 1] = byte1;
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

int sw(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    uint32_t memory_address = vm->reg[rs1] + imm;
    if (memory_address >= INST_MEM_START && memory_address <= INST_MEM_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
        return 1;
    }
    else if (memory_address >= DATA_MEM_START && memory_address <= DATA_MEM_END) {
        uint32_t value = vm->reg[rs2];
        uint8_t byte0 = value & 0xFF;
        uint8_t byte1 = (value >> 8) & 0xFF;
        uint8_t byte2 = (value >> 16) & 0xFF;
        uint8_t byte3 = (value >> 24) & 0xFF;

        vm->data_mem[memory_address - 1024] = byte0;
        vm->data_mem[memory_address - 1023] = byte1;
        vm->data_mem[memory_address - 1022] = byte2;
        vm->data_mem[memory_address - 1021] = byte3;
    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->reg[rs2], vm);

    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
        int bank_index = (memory_address - HEAP_BANKS_START) / HEAP_SIZE;
        if (vm->heap_banks[bank_index].is_free == true) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_SIZE)));
            uint32_t value = vm->reg[rs2];
            uint8_t byte0 = value & 0xFF;
            uint8_t byte1 = (value >> 8) & 0xFF;
            uint8_t byte2 = (value >> 16) & 0xFF;
            uint8_t byte3 = (value >> 24) & 0xFF;
            vm->heap_banks[bank_index].data[data_index] = byte0;
            vm->heap_banks[bank_index].data[data_index + 1] = byte1;
            vm->heap_banks[bank_index].data[data_index + 2] = byte2;
            vm->heap_banks[bank_index].data[data_index + 3] = byte3;
            
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

int slt(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        if ((int32_t)vm->reg[rs1] < (int32_t)vm->reg[rs2]) {
            vm->reg[rd] = 1;
        }
        else {
            vm->reg[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int slti(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        if ((int32_t)vm->reg[rs1] < (int32_t)imm) {
            vm->reg[rd] = 1;
        }
        else {
            vm->reg[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int sltu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    if (rd != 0) {
        if (vm->reg[rs1] < vm->reg[rs2]) {
            vm->reg[rd] = 1;
        }
        else {
            vm->reg[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int sltiu(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        if (vm->reg[rs1] < imm) {
            vm->reg[rd] = 1;
        }
        else {
            vm->reg[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int beq(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if ((int32_t)vm->reg[rs1] == (int32_t)vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int bne(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if ((int32_t)vm->reg[rs1] != (int32_t)vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int blt(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if ((int32_t)vm->reg[rs1] < (int32_t)vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int bltu(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if (vm->reg[rs1] < vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int bge(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if ((int32_t)vm->reg[rs1] >= (int32_t)vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int bgeu(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    if (vm->reg[rs1] >= vm->reg[rs2]) {
        vm->pc += (imm << 1);
    }
    else {
        vm->pc += 4;
    }
    return 0;
}

int jal(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->pc + 4;
    }
    vm->pc += (imm << 1);
    return 0;
}

int jalr(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    if (rd != 0) {
        vm->reg[rd] = vm->pc + 4;
    }
    vm->pc = vm->reg[rs1] + imm;
    return 0;
}