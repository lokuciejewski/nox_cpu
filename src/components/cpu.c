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

// CPU operations
void push_register_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t value_lsb;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, value);
        break;
    case B:
        push(&cpu->registers.reg_B, value);
        break;
    case HI:
        cpu->registers.reg_HI = value;
        break;
    case LI:
        cpu->registers.reg_LI = value;
        break;
    case AB:
        value_lsb = read_immediate(cpu);
        push_to_double_reg(cpu, AB, from_2_data_bus_t(value, value_lsb));
        cpu->zero_flag = value == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        return;
    case HLI:
        value_lsb = read_immediate(cpu);
        push_to_double_reg(cpu, HLI, from_2_data_bus_t(value, value_lsb));
        cpu->zero_flag = value == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        return;
    default:
        break;
    }
    cpu->zero_flag = value == 0;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_register_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    data_bus_t value_msb, value_lsb;
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        push_register_immediate(cpu, target, value);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case AB:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        push_to_double_reg(cpu, AB, from_2_data_bus_t(value_msb, value_lsb));
        cpu->zero_flag = value_msb == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        push_to_double_reg(cpu, HLI, from_2_data_bus_t(value_msb, value_lsb));
        cpu->zero_flag = value_msb == 0 && value_lsb == 0;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

void push_register_to_register(Cpu *cpu, Register source, Register target)
{
    switch (source)
    {
    case A:
        switch (target)
        {
        case B:
            push(&cpu->registers.reg_B, peek(&cpu->registers.reg_A));
            break;
        case HI:
            cpu->registers.reg_HI = peek(&cpu->registers.reg_A);
            break;
        case LI:
            cpu->registers.reg_LI = peek(&cpu->registers.reg_A);
            break;
        }
        break;
    case B:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, peek(&cpu->registers.reg_B));
            break;
        case HI:
            cpu->registers.reg_HI = peek(&cpu->registers.reg_B);
            break;
        case LI:
            cpu->registers.reg_LI = peek(&cpu->registers.reg_B);
            break;
        }
        break;
    case HI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_HI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_HI);
            break;
        case LI:
            cpu->registers.reg_LI = cpu->registers.reg_HI;
            break;
        }
        break;
    case LI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_LI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_LI);
            break;
        case LI:
            cpu->registers.reg_HI = cpu->registers.reg_LI;
            break;
        }
        break;
    case EXIT_CODE:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_EXIT_CODE  & 0b00001111);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_EXIT_CODE  & 0b00001111);
            break;
        }
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_register_indirect(Cpu *cpu, Register target)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    data_bus_t value = read_data(cpu);
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, value);
        cpu->zero_flag = value == 0;
        break;
    case B:
        push(&cpu->registers.reg_B, value);
        cpu->zero_flag = value == 0;
        break;
    case AB:
        push(&cpu->registers.reg_A, value);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        push(&cpu->registers.reg_B, value_lsb);
        cpu->zero_flag = value == 0 == value_lsb;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_absolute(Cpu *cpu, Register source, address_bus_t address)
{
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case HI:
        set_data_bus(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        set_data_bus(cpu, cpu->registers.reg_LI);
        break;
    case AB:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case HLI:
        set_data_bus(cpu, cpu->registers.reg_HI);
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, cpu->registers.reg_LI);
        break;
    default:
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void pop_register_to_register(Cpu *cpu, Register source, Register target)
{
    switch (source)
    {
    case A:
        switch (target)
        {
        case B:
            push(&cpu->registers.reg_B, pop(&cpu->registers.reg_A));
            break;
        case HI:
            cpu->registers.reg_HI = pop(&cpu->registers.reg_A);
            break;
        case LI:
            cpu->registers.reg_LI = pop(&cpu->registers.reg_A);
            break;
        }
        break;
    case B:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, pop(&cpu->registers.reg_B));
            break;
        case HI:
            cpu->registers.reg_HI = pop(&cpu->registers.reg_B);
            break;
        case LI:
            cpu->registers.reg_LI = pop(&cpu->registers.reg_B);
            break;
        }
        break;
    case HI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_HI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_HI);
            break;
        case LI:
            cpu->registers.reg_LI = cpu->registers.reg_HI;
            break;
        }
        break;
    case LI:
        switch (target)
        {
        case A:
            push(&cpu->registers.reg_A, cpu->registers.reg_LI);
            break;
        case B:
            push(&cpu->registers.reg_B, cpu->registers.reg_LI);
            break;
        case LI:
            cpu->registers.reg_HI = cpu->registers.reg_LI;
            break;
        }
        break;
    case EXIT_CODE:
        switch (target)
        {
        case A:
            cpu->registers.reg_EXIT_CODE = pop(&cpu->registers.reg_A) & 0b00001111;
            break;
        case B:
            cpu->registers.reg_EXIT_CODE = pop(&cpu->registers.reg_B) & 0b00001111;
            break;
        }
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_indirect(Cpu *cpu, Register source)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, pop(&cpu->registers.reg_B));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, pop(&cpu->registers.reg_A));
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void peek_register_absolute(Cpu *cpu, Register source, address_bus_t address)
{
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    default:
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void peek_register_indirect(Cpu *cpu, Register source)
{
    address_bus_t address = from_2_data_bus_t(cpu->registers.reg_HI, cpu->registers.reg_LI);
    set_address_bus(cpu, address);
    switch (source)
    {
    case A:
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    case B:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        break;
    case AB:
        set_data_bus(cpu, peek(&cpu->registers.reg_B));
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        set_address_bus(cpu, address + 1);
        set_data_bus(cpu, peek(&cpu->registers.reg_A));
        break;
    }
    set_write_signal(cpu, true);
    delay_for_n_clock_ticks(cpu->clock, 1);
    set_write_signal(cpu, false);
}

void add_registers(Cpu *cpu, Register summand, Register target)
{
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->overflow_flag = val_a > 127 && val_b > 127; // TODO: hardcoded values
    cpu->zero_flag = val_a == 0 == val_b;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, val_a + val_b);
        break;
    case B:
        push(&cpu->registers.reg_B, val_a + val_b);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void add_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->overflow_flag = val > 127 && value > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_A, val + value);
        cpu->zero_flag = peek(&cpu->registers.reg_A) == 0;
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->overflow_flag = val > 127 && value > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_B, val + value);
        cpu->zero_flag = peek(&cpu->registers.reg_B) == 0;
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void add_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        add_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        data_bus_t value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        data_bus_t val_a = peek(&cpu->registers.reg_A);
        data_bus_t val_b = peek(&cpu->registers.reg_B);
        bool overflow_lsb = val_b > 127 && value_lsb > 127;                 // TODO: hardcoded values
        cpu->overflow_flag = val_a + overflow_lsb > 127 && value_msb > 127; // TODO: hardcoded values
        push(&cpu->registers.reg_B, val_b + value_lsb);
        push(&cpu->registers.reg_A, val_a + value_msb + overflow_lsb);
        cpu->zero_flag = 0 == peek(&cpu->registers.reg_A) == peek(&cpu->registers.reg_B);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

void sub_registers(Cpu *cpu, Register minuend, Register target)
{
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->overflow_flag = val_a < val_b;
    cpu->zero_flag = val_a == val_b;
    switch (target)
    {
    case A:
        push(&cpu->registers.reg_A, val_a - val_b);
        break;
    case B:
        push(&cpu->registers.reg_B, val_b - val_a);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void sub_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val, val_msb, val_lsb;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->overflow_flag = val < value;
        cpu->zero_flag = val == value;
        push(&cpu->registers.reg_A, val - value);
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->overflow_flag = val < value;
        cpu->zero_flag = val == value;
        push(&cpu->registers.reg_B, val - value);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void sub_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        sub_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        data_bus_t value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        data_bus_t value_lsb = read_data(cpu);
        data_bus_t val_a = peek(&cpu->registers.reg_A);
        data_bus_t val_b = peek(&cpu->registers.reg_B);
        bool overflow_lsb = val_b < value_lsb;
        cpu->overflow_flag = val_a - overflow_lsb < value_msb || val_a == 0;
        push(&cpu->registers.reg_B, val_b - value_lsb);
        push(&cpu->registers.reg_A, val_a - value_msb - overflow_lsb);
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

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

void compare_register_immediate(Cpu *cpu, Register target, data_bus_t value)
{
    data_bus_t val, reg_msb, reg_lsb;
    switch (target)
    {
    case A:
        val = peek(&cpu->registers.reg_A);
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case B:
        val = peek(&cpu->registers.reg_B);
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case HI:
        val = cpu->registers.reg_HI;
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case LI:
        val = cpu->registers.reg_LI;
        cpu->zero_flag = val == value;
        cpu->overflow_flag = val > value;
        break;
    case AB:
        val = read_immediate(cpu); // value_lsb
        reg_msb = peek(&cpu->registers.reg_A);
        reg_lsb = peek(&cpu->registers.reg_B);
        cpu->zero_flag = reg_lsb == val && reg_msb == value;
        cpu->overflow_flag = reg_lsb > val || reg_msb > value;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        val = read_immediate(cpu); // value_lsb
        reg_msb = cpu->registers.reg_HI;
        reg_lsb = cpu->registers.reg_LI;
        cpu->zero_flag = reg_lsb == val && reg_msb == value;
        cpu->overflow_flag = reg_lsb > val || reg_msb > value;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void compare_register_absolute(Cpu *cpu, Register target, address_bus_t address)
{
    data_bus_t reg_msb, reg_lsb, value_msb, value_lsb;
    switch (target)
    {
    case A:
    case B:
    case HI:
    case LI:
        set_address_bus(cpu, address);
        data_bus_t value = read_data(cpu);
        compare_register_immediate(cpu, target, value);
        break;
    case AB:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        reg_msb = peek(&cpu->registers.reg_A);
        reg_lsb = peek(&cpu->registers.reg_B);
        cpu->zero_flag = reg_lsb == value_lsb && reg_msb == value_msb;
        cpu->overflow_flag = reg_lsb > value_lsb || reg_msb > value_msb;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    case HLI:
        set_address_bus(cpu, address);
        value_msb = read_data(cpu);
        set_address_bus(cpu, address + 1);
        value_lsb = read_data(cpu);
        reg_msb = cpu->registers.reg_HI;
        reg_lsb = cpu->registers.reg_LI;
        cpu->zero_flag = reg_lsb == value_lsb && reg_msb == value_msb;
        cpu->overflow_flag = reg_lsb > value_lsb || reg_msb > value_msb;
        delay_for_n_clock_ticks(cpu->clock, 1);
        break;
    }
}

void compare_registers(Cpu *cpu, Register first, Register second)
{
    // only A and B
    data_bus_t val_a = peek(&cpu->registers.reg_A);
    data_bus_t val_b = peek(&cpu->registers.reg_B);
    cpu->zero_flag = val_a == val_b;
    cpu->overflow_flag = val_a > val_b;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

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

void jump_if_zero(Cpu *cpu, address_bus_t address)
{
    if (cpu->zero_flag)
    {
        jump(cpu, address);
    }
}

void jump_if_overflow(Cpu *cpu, address_bus_t address)
{
    if (cpu->overflow_flag)
    {
        jump(cpu, address);
    }
}

void jump_if_error(Cpu *cpu, address_bus_t address)
{
    if (cpu->err_flag)
    {
        jump(cpu, address);
    }
}
void jump_if_ok(Cpu *cpu, address_bus_t address)
{
    if (!cpu->err_flag)
    {
        jump(cpu, address);
    }
}

void jump(Cpu *cpu, address_bus_t address)
{
    cpu->program_counter = address;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_value_to_stack(Cpu *cpu, data_bus_t value)
{
    if (cpu->stack_pointer < cpu->stack_address + cpu->stack_size)
    {
        set_address_bus(cpu, cpu->stack_pointer);
        set_data_bus(cpu, value);
        set_write_signal(cpu, true);
        delay_for_n_clock_ticks(cpu->clock, 1);
        set_write_signal(cpu, false);
        cpu->stack_pointer++;
        cpu->overflow_flag = cpu->stack_pointer == cpu->stack_address + cpu->stack_size;
    }
    else
    {
        cpu->overflow_flag = true;
        cpu->err_flag = true;
    }
}

data_bus_t pop_value_from_stack(Cpu *cpu)
{
    data_bus_t value = 0;
    if (cpu->stack_pointer > cpu->stack_address)
    {
        cpu->stack_pointer--;
        set_address_bus(cpu, cpu->stack_pointer);
        value = read_data(cpu);
        cpu->zero_flag = cpu->stack_pointer == cpu->stack_address;
    }
    else
    {
        cpu->zero_flag = true;
        cpu->err_flag = true;
    }
    return value;
}

void push_stack_address_to_ab(Cpu *cpu)
{
    push_to_double_reg(cpu, AB, cpu->stack_address);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_stack_size_to_ab(Cpu *cpu)
{
    push_to_double_reg(cpu, AB, cpu->stack_size);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_ab_to_stack_address(Cpu *cpu)
{
    cpu->stack_address = pop_from_double_reg(cpu, AB);
    cpu->stack_pointer = cpu->stack_address;
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_ab_to_stack_size(Cpu *cpu)
{
    cpu->stack_size = pop_from_double_reg(cpu, AB);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_ab_to_irq_address(Cpu *cpu)
{
    cpu->irq_address = pop_from_double_reg(cpu, AB);
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void push_register_to_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, peek(&cpu->registers.reg_A));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, peek(&cpu->registers.reg_B));
            n_of_values--;
        }
        break;
    case HI:
        push_value_to_stack(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        push_value_to_stack(cpu, cpu->registers.reg_LI);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_to_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, pop(&cpu->registers.reg_A));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push_value_to_stack(cpu, pop(&cpu->registers.reg_B));
            n_of_values--;
        }
        break;
    case HI:
        push_value_to_stack(cpu, cpu->registers.reg_HI);
        break;
    case LI:
        push_value_to_stack(cpu, cpu->registers.reg_LI);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

void pop_register_from_stack(Cpu *cpu, Register target, data_bus_t n_of_values)
{
    switch (target)
    {
    case A:
        while (n_of_values != 0)
        {
            push(&cpu->registers.reg_A, pop_value_from_stack(cpu));
            n_of_values--;
        }
        break;
    case B:
        while (n_of_values != 0)
        {
            push(&cpu->registers.reg_B, pop_value_from_stack(cpu));
            n_of_values--;
        }
        break;
    case HI:
        cpu->registers.reg_HI = pop_value_from_stack(cpu);
        break;
    case LI:
        cpu->registers.reg_LI = pop_value_from_stack(cpu);
        break;
    }
    delay_for_n_clock_ticks(cpu->clock, 1);
}

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