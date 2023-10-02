#include "clock.h"

void tick(Clock *clock)
{
    clock->phase = !clock->phase;
}

void delay_for_n_clock_ticks(Clock *clock, uint8_t ticks)
{
    while (ticks != 0)
    {
        while (!clock->phase)
            ;
        ticks -= 1;
        while (clock->phase)
            ;
    }
}

