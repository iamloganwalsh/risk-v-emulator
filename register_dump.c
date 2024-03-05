#include "register_dump.h"


void register_dump(VM *vm) {
    printf("PC = 0x%08x;\n", vm->pc);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("R[%d] = 0x%08x;\n", i, vm->reg[i]);
    }
}