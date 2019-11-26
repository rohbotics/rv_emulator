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

#include <type_traits>
#include "decoding.h"

// Annonymous namespace for hiding implementation details
namespace
{
enum class Opcodes {
    LUI = 0b0110111,
    AUIPC = 0b0010111,
    JAL = 0b1101111,
    JALR = 0b1100111,
    BRANCH = 0b1100011,
    LOAD = 0b0000011,
    STORE = 0b0100011,
    ARITH_IMM = 0b0010011,
    ARITH = 0b0110011,
    FENCE = 0b0001111
};

/*** Inline utility functions ***/

// Adapted from http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
template <typename T, unsigned B>
constexpr inline static T signextend(const T x) {
    static_assert(sizeof(T) * 8 > B, "size of type must be larger than sign extended value");
    static_assert(std::is_signed<T>::value, "return must be of signed type or else this function invokes undefined "
                                            "behavior");
    struct {
        T x : B;
    } s = { 0 };
    return s.x = x;
}

// These pull out common parts of instructions
constexpr inline uint8_t get_rd(uint32_t inst) {
    return ((inst >> 7) & 0x1F);
}

constexpr inline uint8_t get_rs1(uint32_t inst) {
    return ((inst >> 15) & 0x1F);
}

constexpr inline uint8_t get_rs2(uint32_t inst) {
    return ((inst >> 20) & 0x1F);
}

constexpr inline uint8_t get_funct3(uint32_t inst) {
    return ((inst >> 12) & 0x07);
}

constexpr inline uint8_t get_funct7(uint32_t inst) {
    return ((inst >> 25) & 0x7F);
}
constexpr inline Opcodes get_opcode(uint32_t inst) {
    uint8_t opcode = inst & 0x7f;
    return static_cast<Opcodes>(opcode);
}

/*** Unpacking functions ***/

// NOTE: These could be part of the class, but I don't want to put them in the header file,
// even if they are as private functions
constexpr Instruction unpack_instruction_R(Instruction& in, uint32_t raw_inst) {
    in.funct7 = get_funct7(raw_inst);
    in.funct3 = get_funct3(raw_inst);
    in.rs1 = get_rs1(raw_inst);
    in.rs2 = get_rs2(raw_inst);
    in.rd = get_rd(raw_inst);
    return in;
}

constexpr Instruction unpack_instruction_I(Instruction& in, uint32_t raw_inst) {
    in.funct3 = get_funct3(raw_inst);
    in.rs1 = get_rs1(raw_inst);
    in.rd = get_rd(raw_inst);
    in.imm = (raw_inst >> 20) & 0xFFF;
    in.simm = signextend<int32_t, 12>(in.imm);
    return in;
}

constexpr Instruction unpack_instruction_S(Instruction& in, uint32_t raw_inst) {
    in.funct3 = get_funct3(raw_inst);
    in.rs1 = get_rs1(raw_inst);
    in.rs2 = get_rs2(raw_inst);

    in.imm = 0;
    in.imm = ((raw_inst >> 7) & 0b000000011111);
    in.imm = in.imm | ((raw_inst >> 20) & 0b111111100000);
    in.simm = signextend<int32_t, 12>(in.imm);
    return in;
}

constexpr Instruction unpack_instruction_B(Instruction& in, uint32_t raw_inst) {
    in.funct3 = get_funct3(raw_inst);
    in.rs1 = get_rs1(raw_inst);
    in.rs2 = get_rs2(raw_inst);

    in.imm = 0;
    in.imm = ((raw_inst >> 7) & 0b0000000011110);
    in.imm = in.imm | ((raw_inst >> 20) & 0b0011111100000);
    in.imm = in.imm | ((raw_inst << 4) & 0b0100000000000);
    in.imm = in.imm | ((raw_inst >> 19) & 0b1000000000000);
    in.simm = signextend<int32_t, 13>(in.imm);
    return in;
}

constexpr Instruction unpack_instruction_U(Instruction& in, uint32_t raw_inst) {
    in.rd = get_rd(raw_inst);
    in.imm = (raw_inst & 0xFFFFF000);
    in.simm = in.imm;  // sign extenstion not needed because
                       // most significant bit doesn't move
    return in;
}

constexpr Instruction unpack_instruction_J(Instruction& in, uint32_t raw_inst) {
    in.rd = get_rd(raw_inst);

    in.imm = 0;
    in.imm = ((raw_inst >> 20) & 0b000000000011111111110);
    in.imm = in.imm | ((raw_inst >> 9) & 0b000000000100000000000);
    in.imm = in.imm | ((raw_inst)&0b011111111000000000000);
    in.imm = in.imm | ((raw_inst >> 11) & 0b100000000000000000000);
    in.simm = signextend<int32_t, 21>(in.imm);
    return in;
}

}  // namespace

// This constructor should be the only publically visible function
Instruction::Instruction(uint32_t raw_inst) {
    switch (get_opcode(raw_inst)) {
        case Opcodes::LUI: {
            unpack_instruction_U(*this, raw_inst);
            operation = Operations::LUI;
            break;
        }
        case Opcodes::AUIPC: {
            unpack_instruction_U(*this, raw_inst);
            operation = Operations::AUIPC;
            break;
        }
        case Opcodes::JAL: {
            unpack_instruction_J(*this, raw_inst);
            operation = Operations::JAL;
            break;
        }
        case Opcodes::JALR: {
            unpack_instruction_I(*this, raw_inst);
            operation = Operations::JALR;
            break;
        }
        case Opcodes::BRANCH: {
            unpack_instruction_B(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    operation = Operations::BEQ;
                    break;
                case 0b001:
                    operation = Operations::BNE;
                    break;
                case 0b100:
                    operation = Operations::BLT;
                    break;
                case 0b101:
                    operation = Operations::BGE;
                    break;
                case 0b110:
                    operation = Operations::BLTU;
                    break;
                case 0b111:
                    operation = Operations::BGEU;
                    break;
            }
            break;
        }
        case Opcodes::LOAD: {
            unpack_instruction_I(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    operation = Operations::LB;
                    break;
                case 0b001:
                    operation = Operations::LH;
                    break;
                case 0b010:
                    operation = Operations::LW;
                    break;
                case 0b100:
                    operation = Operations::LBU;
                    break;
                case 0b101:
                    operation = Operations::LHU;
                    break;
            }
            break;
        }
        case Opcodes::STORE: {
            unpack_instruction_S(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    operation = Operations::SB;
                    break;
                case 0b001:
                    operation = Operations::SH;
                    break;
                case 0b010:
                    operation = Operations::SW;
                    break;
            }
            break;
        }
        case Opcodes::ARITH_IMM: {
            unpack_instruction_I(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    operation = Operations::ADDI;
                    break;
                case 0b010:
                    operation = Operations::SLTI;
                    break;
                case 0b011:
                    operation = Operations::SLTIU;
                    break;
                case 0b100:
                    operation = Operations::XORI;
                    break;
                case 0b110:
                    operation = Operations::ORI;
                    break;
                case 0b111:
                    operation = Operations::ANDI;
                    break;
                case 0b001:
                    operation = Operations::SLLI;
                    break;
                case 0b101:
                    // Shift right instruction uses 5 bit
                    // immediate for shift amount, and
                    // funct7 for what type of shift
                    uint8_t funct7 = get_funct7(raw_inst);
                    if (funct7 == 0) {
                        operation = Operations::SRLI;
                        imm = imm & 0x1F;
                    } else {
                        operation = Operations::SRAI;
                        imm = imm & 0x1F;
                    }
                    break;
            }
            break;
        }
        case Opcodes::ARITH: {
            unpack_instruction_R(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    if (funct7 == 0) {
                        operation = Operations::ADD;
                    } else {
                        operation = Operations::SUB;
                    }
                    break;
                case 0b001:
                    operation = Operations::SLL;
                    break;
                case 0b010:
                    operation = Operations::SLT;
                    break;
                case 0b011:
                    operation = Operations::SLTU;
                    break;
                case 0b100:
                    operation = Operations::XOR;
                    break;
                case 0b101:
                    if (funct7 == 0) {
                        operation = Operations::SRL;
                    } else {
                        operation = Operations::SRA;
                    }
                    break;
                case 0b110:
                    operation = Operations::OR;
                    break;
                case 0b111:
                    operation = Operations::AND;
                    break;
            }
            break;
        }
        case Opcodes::FENCE: {
            unpack_instruction_I(*this, raw_inst);
            switch (funct3) {
                case 0b000:
                    operation = Operations::FENCE;
                    break;
                case 0b001:
                    operation = Operations::FENCEI;
                    break;
            }
            break;
        }
    }
}
