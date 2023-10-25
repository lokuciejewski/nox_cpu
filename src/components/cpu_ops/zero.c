#include "../cpu.h"

void zero_register(Cpu *cpu, Register target)
{
    switch (target)
    {
    case HI:
        cpu->registers.reg_HI = 0;
        break;
    case LI:
        cpu->registers.reg_LI = 0;
        break;
    case HLI:
        cpu->registers.reg_HI = 0;
        cpu->registers.reg_LI = 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}