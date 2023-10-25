#include "cpu.h"

Cpu *init_cpu(Clock *clock, address_bus_t *address_bus, data_bus_t *data_bus, bool *sig_write, bool *sig_irq)
{
    Cpu *cpu = malloc(sizeof(Cpu));
    cpu->clock = clock;
    cpu->registers.reg_A = *init_stack(8);
    cpu->registers.reg_B = *init_stack(8);
    cpu->buses.address_bus = address_bus;
    cpu->buses.data_bus = data_bus;
    cpu->buses.sig_write = sig_write;
    cpu->buses.sig_irq = sig_irq;
    cpu->irq_flag = false;
    cpu->err_flag = false;
    cpu->overflow_flag = false;
    cpu->zero_flag = false;
    return cpu;
};

void print_cpu(Cpu *cpu)
{
    printf("\t==== CPU ====\n");
    printf("A:\tB:\tHI: 0x%02x\tLI: 0x%02x\n",
           cpu->registers.reg_HI, cpu->registers.reg_LI);
    for (uint8_t i = cpu->registers.reg_A.stack_size; i > 0; i--)
    {
        uint8_t current_A = (uint8_t)(cpu->registers.reg_A.stack_ptr - i) % cpu->registers.reg_A.stack_size;
        uint8_t current_B = (uint8_t)(cpu->registers.reg_B.stack_ptr - i) % cpu->registers.reg_B.stack_size;
        printf("0x%02x\t0x%02x\n", cpu->registers.reg_A.stack[current_A], cpu->registers.reg_B.stack[current_B]);
    }
    printf("Address bus: 0x%04x\tData bus: 0x%02x\n", *cpu->buses.address_bus, *cpu->buses.data_bus);
    printf("PC: 0x%04x\tSP: 0x%04x\tIRQ_ADDR: 0x%04x\n", cpu->program_counter, cpu->stack_pointer, cpu->irq_address);
    printf("WRT: %d\t IRQ: %d\n", *cpu->buses.sig_write, *cpu->buses.sig_irq);
    printf("OVF: %d\tZERO: %d\tIRQ: %d\tERR:%d\tEXIT_CODE: 0x%02x\n",
           cpu->overflow_flag, cpu->zero_flag, cpu->irq_flag, cpu->err_flag, cpu->registers.reg_EXIT_CODE);
}

void run(Cpu *cpu, bool print_status, bool step_by_instruction, bool exit_on_halt)
{
    while (1)
    {
        if (print_status)
            print_cpu(cpu);
        if (*cpu->buses.sig_irq && cpu->irq_flag)
        {
            call(cpu, cpu->irq_address);
        }
        else
        {
            set_address_bus(cpu, cpu->program_counter);
            data_bus_t next_instruction = read_data(cpu);
            cpu->program_counter++;
            execute_instruction(cpu, next_instruction, exit_on_halt);
            if (step_by_instruction)
            {
                getchar();
            }
        }
    }
}

void execute_instruction(Cpu *cpu, data_bus_t instruction, bool exit_on_halt)
{
    delay_for_n_clock_ticks(cpu->clock, DEFAULT_OP_DELAY_TICKS);
    switch (instruction)
    {
    case PUSH_IMMEDIATE_A:
        push_register_immediate(cpu, A, read_immediate(cpu));
        break;
    case PUSH_IMMEDIATE_B:
        push_register_immediate(cpu, B, read_immediate(cpu));
        break;
    case PUSH_ABSOLUTE_A:
        push_register_absolute(cpu, A, read_address(cpu));
        break;
    case PUSH_ABSOLUTE_B:
        push_register_absolute(cpu, B, read_address(cpu));
        break;
    case PUSH_INDIRECT_A:
        push_register_indirect(cpu, A);
        break;
    case PUSH_INDIRECT_B:
        push_register_indirect(cpu, B);
        break;
    case PUSH_A_B:
        push_register_to_register(cpu, A, B);
        break;
    case PUSH_B_A:
        push_register_to_register(cpu, B, A);
        break;
    case PUSH_HI_A:
        push_register_to_register(cpu, HI, A);
        break;
    case PUSH_LI_A:
        push_register_to_register(cpu, LI, A);
        break;
    case PUSH_HI_B:
        push_register_to_register(cpu, HI, B);
        break;
    case PUSH_LI_B:
        push_register_to_register(cpu, LI, B);
        break;
    case PUSH_EXIT_CODE_A:
        push_register_to_register(cpu, EXIT_CODE, A);
        break;
    case PUSH_EXIT_CODE_B:
        push_register_to_register(cpu, EXIT_CODE, B);
        break;
    case POP_A:
        pop(&cpu->registers.reg_A);
        break;
    case POP_B:
        pop(&cpu->registers.reg_B);
        break;
    case POP_A_ABSOLUTE:
        pop_register_absolute(cpu, A, read_address(cpu));
        break;
    case POP_B_ABSOLUTE:
        pop_register_absolute(cpu, B, read_address(cpu));
        break;
    case POP_A_INDIRECT:
        pop_register_indirect(cpu, A);
        break;
    case POP_B_INDIRECT:
        pop_register_indirect(cpu, B);
        break;
    case POP_A_B:
        pop_register_to_register(cpu, A, B);
        break;
    case POP_B_A:
        pop_register_to_register(cpu, B, A);
        break;
    case POP_A_HI:
        pop_register_to_register(cpu, A, HI);
        break;
    case POP_A_LI:
        pop_register_to_register(cpu, A, LI);
        break;
    case POP_B_HI:
        pop_register_to_register(cpu, B, HI);
        break;
    case POP_B_LI:
        pop_register_to_register(cpu, B, LI);
        break;
    case PEEK_A_ABSOLUTE:
        peek_register_absolute(cpu, A, read_address(cpu));
        break;
    case PEEK_B_ABSOLUTE:
        peek_register_absolute(cpu, B, read_address(cpu));
        break;
    case PEEK_A_INDIRECT:
        peek_register_indirect(cpu, A);
        break;
    case PEEK_B_INDIRECT:
        peek_register_indirect(cpu, B);
        break;
    case ADD_A_B:
        add_registers(cpu, A, B);
        break;
    case ADD_B_A:
        add_registers(cpu, B, A);
        break;
    case ADD_IMMEDIATE_A:
        add_immediate(cpu, A, read_immediate(cpu));
        break;
    case ADD_IMMEDIATE_B:
        add_immediate(cpu, B, read_immediate(cpu));
        break;
    case ADD_ABSOLUTE_A:
        add_absolute(cpu, A, read_address(cpu));
        break;
    case ADD_ABSOLUTE_B:
        add_absolute(cpu, B, read_address(cpu));
    case SUB_A_B:
        sub_registers(cpu, A, B);
        break;
    case SUB_B_A:
        sub_registers(cpu, A, B);
        break;
    case SUB_IMMEDIATE_A:
        sub_immediate(cpu, A, read_immediate(cpu));
        break;
    case SUB_IMMEDIATE_B:
        sub_immediate(cpu, B, read_immediate(cpu));
        break;
    case SUB_ABSOLUTE_A:
        sub_absolute(cpu, A, read_address(cpu));
        break;
    case SUB_ABSOLUTE_B:
        sub_absolute(cpu, B, read_address(cpu));
        break;
    case SHIFT_LEFT_A:
        shift_register(cpu, A, true);
        break;
    case SHIFT_RIGHT_A:
        shift_register(cpu, A, false);
        break;
    case SHIFT_LEFT_B:
        shift_register(cpu, B, true);
        break;
    case SHIFT_RIGHT_B:
        shift_register(cpu, B, false);
        break;
    case AND_A_B:
        perform_logic_operation_on_registers(cpu, A, And, B);
        break;
    case AND_B_A:
        perform_logic_operation_on_registers(cpu, B, And, A);
        break;
    case OR_A_B:
        perform_logic_operation_on_registers(cpu, A, Or, B);
        break;
    case OR_B_A:
        perform_logic_operation_on_registers(cpu, B, Or, A);
        break;
    case XOR_A_B:
        perform_logic_operation_on_registers(cpu, A, Xor, B);
        break;
    case XOR_B_A:
        perform_logic_operation_on_registers(cpu, B, Xor, A);
        break;
    case NOT_A:
        perform_logic_operation_not(cpu, A);
        break;
    case NOT_B:
        perform_logic_operation_not(cpu, B);
        break;
    case CMP_A_B:
        compare_registers(cpu, A, B);
        break;
    case CMP_IMMEDIATE_A:
        compare_register_immediate(cpu, A, read_immediate(cpu));
        break;
    case CMP_IMMEDIATE_B:
        compare_register_immediate(cpu, B, read_immediate(cpu));
        break;
    case CMP_ABSOLUTE_A:
        compare_register_absolute(cpu, A, read_address(cpu));
        break;
    case CMP_ABSOLUTE_B:
        compare_register_absolute(cpu, B, read_address(cpu));
        break;
    case PUSH_IMMEDIATE_HI:
        push_register_immediate(cpu, HI, read_immediate(cpu));
        break;
    case PUSH_IMMEDIATE_LI:
        push_register_immediate(cpu, LI, read_immediate(cpu));
        break;
    case PUSH_ABSOLUTE_HI:
        push_register_absolute(cpu, HI, read_address(cpu));
        break;
    case PUSH_ABSOLUTE_LI:
        push_register_absolute(cpu, LI, read_address(cpu));
        break;
    case STORE_HI_ABSOLUTE:
        pop_register_absolute(cpu, HI, read_address(cpu));
        break;
    case STORE_LI_ABSOLUTE:
        pop_register_absolute(cpu, LI, read_address(cpu));
        break;
    case CMP_IMMEDIATE_HI:
        compare_register_immediate(cpu, HI, read_immediate(cpu));
        break;
    case CMP_IMMEDIATE_LI:
        compare_register_immediate(cpu, LI, read_immediate(cpu));
        break;
    case CMP_ABSOLUTE_HI:
        compare_register_absolute(cpu, HI, read_address(cpu));
        break;
    case CMP_ABSOLUTE_LI:
        compare_register_absolute(cpu, LI, read_address(cpu));
        break;
    case INC_HI:
        increment_register(cpu, HI);
        break;
    case INC_LI:
        increment_register(cpu, LI);
        break;
    case DEC_HI:
        decrement_register(cpu, HI);
        break;
    case DEC_LI:
        decrement_register(cpu, LI);
        break;
    case ZERO_HI:
        zero_register(cpu, HI);
        break;
    case ZERO_LI:
        zero_register(cpu, LI);
        break;
    case SWAP_HI_LI:
        pop_register_to_register(cpu, HI, LI);
        break;
    case PUSH_IMMEDIATE_AB:
        push_register_immediate(cpu, AB, read_immediate(cpu));
        break;
    case PUSH_ABSOLUTE_AB:
        push_register_absolute(cpu, AB, read_address(cpu));
        break;
    case PUSH_INDIRECT_AB:
        push_register_indirect(cpu, AB);
        break;
    case PUSH_HLI_AB:
        push_register_to_register(cpu, HLI, AB);
        break;
    case POP_AB_ABSOLUTE:
        pop_register_absolute(cpu, AB, read_address(cpu));
        break;
    case POP_AB_INDIRECT:
        pop_register_indirect(cpu, AB);
        break;
    case POP_AB_HLI:
        pop_register_to_register(cpu, AB, HLI);
        break;
    case PEEK_AB_ABSOLUTE:
        peek_register_absolute(cpu, AB, read_address(cpu));
        break;
    case PEEK_AB_INDIRECT:
        peek_register_indirect(cpu, AB);
        break;
    case ADD_IMMEDIATE_AB:
        add_immediate(cpu, AB, read_immediate(cpu));
        break;
    case ADD_ABSOLUTE_AB:
        sub_absolute(cpu, AB, read_address(cpu));
        break;
    case SUB_IMMEDIATE_AB:
        sub_immediate(cpu, AB, read_immediate(cpu));
        break;
    case SUB_ABSOLUTE_AB:
        sub_absolute(cpu, AB, read_address(cpu));
        break;
    case SHIFT_LEFT_AB:
        shift_register(cpu, AB, true);
        break;
    case SHIFT_RIGHT_AB:
        shift_register(cpu, AB, false);
        break;
    case AND_IMMEDIATE_AB:
        perform_logic_operation_immediate(cpu, AB, And, read_immediate(cpu));
        break;
    case AND_ABSOLUTE_AB:
        perform_logic_operation_absolute(cpu, AB, And, read_address(cpu));
        break;
    case OR_IMMEDIATE_AB:
        perform_logic_operation_immediate(cpu, AB, Or, read_immediate(cpu));
        break;
    case OR_ABSOLUTE_AB:
        perform_logic_operation_absolute(cpu, AB, Or, read_address(cpu));
        break;
    case XOR_IMMEDIATE_AB:
        perform_logic_operation_immediate(cpu, AB, Xor, read_immediate(cpu));
        break;
    case XOR_ABSOLUTE_AB:
        perform_logic_operation_immediate(cpu, AB, Xor, read_address(cpu));
        break;
    case NOT_AB:
        perform_logic_operation_not(cpu, AB);
        break;
    case PUSH_IMMEDIATE_HLI:
        push_register_immediate(cpu, HLI, read_immediate(cpu));
        break;
    case PUSH_ABSOLUTE_HLI:
        push_register_absolute(cpu, HLI, read_immediate(cpu));
        break;
    case STORE_HLI_ABSOLUTE:
        pop_register_absolute(cpu, HLI, read_address(cpu));
        break;
    case CMP_IMMEDIATE_AB:
        compare_register_immediate(cpu, AB, read_immediate(cpu));
        break;
    case CMP_ABSOLUTE_AB:
        compare_register_absolute(cpu, AB, read_address(cpu));
        break;
    case CMP_IMMEDIATE_HLI:
        compare_register_immediate(cpu, HLI, read_immediate(cpu));
        break;
    case CMP_ABSOLUTE_HLI:
        compare_register_absolute(cpu, HLI, read_address(cpu));
        break;
    case INC_HLI:
        increment_register(cpu, HLI);
        break;
    case DEC_HLI:
        decrement_register(cpu, HLI);
        break;
    case ZERO_HLI:
        zero_register(cpu, HLI);
        break;
    case JUMP_IF_ZERO:
        jump_if_zero(cpu, read_address(cpu));
        break;
    case JUMP_IF_OVERFLOW:
        jump_if_overflow(cpu, read_address(cpu));
        break;
    case JUMP_IF_ERROR:
        jump_if_error(cpu, read_address(cpu));
        break;
    case JUMP_IF_OK:
        jump_if_ok(cpu, read_address(cpu));
        break;
    case JUMP:
        jump(cpu, read_address(cpu));
        break;
    case PUSH_AB_STACK_ADDRESS:
        push_stack_address_to_ab(cpu);
        break;
    case PUSH_AB_STACK_SIZE:
        push_stack_size_to_ab(cpu);
        break;
    case POP_STACK_ADDRESS_AB:
        pop_ab_to_stack_address(cpu);
        break;
    case POP_STACK_SIZE_AB:
        pop_ab_to_stack_size(cpu);
        break;
    case POP_AB_IRQ:
        pop_ab_to_irq_address(cpu);
        break;
    case PUSH_A_STACK:
        push_register_to_stack(cpu, A, read_immediate(cpu));
        break;
    case POP_A_STACK:
        pop_register_to_stack(cpu, A, read_immediate(cpu));
        break;
    case POP_STACK_A:
        pop_register_from_stack(cpu, A, read_immediate(cpu));
        break;
    case PUSH_B_STACK:
        push_register_to_stack(cpu, B, read_immediate(cpu));
        break;
    case POP_B_STACK:
        pop_register_to_stack(cpu, B, read_immediate(cpu));
        break;
    case POP_STACK_B:
        pop_register_from_stack(cpu, B, read_immediate(cpu));
        break;
    case PUSH_HI_STACK:
        push_register_to_stack(cpu, HI, 0);
        break;
    case POP_STACK_HI:
        pop_register_from_stack(cpu, HI, 0);
        break;
    case PUSH_LI_STACK:
        push_register_to_stack(cpu, LI, 0);
        break;
    case POP_STACK_LI:
        pop_register_from_stack(cpu, LI, 0);
        break;
    case CALL:
        call(cpu, read_address(cpu));
        break;
    case RETURN_OK:
        return_ok(cpu);
        break;
    case RETURN_OK_EXIT_CODE:
        return_ok_with_code(cpu, read_immediate(cpu));
        break;
    case RETURN_ERR:
        return_err(cpu);
        break;
    case RETURN_ERR_EXIT_CODE:
        return_err_with_code(cpu, read_immediate(cpu));
        break;
    case SET_ERR:
        set_flag(cpu, Err);
        break;
    case SET_IRQ:
        set_flag(cpu, Irq);
        break;
    case CLEAR_EXIT_CODE:
        cpu->registers.reg_EXIT_CODE = 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case CLEAR_ERR:
        clear_flag(cpu, Err);
        break;
    case CLEAR_IRQ:
        clear_flag(cpu, Irq);
        break;
    case CLEAR_OVF:
        clear_flag(cpu, Ovf);
        break;
    case CLEAR_ZERO:
        clear_flag(cpu, Zero);
        break;
    case HALT:
        if (exit_on_halt)
            exit(0);
        cpu->program_counter--;
    default:
        noop(cpu);
    }
}

// reading data/address
address_bus_t read_address(Cpu *cpu)
{
    uint8_t address_data_multiplier = sizeof(address_bus_t) / sizeof(data_bus_t);
    address_bus_t address = 0;
    for (uint8_t i = address_data_multiplier; i > 0; i--)
    {
        set_address_bus(cpu, cpu->program_counter++);
        address |= ((address_bus_t)*cpu->buses.data_bus << (sizeof(data_bus_t) * 8 * (i - 1)));
    }
    return address;
}

data_bus_t read_immediate(Cpu *cpu)
{
    set_address_bus(cpu, cpu->program_counter);
    cpu->program_counter++;
    return read_data(cpu);
}

data_bus_t read_data(Cpu *cpu)
{
    return *cpu->buses.data_bus;
}

// Double register operations
void push_to_double_reg(Cpu *cpu, Register target, double_data_bus_t value)
{
    switch (target)
    {
    case AB:
        push(&cpu->registers.reg_A, (value & 0xff00) >> 8);
        push(&cpu->registers.reg_B, value & 0x00ff);
        break;
    case HLI:
        cpu->registers.reg_HI = (value & 0xff00) >> 8;
        cpu->registers.reg_LI = value & 0x00ff;
        break;
    }
}

double_data_bus_t pop_from_double_reg(Cpu *cpu, Register source)
{
    switch (source)
    {
    case AB:

        return from_2_data_bus_t(pop(&cpu->registers.reg_A), pop(&cpu->registers.reg_B));
    case HLI:

        return from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    }
}

double_data_bus_t from_2_data_bus_t(data_bus_t msb, data_bus_t lsb)
{
    double_data_bus_t result = (double_data_bus_t)lsb;
    result |= ((double_data_bus_t)msb) << 8;
    return result;
}

// Elementary operations
void set_address_bus(Cpu *cpu, address_bus_t value)
{
    *cpu->buses.address_bus = value;
    delay_for_n_clock_ticks(cpu->clock, SET_ADDRESS_BUS_DELAY_TICKS);
}

void set_data_bus(Cpu *cpu, data_bus_t value)
{
    *cpu->buses.data_bus = value;
    delay_for_n_clock_ticks(cpu->clock, SET_DATA_BUS_DELAY_TICKS);
}

void set_write_signal(Cpu *cpu, bool write)
{
    *cpu->buses.sig_write = write;
}
