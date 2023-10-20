# NOX CPU Architecture

While the cpu is written in a way to allow for easy change of data and address buses, the current documentation assumes 8-bit data bus and 16-bit address bus. This is the way the CPU was initially designed and this emulator is currently written to support only this mode.

## External signals and buses

- data bus of `data_bus_t` width (8 bits)
- address bus of `address_bus_t` width (16 bits)
- write signal `sig_write` (1 bit)
- interrupt signal `sig_irq` (1 bit)
- reset signal `sig_res` (1 bit) // Not added yet
- sync signal `sig_sync` (1 bit) // Not added yet

## Internal registers

- Two hardware `data_bus_t` width stack registers `A` and `B` supporting `push`, `pop` and `peek` operations
- Two hardware counter/index `data_bus_t` width registers `HI` and `LI` supporting `zero`, `load` and `store` operations
- `A` and `B` can be treated as one double `data_bus_t` width stack register, `AB` supporting some additional operations
- `HI` and `LI` can be treated as one double `data_bus_t` width counter/index register, `HLI` supporting some additional operations
- one 8-bit flag/exit code register containing the following:
  - bit 7: `overflow flag`
  - bit 6: `zero flag`
  - bit 5: `interrupt enable flag`
  - bit 4: `error flag`
  - bits 3-0: `exit code` register

## Endianness

Nox CPU is inherently a [big endian](https://en.wikipedia.org/wiki/Endianness) CPU and all multi-byte values should be decoded as the leftmost (first) byte being the most significant byte (MSB). While for 8-bit `data_bus_t` and 16-bit `address_bus_t` this matters only for 16-bit values, if the architecture were to be scaled up, all values would still need to be written with MSB first.

## Clock cycles

Nox CPU uses an active high clock (meaning the "tick" happens when the clock signal goes high from low). Each instruction uses at least 1 cycle (used for loading the byte). Each memory operation (read/write) needs at least 1 cycle per byte read/written. Additionally, all instructions that use multi-byte opcodes (eg. all that use absolute addressing) require additional 1 cycle per byte of the instruction. Pushing/popping from stack also uses 1 cycle per byte.
