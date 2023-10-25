#include "../cpu.h"

void add_registers(Cpu *cpu, Register summand, Register target)
{
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->overflow_flag = val_a > 127 && val_b > 127; // TODO: hardcoded values
    cpu->zero_flag = val_a == 0 == val_b;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, val_a + val_b);
        break;
    case B:
        push(&cpu->registers.reg_B, val_a + val_b);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void add_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->overflow_flag = val > 127 && value > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_A, val + value);
        cpu->zero_flag = peek(&cpu->registers.reg_A) == 0;
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->overflow_flag = val > 127 && value > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_B, val + value);
        cpu->zero_flag = peek(&cpu->registers.reg_B) == 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void add_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        add_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        data_bus_t value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        data_bus_t val_a = peek(&cpu->registers.reg_A);
        data_bus_t val_b = peek(&cpu->registers.reg_B);
        bool overflow_lsb = val_b > 127 && value_lsb > 127;                 // TODO: hardcoded values
        cpu->overflow_flag = val_a + overflow_lsb > 127 && value_msb > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_B, val_b + value_lsb);
        push(&cpu->registers.reg_A, val_a + value_msb + overflow_lsb);
        cpu->zero_flag = 0 == peek(&cpu->registers.reg_A) == peek(&cpu->registers.reg_B);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}