# rv_emulator

This is a educational emulator I am building.
The idea is that the emulator will help you understand how CPUs work from a conceptual level.

## About RISC-V
This emulator interprets the machine code for the RISC-V ISA (Instruction Set Architecture).
RISC-V is a open source freely licensed ISA that formed out a research project at UC Berkeley, learn more at [riscv.org](https://riscv.org/).

This emulator targets the `RV32i` subset of the RISC-V ISA.
This is the base level 32-bit architecture without hardware support for things like floating point or single-cycle multiply.
In the future I might decide to add support for more instructions.

## Building
1. Clone: `git clone https://github.com/rohbotics/rv_emulator.git`
2. Go into a build dir: `cd rv_emulator; mkdir build; cd build`
3. Run CMake: `cmake ..`
4. Build: `make`

5. Run tests (optional): `make check`

All dependencies are vendored (git subtree into the `external` directory).
