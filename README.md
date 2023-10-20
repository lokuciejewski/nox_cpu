# Nox CPU

This repository contains an emulator of the fictional `Nox CPU`, an 8/16bit cpu based on stack registers and counters.

## Building

To build the project, install `cmake` and enter the following commands:

```bash
mkdir build
cd build
cmake ..
make nox-cpu
```

## Running

To run the emulator, use `./nox-cpu` with the following flags:
    - `-m <path>` indicates the path to the `.bin` 64kb file containing the ROM/RAM
    - `-d <int>` indicates the clock delay (in useconds) between changing the states (low/high). Each full clock cycle is double this amount

## Architecture

Detailed architecture description can be found [here](docs/architecture.md).

## Opcodes

All opcodes (using syntax from [Nox ASM](https://github.com/lokuciejewski/nox_asm)) can be found [here](docs/opcodes.md).
