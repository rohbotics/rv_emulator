#include <catch2/catch.hpp>

#include "decoding.h"
#include "test_instructions.h"

TEST_CASE("Operation Decoding", "[decoding]") {
    REQUIRE(Instruction(ilui).operation == Operations::LUI);
    REQUIRE(Instruction(iauipc).operation == Operations::AUIPC);
    REQUIRE(Instruction(ijal).operation == Operations::JAL);
    REQUIRE(Instruction(ijalr).operation == Operations::JALR);
    REQUIRE(Instruction(ibeq).operation == Operations::BEQ);
    REQUIRE(Instruction(ibne).operation == Operations::BNE);
    REQUIRE(Instruction(iblt).operation == Operations::BLT);
    REQUIRE(Instruction(ibge).operation == Operations::BGE);
    REQUIRE(Instruction(ibltu).operation == Operations::BLTU);
    REQUIRE(Instruction(ibgeu).operation == Operations::BGEU);
    REQUIRE(Instruction(ilb).operation == Operations::LB);
    REQUIRE(Instruction(ilh).operation == Operations::LH);
    REQUIRE(Instruction(ilw).operation == Operations::LW);
    REQUIRE(Instruction(ilbu).operation == Operations::LBU);
    REQUIRE(Instruction(ilhu).operation == Operations::LHU);
    REQUIRE(Instruction(isb).operation == Operations::SB);
    REQUIRE(Instruction(ish).operation == Operations::SH);
    REQUIRE(Instruction(isw).operation == Operations::SW);
    REQUIRE(Instruction(iaddi).operation == Operations::ADDI);
    REQUIRE(Instruction(islti).operation == Operations::SLTI);
    REQUIRE(Instruction(isltiu).operation == Operations::SLTIU);
    REQUIRE(Instruction(ixori).operation == Operations::XORI);
    REQUIRE(Instruction(iori).operation == Operations::ORI);
    REQUIRE(Instruction(iandi).operation == Operations::ANDI);
    REQUIRE(Instruction(islli).operation == Operations::SLLI);
    REQUIRE(Instruction(isrli).operation == Operations::SRLI);
    REQUIRE(Instruction(israi).operation == Operations::SRAI);
    REQUIRE(Instruction(iadd).operation == Operations::ADD);
    REQUIRE(Instruction(isub).operation == Operations::SUB);
    REQUIRE(Instruction(isll).operation == Operations::SLL);
    REQUIRE(Instruction(islt).operation == Operations::SLT);
    REQUIRE(Instruction(isltu).operation == Operations::SLTU);
    REQUIRE(Instruction(ixor).operation == Operations::XOR);
    REQUIRE(Instruction(isrl).operation == Operations::SRL);
    REQUIRE(Instruction(isra).operation == Operations::SRA);
    REQUIRE(Instruction(ior).operation == Operations::OR);
    REQUIRE(Instruction(iand).operation == Operations::AND);
    REQUIRE(Instruction(ifence).operation == Operations::FENCE);
    REQUIRE(Instruction(ifencei).operation == Operations::FENCEI);
}

TEST_CASE("R-Type Upacking", "[decoding]") {
    auto a = Instruction(0b00000000001000001000000110110011);
    REQUIRE(a.funct7 == 0);
    REQUIRE(a.funct3 == 0);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00010);
    REQUIRE(a.rd == 0b00011);

    a = Instruction(0b00001110001000001011000110110011);
    REQUIRE(a.funct7 == 7);
    REQUIRE(a.funct3 == 3);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00010);
    REQUIRE(a.rd == 0b00011);
}

TEST_CASE("I-Type Upacking", "[decoding]") {
    auto a = Instruction(0b00000000000100001000000010010011);
    REQUIRE(a.funct3 == 0);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == 1);

    a = Instruction(0b11111111111000001011000010010011);
    REQUIRE(a.funct3 == 3);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == -2);
}

TEST_CASE("S-Type Upacking", "[decoding]") {
    auto a = Instruction(0b00000000000100001000000010100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == 1);

    a = Instruction(0b11111110111100001000111100100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b01111);
    REQUIRE(a.simm == -2);
}

TEST_CASE("B-Type Upacking", "[decoding]") {
    auto a = Instruction(0b00000000000100001001000101100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == 2);

    a = Instruction(0b11111110000111111001111111100011);
    REQUIRE(a.rs1 == 0b11111);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == -2);
}

TEST_CASE("U-Type Upacking", "[decoding]") {
    auto a = Instruction(0b00000000000000000001000010110111);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == 4096);

    a = Instruction(0b11111111111111111111111110010111);
    REQUIRE(a.rd == 0b11111);
    REQUIRE(a.simm == -4096);
}

TEST_CASE("J-Type Upacking", "[decoding]") {
    auto a = Instruction(0b11111111100111111111000001101111);
    REQUIRE(a.rd == 0b00000);
    REQUIRE(a.simm == -8);

    a = Instruction(0b00000000100000000000111111101111);
    REQUIRE(a.rd == 0b11111);
    REQUIRE(a.simm == 8);
}
