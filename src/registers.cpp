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

#include <stdexcept>
#include <algorithm>
#include <fmt/format.h>

#include "registers.h"

// Initialize the register array as all 0s
Registers::Registers() : reg_arr{ 0 } {
}

uint32_t Registers::get(uint8_t reg) const {
    if (reg < n_registers)
        return reg_arr[reg];
    else
        throw std::out_of_range(fmt::format("register {} out of bounds [0, {})", reg, n_registers));
}

const uint32_t& Registers::operator[](uint8_t reg) const {
    if (reg < n_registers)
        return reg_arr[reg];
    else
        throw std::out_of_range(fmt::format("register {} out of bounds [0, {})", reg, n_registers));
}

uint32_t Registers::set(uint8_t reg, uint32_t value) {
    if (reg == 0)
        return 0;
    else if (reg < n_registers)
        return reg_arr[reg] = value;
    else
        throw std::out_of_range(fmt::format("register {} out of bounds [0, {})", reg, n_registers));
}
