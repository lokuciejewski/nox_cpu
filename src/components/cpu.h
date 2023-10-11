#pragma once

#include "stdbool.h"
#include "stdio.h"

#include "components.h"
#include "opcodes.h"
#include "clock.h"

typedef struct Cpu
{
    Registers registers;
    Buses buses;
    Clock *clock;

    address_bus_t program_counter;

    address_bus_t irq_address; // Address to jump to when IRQ signal goes high

    address_bus_t stack_pointer;
    address_bus_t stack_address; // Starting address of stack. Stack grows upwards (towards the max possible address)
    address_bus_t stack_size;    // Stack size, set up by using POP_STACK_SIZE_AB
                                 // If not set up before setting the `stack_address`, the stack size is only limited by the memory size - unsafe!
    /*
    Example for 8 bit A/B registers using POP_STACK_SIZE_AB:
    PUSH_AB_IMMEDIATE 0x1f00 <- `stack_size`
    PUSH_AB_IMMEDIATE 0x1000 <- `stack_address`
    POP_AB_STACK_ADDRESS <- sets `stack_address` to 0x1000
    POP_STACK_SIZE_AB <- sets `stack_size` to 0x1f00
    */

    bool overflow_flag;
    bool zero_flag;
    bool irq_flag;
    bool err_flag;
} Cpu;

Cpu *init_cpu(Clock *clock, address_bus_t *address_bus, data_bus_t *data_bus, bool *sig_write, bool *sig_irq);
void print_cpu(Cpu *cpu);
void run(Cpu *cpu, bool print_status, bool step_by_instruction, bool exit_on_halt);
void execute_instruction(Cpu *cpu, data_bus_t instruction, bool exit_on_halt);

address_bus_t read_address(Cpu *cpu);
data_bus_t read_data(Cpu *cpu);
data_bus_t read_immediate(Cpu *cpu);

typedef enum Register
{
    // 8 bit
    A,
    B,
    HI,
    LI,
    EXIT_CODE, // 4 bit register
               // 16 bit
    AB,
    HLI,
} Register;

void set_address_bus(Cpu *cpu, address_bus_t value);
void set_data_bus(Cpu *cpu, data_bus_t value);
void set_write_signal(Cpu *cpu, bool write);

void push_to_double_reg(Cpu *cpu, Register target, double_data_bus_t value);
double_data_bus_t pop_from_double_reg(Cpu *cpu, Register source);
double_data_bus_t from_2_data_bus_t(data_bus_t msb, data_bus_t lsb);

void push_register_immediate(Cpu *cpu, Register target, data_bus_t value);
void push_register_absolute(Cpu *cpu, Register target, address_bus_t address);
void push_register_to_register(Cpu *cpu, Register source, Register target);
void push_register_indirect(Cpu *cpu, Register target);

void pop_register_absolute(Cpu *cpu, Register source, address_bus_t address);
void pop_register_to_register(Cpu *cpu, Register source, Register target);
void pop_register_indirect(Cpu *cpu, Register source);

void peek_register_absolute(Cpu *cpu, Register source, address_bus_t address);
void peek_register_indirect(Cpu *cpu, Register source);

void add_registers(Cpu *cpu, Register summand, Register target);
void add_immediate(Cpu *cpu, Register target, data_bus_t value);
void add_absolute(Cpu *cpu, Register target, address_bus_t address);

void sub_registers(Cpu *cpu, Register minuend, Register target);
void sub_immediate(Cpu *cpu, Register target, data_bus_t value);
void sub_absolute(Cpu *cpu, Register target, address_bus_t address);

void shift_register(Cpu *cpu, Register target, bool left);

void compare_register_immediate(Cpu *cpu, Register target, data_bus_t value);
void compare_register_absolute(Cpu *cpu, Register target, address_bus_t address);
void compare_registers(Cpu *cpu, Register first, Register second);

void increment_register(Cpu *cpu, Register target);
void decrement_register(Cpu *cpu, Register target);
void zero_register(Cpu *cpu, Register target);

typedef enum LogicOp
{
    And,
    Or,
    Xor,
    Not
} LogicOp;

void perform_logic_operation_on_registers(Cpu *cpu, Register target, LogicOp op, Register second);
void perform_logic_operation_immediate(Cpu *cpu, Register target, LogicOp op, data_bus_t value);
void perform_logic_operation_absolute(Cpu *cpu, Register target, LogicOp op, address_bus_t address);
void perform_logic_operation_not(Cpu *cpu, Register target);

void jump_if_zero(Cpu *cpu, address_bus_t address);
void jump_if_overflow(Cpu *cpu, address_bus_t address);
void jump_if_error(Cpu *cpu, address_bus_t address);
void jump_if_ok(Cpu *cpu, address_bus_t address);
void jump(Cpu *cpu, address_bus_t address);

// Pushing the value on stack sets the OVF flag if the stack is full after pushing
// If the stack is full, the ERR and OVF flags are set and the value is not pushed
void push_value_to_stack(Cpu *cpu, data_bus_t value);

// Popping the value from stack sets the ZERO flag if the stack is empty after popping
// If the stack is empty, the ERR and ZERO flags are set and no value is popped
data_bus_t pop_value_from_stack(Cpu *cpu);

void push_stack_address_to_ab(Cpu *cpu);
void push_stack_size_to_ab(Cpu *cpu);

void pop_ab_to_stack_address(Cpu *cpu);
void pop_ab_to_stack_size(Cpu *cpu);
void pop_ab_to_irq_address(Cpu *cpu);

void push_register_to_stack(Cpu *cpu, Register target, data_bus_t n_of_values);
void pop_register_from_stack(Cpu *cpu, Register target, data_bus_t n_of_values);

void call(Cpu *cpu, address_bus_t address);
void return_ok(Cpu *cpu);
void return_ok_with_code(Cpu *cpu, uint8_t exit_code);
void return_err(Cpu *cpu);
void return_err_with_code(Cpu *cpu, uint8_t exit_code);

typedef enum Flag // 8 bit flag register (4 bit flags, 4 bit exit code)
{
    Ovf,
    Zero,
    Irq,
    Err,
    ExitCode,
} Flag;

void set_flag(Cpu *cpu, Flag flag);
void clear_flag(Cpu *cpu, Flag flag);

void noop(Cpu *cpu);
void halt(Cpu *cpu);

void reset(Cpu *cpu);