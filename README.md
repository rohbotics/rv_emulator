# rv_emulator

This is a educational emulator I am building.
The idea is that the emulator will help you understand how CPUs work from a conceptual level.

## Building
1. Clone `git clone https://github.com/rohbotics/rv_emulator.git`
2. Go into a build dir `cd rv_emulator; mkdir build; cd build`
3. Run CMake: `cmake ..`
4. Build `make`

5. Run tests (optional): `make check`

All dependancies are vendored (git subtree into the `external` dir).
