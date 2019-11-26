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

#pragma once

#include <cstdint>
#include <array>

class Registers
{
  public:
    Registers();
    uint32_t get(std::uint8_t reg) const;
    // operator[] is const only to stop code like this r[3] = 6
    // If allowed would bypass the 0 register check, wrecking havoc
    const uint32_t& operator[](std::uint8_t reg) const;

    // Return value of set will the the value written
    uint32_t set(std::uint8_t reg, uint32_t value);

  private:
    enum {
        n_registers = 32
    };  // Ugly hack to avoid linker errors
        // Gives us a private constant in the header file
    std::array<uint32_t, n_registers> reg_arr;
};
