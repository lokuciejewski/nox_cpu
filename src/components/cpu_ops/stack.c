#include "../cpu.h"

void push_value_to_stack(Cpu *cpu, data_bus_t value)
{
    if (cpu->stack_pointer < cpu->stack_address + cpu->stack_size)
    {
        set_address_bus(cpu, cpu->stack_pointer);
        set_data_bus(cpu, value);
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        cpu->stack_pointer++;
        cpu->overflow_flag = cpu->stack_pointer == cpu->stack_address + cpu->stack_size;
    }
    else
    {
        cpu->overflow_flag = true;
        cpu->err_flag = true;
    }
}

data_bus_t pop_value_from_stack(Cpu *cpu)
{
    data_bus_t value = 0;
    if (cpu->stack_pointer > cpu->stack_address)
    {
        cpu->stack_pointer--;
        set_address_bus(cpu, cpu->stack_pointer);
        value = read_data(cpu);
        cpu->zero_flag = cpu->stack_pointer == cpu->stack_address;
    }
    else
    {
        cpu->zero_flag = true;
        cpu->err_flag = true;
    }
    return value;
}

void push_register_to_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, peek(&cpu->registers.reg_A));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, peek(&cpu->registers.reg_B));
            n_of_values--;
        }
        break;
    case HI:
        push_value_to_stack(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        push_value_to_stack(cpu, cpu->registers.reg_LI);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_to_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, pop(&cpu->registers.reg_A));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, pop(&cpu->registers.reg_B));
            n_of_values--;
        }
        break;
    case HI:
        push_value_to_stack(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        push_value_to_stack(cpu, cpu->registers.reg_LI);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_from_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push(&cpu->registers.reg_A, pop_value_from_stack(cpu));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push(&cpu->registers.reg_B, pop_value_from_stack(cpu));
            n_of_values--;
        }
        break;
    case HI:
        cpu->registers.reg_HI = pop_value_from_stack(cpu);
        break;
    case LI:
        cpu->registers.reg_LI = pop_value_from_stack(cpu);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}