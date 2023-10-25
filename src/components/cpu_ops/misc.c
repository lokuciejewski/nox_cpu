#include "../cpu.h"

void noop(Cpu *cpu)
{
}

void halt(Cpu *cpu)
{
}

void reset(Cpu *cpu)
{
    // Zero out registers
    cpu->registers.reg_A.stack_ptr = 0;
    cpu->registers.reg_B.stack_ptr = 0;
    cpu->registers.reg_HI = 0;
    cpu->registers.reg_LI = 0;
    cpu->registers.reg_EXIT_CODE = 0;

    // Reset sig_write
    *cpu->buses.sig_write = false;

    // Reset flags
    cpu->irq_flag = false;
    cpu->err_flag = false;
    cpu->overflow_flag = false;
    cpu->zero_flag = false;

    // Stack set up
    push_register_absolute(cpu, AB, 0xfff8); // <- irq address
    push_register_absolute(cpu, AB, 0xfffa); // <- stack address
    push_register_absolute(cpu, AB, 0xfffc); // <- stack size
    pop_ab_to_stack_size(cpu);
    pop_ab_to_stack_address(cpu);
    pop_ab_to_irq_address(cpu);
    set_address_bus(cpu, 0xfffe);
    data_bus_t reset_vec_msb = read_data(cpu);
    set_address_bus(cpu, 0xffff);
    data_bus_t reset_vec_lsb = read_data(cpu);
    jump(cpu, from_2_data_bus_t(reset_vec_msb, reset_vec_lsb));
}