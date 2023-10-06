# NOX CPU Architecture

While the cpu is written in a way to allow for easy change of data and address buses, the current documentation assumes 8-bit data bus and 16-bit address bus. This is the way the CPU was initially designed and this emulator is currently written to support only this mode.

## External signals and buses

- one data bus of `data_bus_t` width (8 bits)
- one address bus of `address_bus_t` width (16 bits)
- one write signal `sig_write` (1 bit)
- one interrupt signal `sig_irq` (1 bit)
- one reset signal `sig_res` (1 bit) // TODO: to be added

## Internal registers

- Two hardware `data_bus_t` width stack registers `A` and `B` supporting `push`, `pop` and `peek` (used only internally via opcodes such as `CMP`) operations
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

NOX CPU is inherently a [big endian](https://en.wikipedia.org/wiki/Endianness) CPU and all multi-byte values should be decoded as the leftmost (first) byte being the most significant byte (MSB). While for 8-bit `data_bus_t` and 16-bit `address_bus_t` this matters only for 16-bit values, if the architecture were to be scaled up, all values would still need to be written with MSB first.
