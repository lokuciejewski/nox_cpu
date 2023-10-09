#pragma once
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "constants.h"

typedef struct Stack
{
    uint8_t stack_ptr;
    uint16_t stack_size;
    data_bus_t *stack;
} Stack;

Stack *init_stack(uint16_t size);
void push(Stack *stack, data_bus_t value);
data_bus_t pop(Stack *stack);
data_bus_t peek(Stack *stack);
void print_stack(Stack *stack);