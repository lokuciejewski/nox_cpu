#include "../cpu.h"

void call(Cpu *cpu, address_bus_t address)
{
    // Push flags
    uint8_t flags = cpu->registers.reg_EXIT_CODE;
    flags |= (cpu->err_flag << 5);
    flags |= (cpu->irq_flag << 6);
    flags |= (cpu->zero_flag << 7);
    flags |= (cpu->overflow_flag << 8);
    push_value_to_stack(cpu, flags);

    // Push HLI
    push_register_to_stack(cpu, HI, 0);
    push_register_to_stack(cpu, LI, 0);

    // Push program_counter msb and lsb
    push_value_to_stack(cpu, (cpu->program_counter & 0xff00) >> 8);
    push_value_to_stack(cpu, (cpu->program_counter & 0x00ff));

    // Push stack_size and stack_address
    push_value_to_stack(cpu, (cpu->stack_size & 0xff00) >> 8);
    push_value_to_stack(cpu, (cpu->stack_size & 0x00ff));

    push_value_to_stack(cpu, (cpu->stack_address & 0xff00) >> 8);
    push_value_to_stack(cpu, (cpu->stack_address & 0x00ff));

    // init a new stack
    cpu->stack_size = cpu->stack_size - (cpu->stack_pointer - cpu->stack_address);
    cpu->stack_address = cpu->stack_pointer;

    jump(cpu, address);
}