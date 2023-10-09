#include "stdio.h"
#include "unistd.h"
#include "pthread.h"

#include "components.h"
#include "clock.h"
#include "cpu.h"
#include "opcodes.h"

void *tick_function(void *vargrp)
{
    Clock *clock = (Clock *)vargrp;
    uint64_t n_of_ticks = 0;
    while (1)
    {
        usleep(10000);
        tick(clock);
        if (clock->phase)
        {
            n_of_ticks += 1;
            // printf("\r%d", n_of_ticks);
            fflush(stdout);
        }
    }
}

struct arg_buses
{
    uint16_t *address_bus;
    uint8_t *data_bus;
    bool *sig_write;
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

            if (write_val && bus_value == 0xe000)
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

    FILE *fileptr = fopen("./memory.bin", "rb");
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
                // printf("\nWRITE: 0x%02x to 0x%04x\n", *buses->data_bus, bus_value);
                memory[bus_value] = *buses->data_bus;
            }
            else
            {
                *buses->data_bus = memory[bus_value];
            }
        }
    }
}

int main()
{
    Clock clock = {false};
    uint8_t data_bus = 0;
    uint16_t address_bus = 0;
    bool sig_write = false;
    bool sig_irq = false;

    pthread_t clock_thread;
    pthread_create(&clock_thread, NULL, tick_function, (void *)&clock);

    struct arg_buses buses = {
        &address_bus,
        &data_bus,
        &sig_write};
    pthread_t rom_thread;
    pthread_create(&rom_thread, NULL, rom_function, (void *)&buses);

    pthread_t output_thread;
    pthread_create(&output_thread, NULL, output_function, (void *)&buses);

    Cpu *cpu = init_cpu(&clock, &address_bus, &data_bus, &sig_write, &sig_irq);
    reset(cpu);
    run(cpu, false, false);
    return 0;
}