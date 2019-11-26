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

#include <catch2/catch.hpp>

#include "decoding.h"

using fmt::format;
using std::string;

TEST_CASE("Print Operation", "[formating]") {
    REQUIRE(format("{}", Instruction(Operations::LUI)).find("LUI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::AUIPC)).find("AUIPC") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::JAL)).find("JAL") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::JALR)).find("JALR") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BEQ)).find("BEQ") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BNE)).find("BNE") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BLT)).find("BLT") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BGE)).find("BGE") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BLTU)).find("BLTU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::BGEU)).find("BGEU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::LB)).find("LB") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::LH)).find("LH") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::LW)).find("LW") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::LBU)).find("LBU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::LHU)).find("LHU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SB)).find("SB") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SH)).find("SH") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SW)).find("SW") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::ADDI)).find("ADDI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLTI)).find("SLTI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLTIU)).find("SLTIU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::XORI)).find("XORI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::ORI)).find("ORI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::ANDI)).find("ANDI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLLI)).find("SLLI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SRLI)).find("SRLI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SRAI)).find("SRAI") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::ADD)).find("ADD") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SUB)).find("SUB") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLL)).find("SLL") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLT)).find("SLT") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SLTU)).find("SLTU") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::XOR)).find("XOR") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SRL)).find("SRL") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::SRA)).find("SRA") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::OR)).find("OR") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::AND)).find("AND") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::FENCE)).find("FENCE") != string::npos);
    REQUIRE(format("{}", Instruction(Operations::FENCEI)).find("FENCEI") != string::npos);
}
