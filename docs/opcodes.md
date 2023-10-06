# NOX CPU opcodes

Below is the table containing all valid cpu opcodes. The `cycles` value is bound to change if (when?) the physical implementation happens

|Name|Assembler opcode|Machine code|Cycles|Flag/Exit code changes|Example usage|
|----|:--------------:|:----------:|:----:|:--------------------:|-------------|
|No operation|`NOOP`|`0x00`|1|-|`NOOP`|
|Push an immediate value into `A`|`PUSH A #8bit`|`0x01 #8bit`|2|`ZERO` = `#8bit` == 0|`PUSH A 0xab`|
|Push an immediate value into `B`|`PUSH B #8bit`|`0x02 #8bit`|2|`ZERO` = `#8bit` == 0|`PUSH B 0xdc`|
|Push a value at absolute address into `A`|`PUSH A 16bit`|`0x03 <address_msb> <address_lsb>`|3|`ZERO` = `value at address` == 0|`PUSH A 0x233A`|
|Push a value at absolute address into `B`|`PUSH B 16bit`|`0x04 <address_msb> <address_lsb>`|3|`ZERO` = `value at address` == 0|`PUSH B 0x2002`|
|Push a value from `B` into `A`|`PUSH A B`|`0x05`|2|-|`PUSH A B`|
|Push a value from `A` into `B`|`PUSH B A`|`0x06`|2|-|`PUSH B A`|
