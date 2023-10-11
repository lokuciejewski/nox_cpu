# NOX CPU opcodes

Below is the table containing all valid cpu opcodes. The `cycles` value is bound to change if (when?) the physical implementation happens.

*Note:* any 16 bit address (`&[16bit]`) can be substituted by a `[label]` token declared using `[label]:` syntax. To access the value of the address at which the `[label]` is stored, use the `*` operator, e.g. `*[label]`. For more example usages see the `examples/` directory.

| Operation                                                 |  Assembler expression   |            Machine code            | Cycles |      Flag/Exit code changes      |
| --------------------------------------------------------- | :---------------------: | :--------------------------------: | :----: | :------------------------------: |
| No operation                                              |         `NOOP`          |               `0x00`               |   1    |                -                 |
| Push an immediate value into `A`                          |     `PUSH [8bit] A`     |           `0x01 [8bit]`            |   2    |      `ZERO` = `[8bit]` == 0      |
| Push an immediate value into `B`                          |     `PUSH [8bit] B`     |           `0x02 [8bit]`            |   2    |      `ZERO` = `[8bit]` == 0      |
| Push a value at absolute address into `A`                 |    `PUSH &[16bit] A`    | `0x03 [address_msb] [address_lsb]` |   3    | `ZERO` = `value at address` == 0 |
| Push a value at absolute address into `B`                 |    `PUSH &[16bit] B`    | `0x04 [address_msb] [address_lsb]` |   3    | `ZERO` = `value at address` == 0 |
| Push a value from `B` into `A`                            |       `PUSH B A`        |               `0x05`               |   2    |                -                 |
| Push a value from `A` into `B`                            |       `PUSH A B`        |               `0x06`               |   2    |                -                 |
| Push value from address in `HLI` into `A`                 |      `PUSH &HLI A`      |               `0x07`               |   3    |   `ZERO` = `value at HLI` == 0   |
| Push value from address in `HLI` into `B`                 |      `PUSH &HLI B`      |               `0x08`               |   3    |   `ZERO` = `value at HLI` == 0   |
| Push EXIT_CODE to `A`                                     |   `PUSH EXIT_CODE A`    |               `0x09`               |   2    |                -                 |
| Push EXIT_CODE to `B`                                     |   `PUSH EXIT_CODE B`    |               `0x0A`               |   2    |                -                 |
| Pop value from `A` into address in `HLI`                  |      `POP A &HLI`       |               `0x0B`               |   3    |                -                 |
| Pop value from `B` into address in `HLI`                  |      `POP B &HLI`       |               `0x0C`               |   3    |                -                 |
| Pop value from `A` into address in `HLI`                  |      `POP A &HLI`       |               `0x0D`               |   3    |                -                 |
| Pop value from `B` into address in `HLI`                  |      `POP B &HLI`       |               `0x0E`               |   3    |                -                 |
| Peek value from `A` into address in `HLI`                 |      `PEEK A &HLI`      |               `0x0F`               |   3    |                -                 |
| Peek value from `B` into address in `HLI`                 |      `PEEK B &HLI`      |               `0x10`               |   3    |                -                 |
| Add `A` and B, push to `B`                                |        `ADD A B`        |               `0x11`               |   2    |          `OVF`, `ZERO`           |
| Add `B` and A, push to `A`                                |        `ADD B A`        |               `0x12`               |   2    |          `OVF`, `ZERO`           |
| Add immediate value to `A`                                |     `ADD [8bit] A`      |           `0x13 [8bit]`            |   3    |          `OVF`, `ZERO`           |
| Add immediate value to `B`                                |     `ADD [8bit] B`      |           `0x14 [8bit]`            |   3    |          `OVF`, `ZERO`           |
| Add value at absolute address to `A`                      |    `ADD &[16bit] A`     | `0x15 [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Add value at absolute address to `B`                      |    `ADD &[16bit] B`     | `0x16 [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Subtract `B` from A, push to `B`                          |        `SUB A B`        |               `0x17`               |   2    |          `OVF`, `ZERO`           |
| Subtract `A` from B, push to `A`                          |        `SUB B A`        |               `0x18`               |   2    |          `OVF`, `ZERO`           |
| Subtract immediate value from `A`                         |     `SUB [8bit] A`      |           `0x19 [8bit]`            |   3    |          `OVF`, `ZERO`           |
| Subtract immediate value from `B`                         |     `SUB [8bit] B`      |           `0x1A [8bit]`            |   3    |          `OVF`, `ZERO`           |
| Subtract value at absolute address from `A`               |    `SUB &[16bit] A`     | `0x1B [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Subtract value at absolute address from `B`               |    `SUB &[16bit] B`     | `0x1C [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Shift `A` left                                            |     `SHIFT_LEFT A`      |               `0x1D`               |   2    |              `ZERO`              |
| Shift `A` right                                           |     `SHIFT_RIGHT A`     |               `0x1E`               |   2    |              `ZERO`              |
| Shift `B` left                                            |     `SHIFT_LEFT B`      |               `0x1F`               |   2    |              `ZERO`              |
| Shift `B` right                                           |     `SHIFT_RIGHT B`     |               `0x20`               |   2    |              `ZERO`              |
| Bitwise AND `A` and B, push to `B`                        |        `AND A B`        |               `0x21`               |   2    |              `ZERO`              |
| Bitwise AND `B` and A, push to `A`                        |        `AND B A`        |               `0x22`               |   2    |              `ZERO`              |
| Bitwise OR `A` and B, push to `B`                         |        `OR A B`         |               `0x23`               |   2    |              `ZERO`              |
| Bitwise OR `B` and A, push to `A`                         |        `OR B A`         |               `0x24`               |   2    |              `ZERO`              |
| Bitwise XOR `A` and B, push to `B`                        |        `XOR A B`        |               `0x25`               |   2    |              `ZERO`              |
| Bitwise XOR `B` and A, push to `A`                        |        `XOR B A`        |               `0x26`               |   2    |              `ZERO`              |
| Bitwise NOT A, push to `A`                                |         `NOT A`         |               `0x27`               |   2    |              `ZERO`              |
| Bitwise NOT B, push to `B`                                |         `NOT B`         |               `0x28`               |   2    |              `ZERO`              |
| Compare `A` and `B`                                       |        `CMP A B`        |               `0x29`               |   2    |          `ZERO`, `OVF`           |
| Compare immediate value with `A`                          |     `CMP [8bit] A`      |           `0x2A [8bit]`            |   3    |          `ZERO`, `OVF`           |
| Compare immediate value with `B`                          |     `CMP [8bit] B`      |           `0x2B [8bit]`            |   3    |          `ZERO`, `OVF`           |
| Compare value at absolute address with `A`                |    `CMP &[16bit] A`     | `0x2C [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Compare value at absolute address with `B`                |    `CMP &[16bit] B`     | `0x2D [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Push immediate value into `HI`                            |    `PUSH [8bit] HI`     |           `0x2E [8bit]`            |   3    |                -                 |
| Push immediate value into `LI`                            |    `PUSH [8bit] LI`     |           `0x2F [8bit]`            |   3    |                -                 |
| Push value at absolute address into `HI`                  |   `PUSH &[16bit] HI`    | `0x30 [address_msb] [address_lsb]` |   4    |                -                 |
| Push value at absolute address into `LI`                  |   `PUSH &[16bit] LI`    | `0x31 [address_msb] [address_lsb]` |   4    |                -                 |
| Store `HI` at absolute address                            |   `STORE HI &[16bit]`   | `0x32 [address_msb] [address_lsb]` |   4    |                -                 |
| Store `LI` at absolute address                            |   `STORE LI &[16bit]`   | `0x33 [address_msb] [address_lsb]` |   4    |                -                 |
| Compare `HI` and immediate value                          |     `CMP HI [8bit]`     |           `0x34 [8bit]`            |   3    |          `ZERO`, `OVF`           |
| Compare `LI` and immediate value                          |     `CMP LI [8bit]`     |           `0x35 [8bit]`            |   3    |          `ZERO`, `OVF`           |
| Compare `HI` and value at absolute address                |    `CMP HI &[16bit]`    | `0x36 [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Compare `LI` and value at absolute address                |    `CMP LI &[16bit]`    | `0x37 [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Increment `HI`                                            |        `INC HI`         |               `0x38`               |   2    |          `OVF`, `ZERO`           |
| Increment `LI`                                            |        `INC LI`         |               `0x39`               |   2    |          `OVF`, `ZERO`           |
| Decrement `HI`                                            |        `DEC HI`         |               `0x3A`               |   2    |          `OVF`, `ZERO`           |
| Decrement `LI`                                            |        `DEC LI`         |               `0x3B`               |   2    |          `OVF`, `ZERO`           |
| Set `HI` to 0                                             |        `ZERO HI`        |               `0x3C`               |   2    |                -                 |
| Set `LI` to 0                                             |        `ZERO LI`        |               `0x3D`               |   2    |                -                 |
| Swap `HI` and `LI`                                        |      `SWAP HI LI`       |               `0x3E`               |   2    |                -                 |
| Push an immediate value into `AB`                         |    `PUSH [16bit] AB`    |   `0x3F [value_msb] [value_lsb]`   |   3    |                -                 |
| Push value at absolute address into `AB`                  |   `PUSH &[16bit] AB`    | `0x40 [address_msb] [address_lsb]` |   4    |                -                 |
| Push value from address in `HLI` into `AB`                |     `PUSH &HLI AB`      |               `0x41`               |   3    |                -                 |
| Push `HI` and `LI` into `AB`                              |      `PUSH HLI AB`      |               `0x42`               |   3    |                -                 |
| Pop value from `AB` into address in `HLI`                 |      `POP AB &HLI`      |               `0x43`               |   4    |                -                 |
| Pop value from `AB` into address in `HLI`                 |      `POP AB HLI`       |               `0x44`               |   4    |                -                 |
| Peek value from `AB` into address in `HLI`                |     `PEEK AB &HLI`      |               `0x45`               |   4    |                -                 |
| Peek value from `AB` into address in `HLI`                |      `PEEK AB HLI`      |               `0x46`               |   4    |                -                 |
| Add immediate value to `AB`                               |    `ADD [16bit] AB`     |   `0x47 [value_msb] [value_lsb]`   |   3    |          `OVF`, `ZERO`           |
| Add value at absolute address to `AB`                     |    `ADD &[16bit] AB`    | `0x48 [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Subtract immediate value from `AB`                        |    `SUB [16bit] AB`     |   `0x49 [value_msb] [value_lsb]`   |   3    |          `OVF`, `ZERO`           |
| Subtract value at absolute address from `AB`              |    `SUB &[16bit] AB`    | `0x4A [address_msb] [address_lsb]` |   4    |          `OVF`, `ZERO`           |
| Shift `AB` left                                           |     `SHIFT_LEFT AB`     |               `0x4B`               |   3    |              `ZERO`              |
| Shift `AB` right                                          |    `SHIFT_RIGHT AB`     |               `0x4C`               |   3    |              `ZERO`              |
| Bitwise AND immediate value with `AB`                     |    `AND [16bit] AB`     |   `0x4D [value_msb] [value_lsb]`   |   3    |              `ZERO`              |
| Bitwise AND value at absolute address with `AB`           |    `AND &[16bit] AB`    | `0x4E [address_msb] [address_lsb]` |   4    |              `ZERO`              |
| Bitwise OR immediate value with `AB`                      |     `OR [16bit] AB`     |   `0x4F [value_msb] [value_lsb]`   |   3    |              `ZERO`              |
| Bitwise OR value at absolute address with `AB`            |    `OR &[16bit] AB`     | `0x50 [address_msb] [address_lsb]` |   4    |              `ZERO`              |
| Bitwise XOR immediate value with `AB`                     |    `XOR [16bit] AB`     |   `0x51 [value_msb] [value_lsb]`   |   3    |              `ZERO`              |
| Bitwise XOR value at absolute address with `AB`           |    `XOR &[16bit] AB`    | `0x52 [address_msb] [address_lsb]` |   4    |              `ZERO`              |
| Bitwise NOT AB, push to `AB`                              |        `NOT AB`         |               `0x53`               |   3    |              `ZERO`              |
| Push immediate value into `HLI`                           |   `PUSH [16bit] HLI`    |   `0x54 [value_msb] [value_lsb]`   |   3    |                -                 |
| Push value at absolute address into `HLI`                 |   `PUSH &[16bit] HLI`   | `0x55 [address_msb] [address_lsb]` |   4    |                -                 |
| Store `HLI` at absolute address                           |  `STORE HLI &[16bit]`   | `0x56 [address_msb] [address_lsb]` |   4    |                -                 |
| Compare immediate value with `AB`                         |    `CMP [16bit] AB`     |   `0x57 [value_msb] [value_lsb]`   |   3    |          `ZERO`, `OVF`           |
| Compare value at absolute address with `AB`               |    `CMP &[16bit] AB`    | `0x58 [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Compare immediate value with `HLI`                        |    `CMP [16bit] HLI`    |   `0x59 [value_msb] [value_lsb]`   |   3    |          `ZERO`, `OVF`           |
| Compare value at absolute address with `HLI`              |   `CMP &[16bit] HLI`    | `0x5A [address_msb] [address_lsb]` |   4    |          `ZERO`, `OVF`           |
| Increment `HLI`                                           |        `INC HLI`        |               `0x5B`               |   3    |          `OVF`, `ZERO`           |
| Decrement `HLI`                                           |        `DEC HLI`        |               `0x5C`               |   3    |          `OVF`, `ZERO`           |
| Set `HLI` to 0                                            |       `ZERO HLI`        |               `0x5D`               |   3    |                -                 |
| Jump to specified address if ZERO flag is set             |     `JZE &[16bit]`      | `0x5E [address_msb] [address_lsb]` |   3    |                -                 |
| Jump to specified address if OVF flag is set              |     `JOF &[16bit]`      | `0x5F [address_msb] [address_lsb]` |   3    |                -                 |
| Jump to specified address if ERR flag is set              |     `JER &[16bit]`      | `0x60 [address_msb] [address_lsb]` |   3    |                -                 |
| Jump to specified address if OK flag is set               |     `JOK &[16bit]`      | `0x61 [address_msb] [address_lsb]` |   3    |                -                 |
| Jump to specified address                                 |     `JUMP &[16bit]`     | `0x62 [address_msb] [address_lsb]` |   3    |                -                 |
| Push stack address to AB, set ZERO flag accordingly       | `PUSH AB STACK_ADDRESS` |               `0x63`               |   3    |  `ZERO` = `stack_address` == 0   |
| Push stack size to AB, set ZERO flag accordingly          |  `PUSH AB STACK_SIZE`   |               `0x64`               |   3    |    `ZERO` = `stack_size` == 0    |
| Pop value from `AB` to stack address, resetting the stack | `POP STACK_ADDRESS AB`  |               `0x65`               |   4    |                -                 |
| Pop value from `AB` to stack size                         |   `POP STACK_SIZE AB`   |               `0x66`               |   4    |                -                 |
| Pop value from `AB` to IRQ address                        |      `POP AB IRA`       |               `0x67`               |   3    |                -                 |
| Push value(s) from `A` to stack IMMEDIATE value times     |  `PUSH A STACK [8bit]`  |           `0x68 [8bit]`            |   3    |                -                 |
| Pop value(s) from stack to `A` IMMEDIATE value times      |  `POP STACK A [8bit]`   |           `0x69 [8bit]`            |   3    |                -                 |
| Push value(s) from `B` to stack IMMEDIATE value times     |  `PUSH B STACK [8bit]`  |           `0x6A [8bit]`            |   3    |                -                 |
| Pop value(s) from stack to `B` IMMEDIATE value times      |  `POP STACK B [8bit]`   |           `0x6B [8bit]`            |   3    |                -                 |
| Push `HI` to stack                                        |     `PUSH HI STACK`     |               `0x6C`               |   3    |                -                 |
| Pop `HI` from stack                                       |     `POP STACK HI`      |               `0x6D`               |   3    |                -                 |
| Push `LI` to stack                                        |     `PUSH LI STACK`     |               `0x6E`               |   3    |                -                 |
| Pop `LI` from stack                                       |     `POP STACK LI`      |               `0x6F`               |   3    |                -                 |
| Call subroutine                                           |     `CALL &[16bit]`     | `0x70 [address_msb] [address_lsb]` |   4    |                -                 |
| Return from subroutine, set OK flag and reset stack       |        `RET OK`         |               `0x71`               |   4    |      `OK`, `EXIT_CODE` = 0       |
| Return from subroutine, set OK flag and set EXIT_CODE     |     `RET OK [8bit]`     |           `0x72 [8bit]`            |   5    |   `OK`, `EXIT_CODE` = `[8bit]`   |
| Return from subroutine, set ERR flag and reset stack      |        `RET ERR`        |               `0x73`               |   4    |      `ERR`, `EXIT_CODE` = 0      |
| Return from subroutine, set ERR flag and set EXIT_CODE    |    `RET ERR [8bit]`     |           `0x74 [8bit]`            |   5    |  `ERR`, `EXIT_CODE` = `[8bit]`   |
| Set ERR flag                                              |        `SET ERR`        |               `0x75`               |   2    |                -                 |
| Set IRQ flag                                              |        `SET IRQ`        |               `0x76`               |   2    |                -                 |
| Clear EXIT_CODE                                           |        `CLR EX`         |               `0x77`               |   2    |                -                 |
| Clear ERR flag                                            |        `CLR ERR`        |               `0x78`               |   2    |                -                 |
| Clear IRQ flag                                            |        `CLR IRQ`        |               `0x79`               |   2    |                -                 |
| Clear OVF flag                                            |        `CLR OVF`        |               `0x7A`               |   2    |                -                 |
| Clear ZERO flag                                           |        `CLR ZER`        |               `0x7B`               |   2    |                -                 |
| Halt execution                                            |         `HALT`          |               `0x7C`               |   1    |                -                 |
