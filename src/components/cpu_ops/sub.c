#include "../cpu.h"

void sub_registers(Cpu *cpu, Register minuend, Register target)
{
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->overflow_flag = val_a < val_b;
    cpu->zero_flag = val_a == val_b;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, val_a - val_b);
        break;
    case B:
        push(&cpu->registers.reg_B, val_b - val_a);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void sub_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val, val_msb, val_lsb;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->overflow_flag = val < value;
        cpu->zero_flag = val == value;
        push(&cpu->registers.reg_A, val - value);
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->overflow_flag = val < value;
        cpu->zero_flag = val == value;
        push(&cpu->registers.reg_B, val - value);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void sub_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        sub_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        data_bus_t value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        data_bus_t val_a = peek(&cpu->registers.reg_A);
        data_bus_t val_b = peek(&cpu->registers.reg_B);
        bool overflow_lsb = val_b < value_lsb;
        cpu->overflow_flag = val_a - overflow_lsb < value_msb || val_a == 0;
        push(&cpu->registers.reg_B, val_b - value_lsb);
        push(&cpu->registers.reg_A, val_a - value_msb - overflow_lsb);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}