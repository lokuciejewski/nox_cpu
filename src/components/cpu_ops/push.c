#include "../cpu.h"

void push_register_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t value_lsb;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, value);
        break;
    case B:
        push(&cpu->registers.reg_B, value);
        break;
    case HI:
        cpu->registers.reg_HI = value;
        break;
    case LI:
        cpu->registers.reg_LI = value;
        break;
    case AB:
        value_lsb = read_immediate(cpu);
        push_to_double_reg(cpu, AB, from_2_data_bus_t(value, value_lsb));
        cpu->zero_flag = value == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        return;
    case HLI:
        value_lsb = read_immediate(cpu);
        push_to_double_reg(cpu, HLI, from_2_data_bus_t(value, value_lsb));
        cpu->zero_flag = value == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        return;
    default:
        break;
    }
    cpu->zero_flag = value == 0;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_register_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    data_bus_t value_msb, value_lsb;
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        push_register_immediate(cpu, target, value);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case AB:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        push_to_double_reg(cpu, AB, from_2_data_bus_t(value_msb, value_lsb));
        cpu->zero_flag = value_msb == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        push_to_double_reg(cpu, HLI, from_2_data_bus_t(value_msb, value_lsb));
        cpu->zero_flag = value_msb == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

void push_register_to_register(Cpu *cpu, Register source, Register target)
{
    switch (source)
    {
    case A:
        switch (target)
        {
        case B:
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_A));
            break;
        case HI:
            cpu->registers.reg_HI = peek(&cpu->registers.reg_A);
            break;
        case LI:
            cpu->registers.reg_LI = peek(&cpu->registers.reg_A);
            break;
        }
        break;
    case B:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_B));
            break;
        case HI:
            cpu->registers.reg_HI = peek(&cpu->registers.reg_B);
            break;
        case LI:
            cpu->registers.reg_LI = peek(&cpu->registers.reg_B);
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
            push(&cpu->registers.reg_A, cpu->registers.reg_EXIT_CODE & 0b00001111);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_EXIT_CODE & 0b00001111);
            break;
        }
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_register_indirect(Cpu *cpu, Register target)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    data_bus_t value = read_data(cpu);
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, value);
        cpu->zero_flag = value == 0;
        break;
    case B:
        push(&cpu->registers.reg_B, value);
        cpu->zero_flag = value == 0;
        break;
    case AB:
        push(&cpu->registers.reg_A, value);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        push(&cpu->registers.reg_B, value_lsb);
        cpu->zero_flag = value == 0 == value_lsb;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_stack_address_to_ab(Cpu *cpu)
{
    push_to_double_reg(cpu, AB, cpu->stack_address);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_stack_size_to_ab(Cpu *cpu)
{
    push_to_double_reg(cpu, AB, cpu->stack_size);
    delay_for_n_clock_ticks(cpu->clock, 1);
}