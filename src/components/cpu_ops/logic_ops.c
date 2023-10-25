#include "../cpu.h"

void perform_logic_operation_on_registers(Cpu *cpu, Register target, LogicOp op, Register second)
{
    switch (target)
    {
    case A:
        switch (op)
        {
        case And:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) & peek(&cpu->registers.reg_B));
            break;
        case Or:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) | peek(&cpu->registers.reg_B));
            break;
        case Xor:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) ^ peek(&cpu->registers.reg_B));
            break;
        }
        cpu->zero_flag = peek(&cpu->registers.reg_A) == 0;
        break;
    case B:
        switch (op)
        {
        case And:
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) & peek(&cpu->registers.reg_A));
            break;
        case Or:
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) | peek(&cpu->registers.reg_A));
            break;
        case Xor:
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) ^ peek(&cpu->registers.reg_A));
            break;
        }
        cpu->zero_flag = peek(&cpu->registers.reg_B) == 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void perform_logic_operation_immediate(Cpu *cpu, Register target, LogicOp op, data_bus_t value)
{
    if (target == AB)
    {
        data_bus_t value_lsb = read_immediate(cpu);
        switch (op)
        {
        case And:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) & value);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) & value_lsb);
            break;
        case Or:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) | value);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) | value_lsb);
            break;
        case Xor:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) ^ value);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) ^ value_lsb);
            break;
        }
        cpu->zero_flag = peek(&cpu->registers.reg_A) == peek(&cpu->registers.reg_B) == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
    }
}

void perform_logic_operation_absolute(Cpu *cpu, Register target, LogicOp op, address_bus_t address)
{
    if (target == AB)
    {
        set_address_bus(cpu, address);
        data_bus_t value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        data_bus_t reg_msb = peek(&cpu->registers.reg_A);
        data_bus_t reg_lsb = peek(&cpu->registers.reg_B);
        switch (op)
        {
        case And:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) & value_msb);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) & value_lsb);
            break;
        case Or:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) | value_msb);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) | value_lsb);
            break;
        case Xor:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_A) ^ value_msb);
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_B) ^ value_lsb);
            break;
        }
        cpu->zero_flag = peek(&cpu->registers.reg_A) == peek(&cpu->registers.reg_B) == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
    }
}

void perform_logic_operation_not(Cpu *cpu, Register target)
{
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, ~peek(&cpu->registers.reg_A));
        cpu->zero_flag = peek(&cpu->registers.reg_A) == 0;
        break;
    case B:
        push(&cpu->registers.reg_B, ~peek(&cpu->registers.reg_B));
        cpu->zero_flag = peek(&cpu->registers.reg_B) == 0;
        break;
    case AB:
        push(&cpu->registers.reg_A, ~peek(&cpu->registers.reg_A));
        push(&cpu->registers.reg_B, ~peek(&cpu->registers.reg_B));
        cpu->zero_flag = peek(&cpu->registers.reg_A) == peek(&cpu->registers.reg_B) == 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}