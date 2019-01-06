#include "decoding.h"

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

constexpr inline static Opcodes get_opcode(uint32_t inst) {
    uint8_t opcode = inst & 0x7f;
    return static_cast<Opcodes>(opcode);
}

constexpr static Instruction unpack_instruction_R(uint32_t raw_inst) {
    Instruction el = {};
    el.funct7 = get_funct7(raw_inst);
    el.funct3 = get_funct3(raw_inst);
    el.rs1 = get_rs1(raw_inst);
    el.rs2 = get_rs2(raw_inst);
    el.rd = get_rd(raw_inst);
    return el;
}
constexpr static Instruction unpack_instruction_I(uint32_t raw_inst) {
    Instruction el = {};
    el.funct3 = get_funct3(raw_inst);
    el.rs1 = get_rs1(raw_inst);
    el.rd = get_rd(raw_inst);
    el.imm = (raw_inst >> 20) & 0xFFF;
    el.simm = signextend<int32_t, 12>(el.imm);
    return el;
}
constexpr static Instruction unpack_instruction_S(uint32_t raw_inst) {
    Instruction el = {};
    el.funct3 = get_funct3(raw_inst);
    el.rs1 = get_rs1(raw_inst);
    el.rs2 = get_rs2(raw_inst);

    el.imm = 0;
    el.imm = ((raw_inst >> 7) & 0b000000011111);
    el.imm = el.imm | ((raw_inst >> 20) & 0b111111100000);
    el.simm = signextend<int32_t, 12>(el.imm);
    return el;
}
constexpr static Instruction unpack_instruction_B(uint32_t raw_inst) {
    Instruction el = {};
    el.funct3 = get_funct3(raw_inst);
    el.rs1 = get_rs1(raw_inst);
    el.rs2 = get_rs2(raw_inst);

    el.imm = 0;
    el.imm = ((raw_inst >> 7) & 0b0000000011110);
    el.imm = el.imm | ((raw_inst >> 20) & 0b0011111100000);
    el.imm = el.imm | ((raw_inst << 4) & 0b0100000000000);
    el.imm = el.imm | ((raw_inst >> 19) & 0b1000000000000);
    el.simm = signextend<int32_t, 13>(el.imm);
    return el;
}
constexpr static Instruction unpack_instruction_U(uint32_t raw_inst) {
    Instruction el = {};
    el.rd = get_rd(raw_inst);
    el.imm = (raw_inst & 0xFFFFF000);
    el.simm = el.imm;  // sign extenstion not needed because
                       // most significant bit doesn't move
    return el;
}
constexpr static Instruction unpack_instruction_J(uint32_t raw_inst) {
    Instruction el = {};
    el.rd = get_rd(raw_inst);

    el.imm = 0;
    el.imm = ((raw_inst >> 20) & 0b000000000011111111110);
    el.imm = el.imm | ((raw_inst >> 9) & 0b000000000100000000000);
    el.imm = el.imm | ((raw_inst)&0b011111111000000000000);
    el.imm = el.imm | ((raw_inst >> 11) & 0b100000000000000000000);
    el.simm = signextend<int32_t, 21>(el.imm);
    return el;
}

Instruction decode_instruction(uint32_t raw_inst) {
    Instruction el = {};

    switch (get_opcode(raw_inst)) {
        case Opcodes::LUI: {
            el = unpack_instruction_U(raw_inst);
            el.operation = Operations::LUI;
            return el;
            break;
        }
        case Opcodes::AUIPC: {
            el = unpack_instruction_U(raw_inst);
            el.operation = Operations::AUIPC;
            return el;
            break;
        }
        case Opcodes::JAL: {
            el = unpack_instruction_J(raw_inst);
            el.operation = Operations::JAL;
            return el;
            break;
        }
        case Opcodes::JALR: {
            el = unpack_instruction_I(raw_inst);
            el.operation = Operations::JALR;
            return el;
            break;
        }
        case Opcodes::BRANCH: {
            el = unpack_instruction_B(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    el.operation = Operations::BEQ;
                    break;
                case 0b001:
                    el.operation = Operations::BNE;
                    break;
                case 0b100:
                    el.operation = Operations::BLT;
                    break;
                case 0b101:
                    el.operation = Operations::BGE;
                    break;
                case 0b110:
                    el.operation = Operations::BLTU;
                    break;
                case 0b111:
                    el.operation = Operations::BGEU;
                    break;
            }

            return el;
            break;
        }
        case Opcodes::LOAD: {
            el = unpack_instruction_I(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    el.operation = Operations::LB;
                    break;
                case 0b001:
                    el.operation = Operations::LH;
                    break;
                case 0b010:
                    el.operation = Operations::LW;
                    break;
                case 0b100:
                    el.operation = Operations::LBU;
                    break;
                case 0b101:
                    el.operation = Operations::LHU;
                    break;
            }
            return el;
            break;
        }
        case Opcodes::STORE: {
            el = unpack_instruction_S(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    el.operation = Operations::SB;
                    break;
                case 0b001:
                    el.operation = Operations::SH;
                    break;
                case 0b010:
                    el.operation = Operations::SW;
                    break;
            }
            return el;
            break;
        }
        case Opcodes::ARITH_IMM: {
            el = unpack_instruction_I(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    el.operation = Operations::ADDI;
                    break;
                case 0b010:
                    el.operation = Operations::SLTI;
                    break;
                case 0b011:
                    el.operation = Operations::SLTIU;
                    break;
                case 0b100:
                    el.operation = Operations::XORI;
                    break;
                case 0b110:
                    el.operation = Operations::ORI;
                    break;
                case 0b111:
                    el.operation = Operations::ANDI;
                    break;
                case 0b001:
                    el.operation = Operations::SLLI;
                    break;
                case 0b101:
                    // Shift right instruction uses 5 bit
                    // immediate for shift amount, and
                    // funct7 for what type of shift
                    uint8_t funct7 = get_funct7(raw_inst);
                    if (funct7 == 0) {
                        el.operation = Operations::SRLI;
                    } else {
                        el.operation = Operations::SRAI;
                        el.imm = el.imm & 0x1F;
                    }
                    break;
            }
            return el;
            break;
        }
        case Opcodes::ARITH: {
            el = unpack_instruction_R(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    if (el.funct7 == 0) {
                        el.operation = Operations::ADD;
                    } else {
                        el.operation = Operations::SUB;
                    }
                    break;
                case 0b001:
                    el.operation = Operations::SLL;
                    break;
                case 0b010:
                    el.operation = Operations::SLT;
                    break;
                case 0b011:
                    el.operation = Operations::SLTU;
                    break;
                case 0b100:
                    el.operation = Operations::XOR;
                    break;
                case 0b101:
                    if (el.funct7 == 0) {
                        el.operation = Operations::SRL;
                    } else {
                        el.operation = Operations::SRA;
                    }
                    break;
                case 0b110:
                    el.operation = Operations::OR;
                    break;
                case 0b111:
                    el.operation = Operations::AND;
                    break;
            }
            return el;
            break;
        }
        case Opcodes::FENCE: {
            el = unpack_instruction_I(raw_inst);
            switch (el.funct3) {
                case 0b000:
                    el.operation = Operations::FENCE;
                    break;
                case 0b001:
                    el.operation = Operations::FENCEI;
                    break;
            }
            return el;
            break;
        }
    }

    // TODO: If we get here then we should throw an unknown instruction error
    return el;
}
