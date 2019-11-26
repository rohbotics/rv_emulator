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
#include <vector>

class Memory
{
  public:
    explicit Memory(std::size_t n_bytes);

    uint32_t load32(uint32_t addr);
    uint16_t load16(uint32_t addr);
    uint8_t load8(uint32_t addr);

    void store32(uint32_t addr, uint32_t data);
    void store16(uint32_t addr, uint16_t data);
    void store8(uint32_t addr, uint8_t data);

  private:
    std::vector<uint8_t> raw_mem;
};
