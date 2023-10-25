#include "../cpu.h"

void shift_register(Cpu *cpu, Register target, bool left)
{
    data_bus_t value;
    switch (target)
    {
    case A:
        value = pop(&cpu->registers.reg_A);
        if (left)
        {
            push(&cpu->registers.reg_A, value << 1);
        }
        else
        {
            push(&cpu->registers.reg_A, value >> 1);
        }
        break;
    case B:
        value = pop(&cpu->registers.reg_B);
        if (left)
        {
            push(&cpu->registers.reg_B, value << 1);
        }
        else
        {
            push(&cpu->registers.reg_B, value >> 1);
        }
        break;
    case AB:
        data_bus_t value_msb = pop(&cpu->registers.reg_A);
        data_bus_t value_lsb = pop(&cpu->registers.reg_B);
        double_data_bus_t dvalue = from_2_data_bus_t(value_msb, value_lsb);
        if (left)
        {
            dvalue = dvalue << 1;
        }
        else
        {
            dvalue = dvalue >> 1;
        }
        push(&cpu->registers.reg_A, (dvalue & 0xff00) >> 8);
        push(&cpu->registers.reg_B, (dvalue & 0x00ff));
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}