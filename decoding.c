#include "instructions.c"

int get_opcode(uint32_t instruction, VM *vm) {
    uint8_t opcode = instruction & 0x7F;

    switch (opcode) {
        case 0b0110011:
            type_r(instruction, opcode, vm);
            break;
        case 0b0010011:
            type_i(instruction, opcode, vm);
            break;
        case 0b0000011:
            type_i(instruction, opcode, vm);
            break;
        case 0b1100111:
            type_i(instruction, opcode, vm);
            break;
        case 0b0110111:
            type_u(instruction, opcode, vm);
            break;
        case 0b0100011:
            type_s(instruction, opcode, vm);
            break;
        case 0b1100011:
            type_sb(instruction, opcode, vm);
            break;
        case 0b1101111:
            type_uj(instruction, opcode, vm);
            break;
        default:
            exit(1);
    }
    return 0;
}

int type_r(uint32_t instruction, uint8_t opcode, VM * vm) {
    // rd
    uint32_t rd_mask = 0x1F;
    uint32_t rd = (instruction >> 7) & rd_mask;
    // func3
    uint32_t func3_mask = 0x7;
    uint32_t func3 = (instruction >> 12) & func3_mask;
    // rs1
    uint32_t rs1_mask = 0x1F;
    uint32_t rs1 = (instruction >> 15) & rs1_mask;
    // rs2
    uint32_t rs2_mask = 0x1F;
    uint32_t rs2 = (instruction >> 20) & rs2_mask;
    // func7
    uint32_t func7_mask = 0x7F;
    uint32_t func7 = (instruction >> 25) & func7_mask;

    switch (func7) {
        case 0b0000000:
            switch (func3) {
                case 0b000:
                    add(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b100:
                    xor_func(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b110:
                    or_func(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b111:
                    and_func(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b001:
                    sll(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b101:
                    srl(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b010:
                    slt(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b011:
                    sltu(instruction, rd, rs1, rs2, vm);
                    break;
            }
            break;

        case 0b0100000:
            switch (func3) {
                case 0b000:
                    sub(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b101:
                    sra(instruction, rd, rs1, rs2, vm);
                    break;
                    
            }
            break;
    }
    return 0;
}

int type_i(uint32_t instruction, uint8_t opcode, VM * vm) {
    // rd
    uint32_t rd_mask = 0x1F;
    uint32_t rd = (instruction >> 7) & rd_mask;

    // func3
    uint32_t func3_mask = 0x7;
    uint32_t func3 = (instruction >> 12) & func3_mask;

    // rs1
    uint32_t rs1_mask = 0x1F;
    uint32_t rs1 = (instruction >> 15) & rs1_mask;

    // imm
    uint32_t imm_mask = 0xfff;
    uint32_t imm = (instruction >> 20) & imm_mask;
    int32_t signed_imm = ((int32_t)(imm << 20)) >> 20;

    switch(opcode) {
        case 0b0010011:
            switch(func3) {
                case 0b000:
                    addi(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b100:
                    xori(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b110:
                    ori(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b111:
                    andi(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b010:
                    slti(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b011:
                    sltiu(instruction, rd, rs1, signed_imm, vm);
                    break;  
            }
            break;
        case 0b0000011:
            switch(func3) {
                case 0b000:
                    lb(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b001:
                    lh(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b010:
                    lw(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b100:
                    lbu(instruction, rd, rs1, imm, vm);
                    break;
                case 101:
                    lhu(instruction, rd, rs1, imm, vm);
                    break;
            }
            break;
        case 0b1100111:
            switch(func3) {
                case 0b000:
                    jalr(instruction, rd, rs1, signed_imm, vm);
                    break;
            }
            break;
    }
    return 0;
}

int type_s(uint32_t instruction, uint8_t opcode, VM * vm) {

    // func3
    uint32_t func3_mask = 0x7;
    uint32_t func3 = (instruction >> 12) & func3_mask;

    // rs1
    uint32_t rs1_mask = 0x1f;
    uint32_t rs1 = (instruction >> 15) & rs1_mask;

    // rs2
    uint32_t rs2_mask = 0x1f;
    uint32_t rs2 = (instruction >> 20) & rs2_mask;

    // imm
    uint32_t imm = (((instruction >> 25) & 0x7f) << 5) | ((instruction >> 7) & 0x1f);
    int32_t signed_imm = ((int32_t)(imm << 20)) >> 20;

    switch(opcode) {
        case 0b0100011:
            switch(func3) {
                case 0b000:
                    sb(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b001:
                    sh(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b010:
                    sw(instruction, rs1, rs2, signed_imm, vm);
                    break;
            }
            break;
    }
    return 0;
}

int type_sb(uint32_t instruction, uint8_t opcode, VM * vm) {
    // func3
    uint32_t func3_mask = 0x7;
    uint32_t func3 = (instruction >> 12) & func3_mask;

    // rs1
    uint32_t rs1_mask = 0xf;
    uint32_t rs1 = (instruction >> 15) & rs1_mask;

    // rs2
    uint32_t rs2_mask = 0xf;
    uint32_t rs2 = (instruction >> 20) & rs2_mask;

    // imm
    uint32_t imm_first = (instruction >> 31) & 0x1;
    uint32_t imm_second = (instruction >> 7) & 0x1;
    uint32_t imm_third = (instruction >> 25) & 0x3f;
    uint32_t imm_fourth = (instruction >> 8) & 0xf;

    uint32_t combined_imm = (imm_first << 12) | 
                            (imm_second << 11) |
                            (imm_third << 5) |
                            (imm_fourth << 1);

    int32_t signed_imm = ((int32_t)(combined_imm << 19)) >> 19;
    
    switch (opcode) {
        case 0b1100011:
            switch (func3) {
                case 0b000:
                    beq(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b001:
                    bne(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b100:
                    blt(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b110:
                    bltu(instruction, rs1, rs2, combined_imm, vm);
                    break;
                case 0b101:
                    bge(instruction, rs1, rs2, signed_imm, vm);
                    break;
                case 0b111:
                    bgeu(instruction, rs1, rs2, combined_imm, vm);
                    break;
            }
            break;
    }
    return 0;
}

int type_u(uint32_t instruction, uint8_t opcode, VM * vm) {
    // rd
    uint32_t rd_mask = 0x1f;
    uint32_t rd = (instruction >> 7) & rd_mask;

    // imm
    uint32_t imm_mask = 0xfffff;
    uint32_t imm = (instruction >> 12) & imm_mask;
    int32_t signed_imm = ((int32_t)(imm));

    lui(instruction, rd, signed_imm, vm);

    return 0;

}

int type_uj(uint32_t instruction, uint8_t opcode, VM * vm) {
    // rd
    uint32_t rd_mask = 0x1f;
    uint32_t rd = (instruction >> 7) & rd_mask;

    // imm
    uint32_t imm_first = (instruction >> 31) & 0x1;
    uint32_t imm_second = (instruction >> 12) & 0xff;
    uint32_t imm_third = (instruction >> 20) & 0x1;
    uint32_t imm_fourth = (instruction >> 21) & 0x3ff;
    uint32_t combined_imm = (imm_first << 20) | 
                            (imm_second << 12) | 
                            (imm_third << 11) | 
                            (imm_fourth << 1);
    int32_t signed_imm = ((int32_t)(combined_imm << 11)) >> 11;

    jal(instruction, rd, signed_imm, vm);

    return 0;

}