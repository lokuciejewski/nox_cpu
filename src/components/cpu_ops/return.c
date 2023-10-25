#include "../cpu.h"

void return_ok(Cpu *cpu)
{
    // Recreate old stack
    cpu->stack_address = 0; // to allow popping from the previous stack

    data_bus_t stack_address_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_address_msb = pop_value_from_stack(cpu);
    cpu->stack_address = from_2_data_bus_t(stack_address_msb, stack_address_lsb);

    data_bus_t stack_size_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_size_msb = pop_value_from_stack(cpu);
    cpu->stack_size = from_2_data_bus_t(stack_size_msb, stack_size_lsb);

    // Pop program_counter
    data_bus_t program_counter_lsb = pop_value_from_stack(cpu);
    data_bus_t program_counter_msb = pop_value_from_stack(cpu);
    cpu->program_counter = from_2_data_bus_t(program_counter_msb, program_counter_lsb);

    // Pop HLI
    pop_register_from_stack(cpu, LI, 0);
    pop_register_from_stack(cpu, HI, 0);

    // Pop flags
    data_bus_t flags = pop_value_from_stack(cpu);

    cpu->overflow_flag = (flags & 0b10000000) != 0;
    cpu->zero_flag = (flags & 0b01000000) != 0;
    cpu->irq_flag = (flags & 0b00100000) != 0;

    // Clear err flag and set exit code
    cpu->err_flag = false;
    cpu->registers.reg_EXIT_CODE = flags & 0b00001111;
}

void return_ok_with_code(Cpu *cpu, uint8_t exit_code)
{
    // Recreate old stack
    cpu->stack_address = 0; // to allow popping from the previous stack

    data_bus_t stack_address_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_address_msb = pop_value_from_stack(cpu);
    cpu->stack_address = from_2_data_bus_t(stack_address_msb, stack_address_lsb);

    data_bus_t stack_size_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_size_msb = pop_value_from_stack(cpu);
    cpu->stack_size = from_2_data_bus_t(stack_size_msb, stack_size_lsb);

    // Pop program_counter
    data_bus_t program_counter_lsb = pop_value_from_stack(cpu);
    data_bus_t program_counter_msb = pop_value_from_stack(cpu);
    cpu->program_counter = from_2_data_bus_t(program_counter_msb, program_counter_lsb);

    // Pop HLI
    pop_register_from_stack(cpu, LI, 0);
    pop_register_from_stack(cpu, HI, 0);

    // Pop flags
    data_bus_t flags = pop_value_from_stack(cpu);

    cpu->overflow_flag = (flags & 0b10000000) != 0;
    cpu->zero_flag = (flags & 0b01000000) != 0;
    cpu->irq_flag = (flags & 0b00100000) != 0;

    // Clear err flag and set exit code
    cpu->err_flag = false;
    cpu->registers.reg_EXIT_CODE = exit_code & 0b00001111;
}

void return_err(Cpu *cpu)
{
    // Recreate old stack
    cpu->stack_address = 0; // to allow popping from the previous stack

    data_bus_t stack_address_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_address_msb = pop_value_from_stack(cpu);
    cpu->stack_address = from_2_data_bus_t(stack_address_msb, stack_address_lsb);

    data_bus_t stack_size_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_size_msb = pop_value_from_stack(cpu);
    cpu->stack_size = from_2_data_bus_t(stack_size_msb, stack_size_lsb);

    // Pop program_counter
    data_bus_t program_counter_lsb = pop_value_from_stack(cpu);
    data_bus_t program_counter_msb = pop_value_from_stack(cpu);
    cpu->program_counter = from_2_data_bus_t(program_counter_msb, program_counter_lsb);

    // Pop HLI
    pop_register_from_stack(cpu, LI, 0);
    pop_register_from_stack(cpu, HI, 0);

    // Pop flags
    data_bus_t flags = pop_value_from_stack(cpu);

    cpu->overflow_flag = (flags & 0b10000000) != 0;
    cpu->zero_flag = (flags & 0b01000000) != 0;
    cpu->irq_flag = (flags & 0b00100000) != 0;

    // Set err flag and set exit code
    cpu->err_flag = true;
    cpu->registers.reg_EXIT_CODE = flags & 0b00001111;
}

void return_err_with_code(Cpu *cpu, uint8_t exit_code)
{
    // Recreate old stack
    cpu->stack_address = 0; // to allow popping from the previous stack

    data_bus_t stack_address_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_address_msb = pop_value_from_stack(cpu);
    cpu->stack_address = from_2_data_bus_t(stack_address_msb, stack_address_lsb);

    data_bus_t stack_size_lsb = pop_value_from_stack(cpu);
    data_bus_t stack_size_msb = pop_value_from_stack(cpu);
    cpu->stack_size = from_2_data_bus_t(stack_size_msb, stack_size_lsb);

    // Pop program_counter
    data_bus_t program_counter_lsb = pop_value_from_stack(cpu);
    data_bus_t program_counter_msb = pop_value_from_stack(cpu);
    cpu->program_counter = from_2_data_bus_t(program_counter_msb, program_counter_lsb);

    // Pop HLI
    pop_register_from_stack(cpu, LI, 0);
    pop_register_from_stack(cpu, HI, 0);

    // Pop flags
    data_bus_t flags = pop_value_from_stack(cpu);

    cpu->overflow_flag = (flags & 0b10000000) != 0;
    cpu->zero_flag = (flags & 0b01000000) != 0;
    cpu->irq_flag = (flags & 0b00100000) != 0;

    // Set err flag and set exit code
    cpu->err_flag = true;
    cpu->registers.reg_EXIT_CODE = exit_code & 0b00001111;
}