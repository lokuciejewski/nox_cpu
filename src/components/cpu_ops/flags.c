#include "../cpu.h"

void set_flag(Cpu *cpu, Flag flag)
{
    switch (flag)
    {
    case Ovf:
        cpu->overflow_flag = true;
        break;
    case Zero:
        cpu->overflow_flag = true;
        break;
    case Irq:
        cpu->irq_flag = true;
        break;
    case Err:
        cpu->err_flag = true;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void clear_flag(Cpu *cpu, Flag flag)
{
    switch (flag)
    {
    case Ovf:
        cpu->overflow_flag = false;
        break;
    case Zero:
        cpu->overflow_flag = false;
        break;
    case Irq:
        cpu->irq_flag = false;
        break;
    case Err:
        cpu->err_flag = false;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}