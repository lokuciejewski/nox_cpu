#pragma once

#include "stdint.h"
#include "stdbool.h"

typedef struct Clock
{
    bool phase;
    uint16_t delay;
} Clock;

void tick(Clock *clock);
void delay_for_n_clock_ticks(Clock *clock, uint8_t ticks);