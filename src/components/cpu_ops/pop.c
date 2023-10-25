#include "../cpu.h"

void pop_register_absolute(Cpu *cpu, Register source, address_bus_t address)
{
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case HI:
        set_data_bus(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        set_data_bus(cpu, cpu->registers.reg_LI);
        break;
    case AB:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case HLI:
        set_data_bus(cpu, cpu->registers.reg_HI);
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, cpu->registers.reg_LI);
        break;
    default:
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void pop_register_to_register(Cpu *cpu, Register source, Register target)
{
    switch (source)
    {
    case A:
        switch (target)
        {
        case B:
            push(&cpu->registers.reg_B, pop(&cpu->registers.reg_A));
            break;
        case HI:
            cpu->registers.reg_HI = pop(&cpu->registers.reg_A);
            break;
        case LI:
            cpu->registers.reg_LI = pop(&cpu->registers.reg_A);
            break;
        }
        break;
    case B:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, pop(&cpu->registers.reg_B));
            break;
        case HI:
            cpu->registers.reg_HI = pop(&cpu->registers.reg_B);
            break;
        case LI:
            cpu->registers.reg_LI = pop(&cpu->registers.reg_B);
            break;
        }
        break;
    case HI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_HI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_HI);
            break;
        case LI:
            cpu->registers.reg_LI = cpu->registers.reg_HI;
            break;
        }
        break;
    case LI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_LI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_LI);
            break;
        case LI:
            cpu->registers.reg_HI = cpu->registers.reg_LI;
            break;
        }
        break;
    case EXIT_CODE:
        switch (target)
        {
        case A:
            cpu->registers.reg_EXIT_CODE = pop(&cpu->registers.reg_A) & 0b00001111;
            break;
        case B:
            cpu->registers.reg_EXIT_CODE = pop(&cpu->registers.reg_B) & 0b00001111;
            break;
        }
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_indirect(Cpu *cpu, Register source)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void pop_ab_to_stack_address(Cpu *cpu)
{
    cpu->stack_address = pop_from_double_reg(cpu, AB);
    cpu->stack_pointer = cpu->stack_address;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_ab_to_stack_size(Cpu *cpu)
{
    cpu->stack_size = pop_from_double_reg(cpu, AB);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_ab_to_irq_address(Cpu *cpu)
{
    cpu->irq_address = pop_from_double_reg(cpu, AB);
    delay_for_n_clock_ticks(cpu->clock, 1);
}