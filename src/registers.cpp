#include <stdexcept>
#include <algorithm>
#include <fmt/format.h>

#include "registers.h"

// Initialize the register array as all 0s
Registers::Registers() : reg_arr{0} {}

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

