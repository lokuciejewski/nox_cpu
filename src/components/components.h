#pragma once

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "constants.h"
#include "stack.h"

typedef struct Buses
{
    address_bus_t *address_bus;
    data_bus_t *data_bus;
    bool *sig_write;
    bool *sig_irq;
} Buses;

typedef struct Registers
{
    Stack reg_A;
    Stack reg_B;
    uint8_t reg_HI;
    uint8_t reg_LI;
    uint8_t reg_EXIT_CODE; // 4 bit register
} Registers;
