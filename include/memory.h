#pragma once

#include <vector>
#include <cstdint>

class Memory
{
  public:
    explicit Memory(size_t bytes);

    uint32_t load32(uint32_t addr);
    uint16_t load16(uint32_t addr);
    uint8_t load8(uint32_t addr);

    void store32(uint32_t addr, uint32_t data);
    void store16(uint32_t addr, uint16_t data);
    void store8(uint32_t addr, uint8_t data);

  private:
    std::vector<uint8_t> raw_mem;
};
