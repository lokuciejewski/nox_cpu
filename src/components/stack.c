#include "stack.h"

Stack *init_stack(uint16_t size)
{
    Stack *stack = malloc(sizeof(Stack));
    stack->stack_ptr = 0;
    stack->stack_size = size;
    stack->stack = malloc(sizeof(data_bus_t) * size);
    return stack;
}

void push(Stack *stack, data_bus_t value)
{
    stack->stack[stack->stack_ptr % stack->stack_size] = value;
    stack->stack_ptr = (stack->stack_ptr + 1) % stack->stack_size;
}

data_bus_t pop(Stack *stack)
{
    uint8_t current = (uint8_t)(stack->stack_ptr - 1) % stack->stack_size;
    data_bus_t val = stack->stack[current];
    stack->stack[current] = 0;
    stack->stack_ptr = current;
    return val;
}

data_bus_t peek(Stack *stack)
{
    return stack->stack[(uint8_t)(stack->stack_ptr - 1) % stack->stack_size];
}

void print_stack(Stack *stack)
{
    printf("STACK\n");
    for (uint8_t i = stack->stack_size; i > 0; i--)
    {
        uint8_t current = (uint8_t)(stack->stack_ptr - i) % stack->stack_size;
        printf("0x%x\n", stack->stack[current]);
    }
}