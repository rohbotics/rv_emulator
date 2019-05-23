#pragma once

#include <cstdint>

class Registers {
  public:
    Registers();
    uint32_t get(std::uint8_t reg) const;
    // operator[] is const only to stop code like this r[3] = 6
    // If allowed would bypass the 0 register check, wrecking havoc
    const uint32_t& operator[](std::uint8_t reg) const;

    // Return value of set will the the value written
    uint32_t set(std::uint8_t reg, uint32_t value);
  private:
    enum {n_registers = 32}; // Ugly hack to avoid linker errors
                             // Gives us a private constant
    uint32_t reg_arr[n_registers];
};

