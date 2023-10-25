#include "../cpu.h"

void jump_if_zero(Cpu *cpu, address_bus_t address)
{
    if (cpu->zero_flag)
    {
        jump(cpu, address);
    }
}

void jump_if_overflow(Cpu *cpu, address_bus_t address)
{
    if (cpu->overflow_flag)
    {
        jump(cpu, address);
    }
}

void jump_if_error(Cpu *cpu, address_bus_t address)
{
    if (cpu->err_flag)
    {
        jump(cpu, address);
    }
}
void jump_if_ok(Cpu *cpu, address_bus_t address)
{
    if (!cpu->err_flag)
    {
        jump(cpu, address);
    }
}

void jump(Cpu *cpu, address_bus_t address)
{
    cpu->program_counter = address;
    delay_for_n_clock_ticks(cpu->clock, 1);
}