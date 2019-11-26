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

#include "memory.h"

Memory::Memory(std::size_t n_bytes) : raw_mem(n_bytes, 0) {
}

uint32_t Memory::load32(uint32_t addr) {
    uint32_t value = raw_mem[addr];
    value |= (raw_mem[addr + 1] << 8);
    value |= (raw_mem[addr + 2] << 16);
    value |= (raw_mem[addr + 3] << 24);
    return value;
}

uint16_t Memory::load16(uint32_t addr) {
    uint16_t value = raw_mem[addr];
    value |= (raw_mem[addr + 1] << 8);
    return value;
}

uint8_t Memory::load8(uint32_t addr) {
    return raw_mem[addr];
}

void Memory::store32(uint32_t addr, uint32_t data) {
    raw_mem[addr] = data & 0xFF;
    raw_mem[addr + 1] = (data >> 8) & 0xFF;
    raw_mem[addr + 2] = (data >> 16) & 0xFF;
    raw_mem[addr + 3] = (data >> 24) & 0xFF;
}

void Memory::store16(uint32_t addr, uint16_t data) {
    raw_mem[addr] = data & 0xFF;
    raw_mem[addr + 1] = (data >> 8) & 0xFF;
}

void Memory::store8(uint32_t addr, uint8_t data) {
    raw_mem[addr] = data & 0xFF;
}
