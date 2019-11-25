#pragma once

#include "decoding.h"
#include "registers.h"

class Core {
public:
    Core();
    uint32_t program_counter;
    uint8_t* memory;
    Registers registers;

    void execute();
};
