#include "../cpu.h"

void compare_register_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val, reg_msb, reg_lsb;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case HI:
        val = cpu->registers.reg_HI;
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case LI:
        val = cpu->registers.reg_LI;
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case AB:
        val = read_immediate(cpu); // value_lsb
        reg_msb = peek(&cpu->registers.reg_A);
        reg_lsb = peek(&cpu->registers.reg_B);
        cpu->zero_flag = reg_lsb == val && reg_msb == value;
        cpu->overflow_flag = reg_lsb > val || reg_msb > value;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        val = read_immediate(cpu); // value_lsb
        reg_msb = cpu->registers.reg_HI;
        reg_lsb = cpu->registers.reg_LI;
        cpu->zero_flag = reg_lsb == val && reg_msb == value;
        cpu->overflow_flag = reg_lsb > val || reg_msb > value;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void compare_register_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    data_bus_t reg_msb, reg_lsb, value_msb, value_lsb;
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        compare_register_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        reg_msb = peek(&cpu->registers.reg_A);
        reg_lsb = peek(&cpu->registers.reg_B);
        cpu->zero_flag = reg_lsb == value_lsb && reg_msb == value_msb;
        cpu->overflow_flag = reg_lsb > value_lsb || reg_msb > value_msb;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        reg_msb = cpu->registers.reg_HI;
        reg_lsb = cpu->registers.reg_LI;
        cpu->zero_flag = reg_lsb == value_lsb && reg_msb == value_msb;
        cpu->overflow_flag = reg_lsb > value_lsb || reg_msb > value_msb;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

void compare_registers(Cpu *cpu, Register first, Register second)
{
    // only A and B
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->zero_flag = val_a == val_b;
    cpu->overflow_flag = val_a > val_b;
    delay_for_n_clock_ticks(cpu->clock, 1);
}