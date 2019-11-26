#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <fmt/format.h>
#include <unistd.h>

#include "decoding.h"
#include "registers.h"
#include "core.h"

uint32_t program[10] = {
    0b000100000000'00000'001'01100'0000011,   // Load byte into x12
    0b000100000000'00000'010'01100'0000011,   // Load byte into x12
    0b00000000111100000000010110010011,       // set x11 to 15
    0b00000000000100000000010100010011,       // set x10 to 0
    0b00000000000101010000010100010011,       // ADDI x10, x10, 1
    0b1111111'01011'01010'100'11001'1100011,  // BLT x10, x11, -8
};
unsigned int END_PROGRAM = 6;

int main() {
    Core c;

    c.program_counter = 0x100;

    // Copy the program defined above into the memory
    // start the program at the initial PC value
    auto pc_start = c.program_counter;
    for (unsigned int i = 0; i < END_PROGRAM; i++) {
        c.memory.store32(pc_start + (4 * i), program[i]);
    }

    uint32_t cycle = 1;
    while (c.program_counter < 0x100 + END_PROGRAM * 4) {
        c.execute();
        printf("PC:%d, x10: %x, x11: %x, x12: %x \n", c.program_counter, c.registers[10], c.registers[11],
               c.registers[12]);
        usleep(500000);  // sleep for 500,000 microseconds (0.5 seconds)
        cycle++;
    }

    printf("\n\n\nRan for %d cycles\n", cycle);
}
