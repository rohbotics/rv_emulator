/*
   Copyright 2019 Rohan Agrawal

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <fmt/format.h>
#include <unistd.h>

#include "decoding.h"
#include "registers.h"
#include "core.h"

/*uint32_t program[10] = {
    0b000100000000'00000'001'01100'0000011,   // Load byte into x12
    0b000100000000'00000'010'01100'0000011,   // Load byte into x12
    0b00000000111100000000010110010011,       // set x11 to 15
    0b00000000000100000000010100010011,       // set x10 to 0
    0b00000000000101010000010100010011,       // ADDI x10, x10, 1
    0b1111111'01011'01010'100'11001'1100011,  // BLT x10, x11, -8
};
unsigned int END_PROGRAM = 6;
*/

/*uint32_t program[10] = {
    0x00000513,
    0x00000597,
    0x01858593,
    0x0005a603,
    0x00a60613,
    0x00150513,
    0xfec546e3,
    0x0028,
    0,
    0
};
unsigned int END_PROGRAM = 10;
*/

/*uint32_t program[15] = {
0x00500513,
0x00600593,
0x008000ef,
0x00000000,
0x00000613,
0x00157693,
0x00d00463,
0x00b60633,
0x00155513,
0x00159593,
0xfea016e3,
0x00008067,
    0
};
unsigned int END_PROGRAM = 15;
*/

uint32_t program[15] = {
//0xffb00513,
0x00500513,
0x00055463,
0x40a00533,
0
};
unsigned int END_PROGRAM = 15;

int main(int argc, char *argv[]) {
    Core c;
    c.program_counter = 0x100;

    // VERY VERY BAD PARSING
    // Load in program from provided hex file
    if (argc != 2) {
        fprintf(stderr, "Please pass in a hex file\n");
        return 1;
    }
    auto file = fopen(argv[1], "r");
    uint32_t data = 0;
    uint32_t idx = 0;
    while (fscanf(file, "%x", &data) == 1) {
        c.memory.store32(idx, data);
        idx += 4;
    }
/*
    // Copy the program defined above into the memory
    // start the program at the initial PC value
    auto pc_start = c.program_counter;
    for (unsigned int i = 0; i < END_PROGRAM; i++) {
        c.memory.store32(pc_start + (4 * i), program[i]);
    }
*/
    uint32_t cycle = 1;
    try {
    while (c.program_counter < 0x10000000) {
        c.execute();
        printf("PC:%d, x10: %x, x11: %x, x15: %x \n", c.program_counter, c.registers[10], c.registers[11],
               c.registers[15]);
    //    usleep(500000);  // sleep for 500,000 microseconds (0.5 seconds)
        cycle++;
    }
    }catch (...) {
        printf("%x %x\n", c.program_counter, c.memory.load32(c.program_counter));
    }

    printf("\n\n\nRan for %d cycles\n", cycle);
}
