#include "../cpu.h"

void decrement_register(Cpu *cpu, Register target)
{
    switch (target)
    {
    case HI:
        cpu->overflow_flag = cpu->registers.reg_HI == 0;
        cpu->registers.reg_HI--;
        cpu->zero_flag = cpu->registers.reg_HI == 0;
        break;
    case LI:
        cpu->overflow_flag = cpu->registers.reg_LI == 0;
        cpu->registers.reg_LI--;
        cpu->zero_flag = cpu->registers.reg_LI == 0;
        break;
    case HLI:
        cpu->overflow_flag = cpu->registers.reg_HI == cpu->registers.reg_LI == 0;
        cpu->registers.reg_HI -= cpu->registers.reg_LI == 0;
        cpu->registers.reg_LI--;
        cpu->zero_flag = cpu->registers.reg_HI == cpu->registers.reg_LI == 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}