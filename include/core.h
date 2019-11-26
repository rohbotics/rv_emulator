#pragma once

#include "registers.h"
#include "memory.h"

class Core
{
  public:
    Core();
    uint32_t program_counter;
    Memory memory;
    Registers registers;

    void execute();
};
