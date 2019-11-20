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

    explicit Instruction(Operations op) : operation(op) {};
    explicit Instruction (uint32_t raw_inst);
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
                return format_to(ctx.begin(), "LUI x{} ← #{}", inst.rd, inst.imm);
            case Operations::AUIPC:
                return format_to(ctx.begin(), "AUIPC x{} ← #(pc + {})", inst.rd, inst.imm);
            case Operations::JAL:
                return format_to(ctx.begin(), "JAL x{}, #{}", inst.rd, inst.simm);
            case Operations::JALR:
                return format_to(ctx.begin(), "JALR x{}, x{}, #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::BEQ:
                return format_to(ctx.begin(), "BEQ (x{} = x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BNE:
                return format_to(ctx.begin(), "BNE (x{} != x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BLT:
                return format_to(ctx.begin(), "BLT (x{} < x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BGE:
                return format_to(ctx.begin(), "BGE (x{} > x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BLTU:
                return format_to(ctx.begin(), "BLTU (x{} < x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::BGEU:
                return format_to(ctx.begin(), "BGEU (x{} > x{}), PC + #{}", inst.rs1, inst.rs2, inst.simm);
            case Operations::LB:
                return format_to(ctx.begin(), "LB x{} ← [x{} + {}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LH:
                return format_to(ctx.begin(), "LH x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LW:
                return format_to(ctx.begin(), "LW x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LBU:
                return format_to(ctx.begin(), "LBU x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::LHU:
                return format_to(ctx.begin(), "LHU x{} ← [x{} + #{}]", inst.rd, inst.rs1, inst.simm);
            case Operations::SB:
                return format_to(ctx.begin(), "SB [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::SH:
                return format_to(ctx.begin(), "SH [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::SW:
                return format_to(ctx.begin(), "SW [x{} + #{}] ← x{}", inst.rs1, inst.simm, inst.rs2);
            case Operations::ADDI:
                return format_to(ctx.begin(), "ADDI x{} ← x{} + #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::SLTI:
                return format_to(ctx.begin(), "SLTI x{} ← x{} < #{}", inst.rd, inst.rs1, inst.simm);
            case Operations::SLTIU:
                return format_to(ctx.begin(), "SLTIU x{} ← x{} < #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::XORI:
                return format_to(ctx.begin(), "XORI x{} ← x{} ^ #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::ORI:
                return format_to(ctx.begin(), "ORI x{} ← x{} | #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::ANDI:
                return format_to(ctx.begin(), "ANDI x{} ← x{} & #{}", inst.rd, inst.rs1, inst.imm);
            case Operations::SLLI:
                return format_to(ctx.begin(), "SLLI");
            case Operations::SRLI:
                return format_to(ctx.begin(), "SRLI");
            case Operations::SRAI:
                return format_to(ctx.begin(), "SRAI");
            case Operations::ADD:
                return format_to(ctx.begin(), "ADD x{} ← x{} + x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SUB:
                return format_to(ctx.begin(), "SUB x{} ← x{} - x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLL:
                return format_to(ctx.begin(), "SLL x{} ← x{} << x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLT:
                return format_to(ctx.begin(), "SLT x{} ← x{} < x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SLTU:
                return format_to(ctx.begin(), "SLTU x{} ← x{} < x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::XOR:
                return format_to(ctx.begin(), "XOR x{} ← x{} ^ x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SRL:
                return format_to(ctx.begin(), "SRL x{} ← x{} >> x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::SRA:
                return format_to(ctx.begin(), "SRA x{} ← x{} >> x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::OR:
                return format_to(ctx.begin(), "OR x{} ← x{} | x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::AND:
                return format_to(ctx.begin(), "AND x{} ← x{} & x{}", inst.rd, inst.rs1, inst.rs2);
            case Operations::FENCE:
                return format_to(ctx.begin(), "FENCE");
            case Operations::FENCEI:
                return format_to(ctx.begin(), "FENCEI");
        }
        return format_to(ctx.begin(), "");
    }
};
}  // namespace fmt


