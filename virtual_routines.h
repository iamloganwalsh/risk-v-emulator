#ifndef VIRTUAL_ROUTINES_H
#define VIRTUAL_ROUTINES_H


#include <stdint.h>
#include <stdlib.h>
#include "vm.h"
#include "register_dump.h"

int check_virtual_routine(uint32_t instruction, uint32_t *address, uint32_t *value, VM *vm);

#endif