#include "stdio.h"
#include "unistd.h"
#include "pthread.h"
#include "string.h"

#include "components.h"
#include "clock.h"
#include "cpu.h"
#include "opcodes.h"

#define STDOUT_DISABLED 0xffff

void *tick_function(void *vargrp)
{
    Clock *clock = (Clock *)vargrp;
    uint64_t n_of_ticks = 0;
    while (1)
    {
        usleep(clock->delay);
        tick(clock);
        if (clock->phase)
        {
            n_of_ticks += 1;
            fflush(stdout);
        }
    }
}

struct arg_buses
{
    uint16_t *address_bus;
    uint8_t *data_bus;
    bool *sig_write;
    char *memory_file_name;
    uint16_t stdout_address;
};

void *output_function(void *vargrp)
{
    struct arg_buses *buses = (struct arg_buses *)vargrp;
    uint16_t bus_value = 0;
    bool write_val = false;
    while (1)
    {
        if (*buses->address_bus != bus_value || write_val != *buses->sig_write)
        {
            bus_value = *buses->address_bus;
            write_val = *buses->sig_write;

            if (write_val && bus_value == buses->stdout_address)
            {
                printf("%c", *buses->data_bus);
            }
        }
    }
}

void *rom_function(void *vargrp)
{
    struct arg_buses *buses = (struct arg_buses *)vargrp;
    uint16_t bus_value = 0;
    bool write_val = false;

    FILE *fileptr = fopen(buses->memory_file_name, "rb");
    uint8_t *memory = (uint8_t *)malloc(0xffff * sizeof(uint8_t));
    fread(memory, 0xffff, 1, fileptr);
    fclose(fileptr);

    while (1)
    {
        if (*buses->address_bus != bus_value || write_val != *buses->sig_write)
        {
            bus_value = *buses->address_bus;
            write_val = *buses->sig_write;

            if (write_val)
            {
                memory[bus_value] = *buses->data_bus;
            }
            else
            {
                *buses->data_bus = memory[bus_value];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    uint8_t data_bus = 0;
    uint16_t address_bus = 0;
    bool sig_write = false;
    bool sig_irq = false;

    char *memory_file_path, *clock_delay, *stdout_address, *current_arg;

    if (argc < 3)
    {
        printf("Provide memory file path `-m <file path>` and a clock delay (useconds) `-d <delay>`");
        exit(1);
    }

    for (int arg = 1; arg < argc; arg++)
    {
        current_arg = argv[arg];
        if (strcmp(current_arg, "-m") == 0 && arg < argc)
        {
            arg++;
            memory_file_path = argv[arg];
        }
        else if (strcmp(current_arg, "-d") == 0 && arg < argc)
        {
            arg++;
            clock_delay = argv[arg];
        }
        else if (strcmp(current_arg, "-o") == 0 && arg < argc)
        {
            arg++;
            stdout_address = argv[arg];
        }
        else
        {
            printf("Invalid argument: %s\n", current_arg);
        }
    }

    uint16_t output_address = STDOUT_DISABLED;
    if (stdout_address != NULL)
    {
        output_address = strtoul(stdout_address, NULL, 16);
    }

    struct arg_buses buses = {
        &address_bus,
        &data_bus,
        &sig_write,
        memory_file_path,
        output_address,
    };

    pthread_t rom_thread;
    pthread_create(&rom_thread, NULL, rom_function, (void *)&buses);

    Clock clock = {false, strtoul(clock_delay, NULL, 10)};

    pthread_t clock_thread;
    pthread_create(&clock_thread, NULL, tick_function, (void *)&clock);

    if (output_address != STDOUT_DISABLED)
    {
        pthread_t output_thread;
        pthread_create(&output_thread, NULL, output_function, (void *)&buses);
    }

    Cpu *cpu = init_cpu(&clock, &address_bus, &data_bus, &sig_write, &sig_irq);
    reset(cpu);
    run(cpu, false, false, true);
    return 0;
}