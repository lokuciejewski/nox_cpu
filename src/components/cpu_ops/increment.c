#include "../cpu.h"

void increment_register(Cpu *cpu, Register target)
{
    switch (target)
    {
    case HI:
        cpu->registers.reg_HI++;
        cpu->zero_flag = cpu->registers.reg_HI == 0;
        break;
    case LI:
        cpu->registers.reg_LI++;
        cpu->zero_flag = cpu->registers.reg_LI == 0;
        break;
    case HLI:
        cpu->registers.reg_LI++;
        cpu->registers.reg_HI += cpu->registers.reg_LI == 0;
        cpu->zero_flag = cpu->registers.reg_HI == 0;
        break;
    }
    cpu->overflow_flag = cpu->zero_flag;
    delay_for_n_clock_ticks(cpu->clock, 1);
}