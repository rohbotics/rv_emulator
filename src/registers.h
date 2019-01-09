#pragma once

#include <cstdint>

class Registers {
  public:
    Registers();
    uint32_t get(std::uint8_t reg) const;
    const uint32_t& operator[](std::uint8_t reg) const; //operator[] is const only
    uint32_t set(std::uint8_t reg, uint32_t value);
  private:
    enum {n_registers = 32}; // Ugly hack to avoid linker errors
    uint32_t reg_arr[n_registers];
};

