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
#include <fmt/format.h>

enum class Operations {
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    FENCE,
    FENCEI
};

struct Instruction {
    // structure to hold all the pieces of an instruction,
    // // only the relevant elements will be set
    // Eg if an instruction doesn't have an rd than it will not be set

    Operations operation;
    uint8_t funct3;
    uint8_t funct7;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;

    uint32_t imm;
    int32_t simm;  // sign extended version

    Instruction() = default;

    explicit Instruction(Operations op) : operation(op){};
    explicit Instruction(uint32_t raw_inst);
};

namespace fmt
{
template <>
struct formatter<Instruction> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Instruction& inst, FormatContext& ctx) {
        switch (inst.operation) {
            case Operations::LUI:
                return format_to(ctx.out(), "LUI x{} ← #{}", inst.rd, inst.imm);
            case Operations::AUIPC:
                return format_to(ctx.out(), "AUIPC x{} ← #(pc + {})", inst.rd, inst.imm);
            case Operations::JAL:
                return format_to(ctx.out(), "JAL x{}, #{}", inst.rd, inst.simm);
            case Operations::JALR:
                return format_to(ctx.out(), "JALR x{}, x{}, #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::BEQ:
                return format_to(ctx.out(), "BEQ (x{} = x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BNE:
                return format_to(ctx.out(), "BNE (x{} != x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BLT:
                return format_to(ctx.out(), "BLT (x{} < x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BGE:
                return format_to(ctx.out(), "BGE (x{} > x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BLTU:
                return format_to(ctx.out(), "BLTU (x{} < x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BGEU:
                return format_to(ctx.out(), "BGEU (x{} > x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::LB:
                return format_to(ctx.out(), "LB x{} ← [x{} + {}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LH:
                return format_to(ctx.out(), "LH x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LW:
                return format_to(ctx.out(), "LW x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LBU:
                return format_to(ctx.out(), "LBU x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LHU:
                return format_to(ctx.out(), "LHU x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::SB:
                return format_to(ctx.out(), "SB [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::SH:
                return format_to(ctx.out(), "SH [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::SW:
                return format_to(ctx.out(), "SW [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::ADDI:
                return format_to(ctx.out(), "ADDI x{} ← x{} + #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::SLTI:
                return format_to(ctx.out(), "SLTI x{} ← x{} < #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::SLTIU:
                return format_to(ctx.out(), "SLTIU x{} ← x{} < #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::XORI:
                return format_to(ctx.out(), "XORI x{} ← x{} ^ #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::ORI:
                return format_to(ctx.out(), "ORI x{} ← x{} | #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::ANDI:
                return format_to(ctx.out(), "ANDI x{} ← x{} & #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::SLLI:
                return format_to(ctx.out(), "SLLI");
            case Operations::SRLI:
                return format_to(ctx.out(), "SRLI");
            case Operations::SRAI:
                return format_to(ctx.out(), "SRAI");
            case Operations::ADD:
                return format_to(ctx.out(), "ADD x{} ← x{} + x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SUB:
                return format_to(ctx.out(), "SUB x{} ← x{} - x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLL:
                return format_to(ctx.out(), "SLL x{} ← x{} << x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLT:
                return format_to(ctx.out(), "SLT x{} ← x{} < x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLTU:
                return format_to(ctx.out(), "SLTU x{} ← x{} < x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::XOR:
                return format_to(ctx.out(), "XOR x{} ← x{} ^ x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SRL:
                return format_to(ctx.out(), "SRL x{} ← x{} >> x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SRA:
                return format_to(ctx.out(), "SRA x{} ← x{} >> x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::OR:
                return format_to(ctx.out(), "OR x{} ← x{} | x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::AND:
                return format_to(ctx.out(), "AND x{} ← x{} & x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::FENCE:
                return format_to(ctx.out(), "FENCE");
            case Operations::FENCEI:
                return format_to(ctx.out(), "FENCEI");
        }
        return format_to(ctx.out(), "");
    }
};
}  // namespace fmt
