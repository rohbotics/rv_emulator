#include <catch2/catch.hpp>

#include "decoding.h"
#include "test_instructions.h"

TEST_CASE("Operation Decoding", "[decoding]") {
    REQUIRE(decode_instruction(ilui).operation == Operations::LUI);
    REQUIRE(decode_instruction(iauipc).operation == Operations::AUIPC);
    REQUIRE(decode_instruction(ijal).operation == Operations::JAL);
    REQUIRE(decode_instruction(ijalr).operation == Operations::JALR);
    REQUIRE(decode_instruction(ibeq).operation == Operations::BEQ);
    REQUIRE(decode_instruction(ibne).operation == Operations::BNE);
    REQUIRE(decode_instruction(iblt).operation == Operations::BLT);
    REQUIRE(decode_instruction(ibge).operation == Operations::BGE);
    REQUIRE(decode_instruction(ibltu).operation == Operations::BLTU);
    REQUIRE(decode_instruction(ibgeu).operation == Operations::BGEU);
    REQUIRE(decode_instruction(ilb).operation == Operations::LB);
    REQUIRE(decode_instruction(ilh).operation == Operations::LH);
    REQUIRE(decode_instruction(ilw).operation == Operations::LW);
    REQUIRE(decode_instruction(ilbu).operation == Operations::LBU);
    REQUIRE(decode_instruction(ilhu).operation == Operations::LHU);
    REQUIRE(decode_instruction(isb).operation == Operations::SB);
    REQUIRE(decode_instruction(ish).operation == Operations::SH);
    REQUIRE(decode_instruction(isw).operation == Operations::SW);
    REQUIRE(decode_instruction(iaddi).operation == Operations::ADDI);
    REQUIRE(decode_instruction(islti).operation == Operations::SLTI);
    REQUIRE(decode_instruction(isltiu).operation == Operations::SLTIU);
    REQUIRE(decode_instruction(ixori).operation == Operations::XORI);
    REQUIRE(decode_instruction(iori).operation == Operations::ORI);
    REQUIRE(decode_instruction(iandi).operation == Operations::ANDI);
    REQUIRE(decode_instruction(islli).operation == Operations::SLLI);
    REQUIRE(decode_instruction(isrli).operation == Operations::SRLI);
    REQUIRE(decode_instruction(israi).operation == Operations::SRAI);
    REQUIRE(decode_instruction(iadd).operation == Operations::ADD);
    REQUIRE(decode_instruction(isub).operation == Operations::SUB);
    REQUIRE(decode_instruction(isll).operation == Operations::SLL);
    REQUIRE(decode_instruction(islt).operation == Operations::SLT);
    REQUIRE(decode_instruction(isltu).operation == Operations::SLTU);
    REQUIRE(decode_instruction(ixor).operation == Operations::XOR);
    REQUIRE(decode_instruction(isrl).operation == Operations::SRL);
    REQUIRE(decode_instruction(isra).operation == Operations::SRA);
    REQUIRE(decode_instruction(ior).operation == Operations::OR);
    REQUIRE(decode_instruction(iand).operation == Operations::AND);
    REQUIRE(decode_instruction(ifence).operation == Operations::FENCE);
    REQUIRE(decode_instruction(ifencei).operation == Operations::FENCEI);
}

TEST_CASE("R-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b00000000001000001000000110110011);
    REQUIRE(a.funct7 == 0);
    REQUIRE(a.funct3 == 0);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00010);
    REQUIRE(a.rd == 0b00011);

    a = decode_instruction(0b00001110001000001011000110110011);
    REQUIRE(a.funct7 == 7);
    REQUIRE(a.funct3 == 3);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00010);
    REQUIRE(a.rd == 0b00011);
}

TEST_CASE("I-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b00000000000100001000000010010011);
    REQUIRE(a.funct3 == 0);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == 1);

    a = decode_instruction(0b11111111111000001011000010010011);
    REQUIRE(a.funct3 == 3);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == -2);
}

TEST_CASE("S-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b00000000000100001000000010100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == 1);

    a = decode_instruction(0b11111110111100001000111100100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b01111);
    REQUIRE(a.simm == -2);
}

TEST_CASE("B-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b00000000000100001001000101100011);
    REQUIRE(a.rs1 == 0b00001);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == 2);

    a = decode_instruction(0b11111110000111111001111111100011);
    REQUIRE(a.rs1 == 0b11111);
    REQUIRE(a.rs2 == 0b00001);
    REQUIRE(a.simm == -2);
}

TEST_CASE("U-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b00000000000000000001000010110111);
    REQUIRE(a.rd == 0b00001);
    REQUIRE(a.simm == 4096);

    a = decode_instruction(0b11111111111111111111111110010111);
    REQUIRE(a.rd == 0b11111);
    REQUIRE(a.simm == -4096);
}

TEST_CASE("J-Type Upacking", "[decoding]") {
    auto a = decode_instruction(0b11111111100111111111000001101111);
    REQUIRE(a.rd == 0b00000);
    REQUIRE(a.simm == -8);

    a = decode_instruction(0b00000000100000000000111111101111);
    REQUIRE(a.rd == 0b11111);
    REQUIRE(a.simm == 8);
}
