#include "../cpu.h"

void peek_register_absolute(Cpu *cpu, Register source, address_bus_t address)
{
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    default:
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void peek_register_indirect(Cpu *cpu, Register source)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}
