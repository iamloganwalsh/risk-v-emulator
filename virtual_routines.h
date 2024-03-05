#ifndef VIRTUAL_ROUTINES_H
#define VIRTUAL_ROUTINES_H


#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "vm.h"
#include "register_dump.h"

char ascii_char;
int32_t input;
uint32_t tempval;
int memory_size;
uint32_t freeAddress;


int check_virtual_routines(uint32_t instruction, uint32_t *address, uint32_t *value, VM *vm);

#endif