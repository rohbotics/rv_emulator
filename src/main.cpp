#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <fmt/format.h>
#include <unistd.h>

#include "decoding.h"
#include "registers.h"

uint32_t program[10] = {
    0b000100000000'00000'001'01100'0000011,   // Load byte into x12
    0b000100000000'00000'010'01100'0000011,   // Load byte into x12
    0b00000000111100000000010110010011,       // set x11 to 15
    0b00000000000100000000010100010011,       // set x10 to 0
    0b00000000000101010000010100010011,       // ADDI x10, x10, 1
    0b1111111'01011'01010'100'11001'1100011,  // BLT x10, x11, -8
};
unsigned int END_PROGRAM = 6;

Registers registers;
uint8_t memory[128 * 1024] = { 0 };  // 128K of memory

uint32_t fetch(uint8_t* instruction_mem) {
    uint32_t inst = 0x00;
    inst |= instruction_mem[0];
    inst |= (instruction_mem[1] << 8);
    inst |= (instruction_mem[2] << 16);
    inst |= (instruction_mem[3] << 24);
    return inst;
}

// Adapted from http://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
template <typename T, unsigned B>
constexpr inline static T signextend(const T x) {
    static_assert(sizeof(T) * 8 > B, "size of type must be larger than sign extended value");
    static_assert(std::is_signed<T>::value, "return must be of signed type");
    struct {
        T x : B;
    } s = { 0 };
    return s.x = x;
}

int main() {
    uint32_t program_counter = 0x100;

    // Copy the program defined above into the memory
    // start the program at the initial PC value
    uint8_t* prog_mem = &memory[program_counter];
    for (unsigned int i = 0; i < END_PROGRAM; i++) {
        prog_mem[4 * i] = program[i] & 0xFF;
        prog_mem[(4 * i) + 1] = (program[i] >> 8) & 0xFF;
        prog_mem[(4 * i) + 2] = (program[i] >> 16) & 0xFF;
        prog_mem[(4 * i) + 3] = (program[i] >> 24) & 0xFF;
    }

    uint32_t cycle = 1;
    while (program_counter < 0x100 + END_PROGRAM * 4) {
        // const auto raw_inst = program[program_counter/4];
        const auto raw_inst = fetch(&memory[program_counter]);
        const auto inst = Instruction(raw_inst);
        puts("-----------------------------------------");
        fmt::print("@{}: {}\n", program_counter, inst);

        program_counter += 4;

        switch (inst.operation) {
            case Operations::JAL:
                registers.set(inst.rd, program_counter + 1);
                program_counter += (inst.simm);
                break;
            case Operations::BEQ: {
                if (registers.get(inst.rs1) == registers.get(inst.rs2)) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::BNE: {
                if (registers.get(inst.rs1) != registers.get(inst.rs2)) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::BLT: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<int32_t>(registers.get(inst.rs2));
                if (rs1 < rs2) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::BLTU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                if (rs1 < rs2) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::BGE: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<int32_t>(registers.get(inst.rs2));
                if (rs1 >= rs2) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::BGEU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                if (rs1 >= rs2) {
                    program_counter += inst.simm;
                }
                break;
            }
            case Operations::ADDI:
                registers.set(inst.rd, registers.get(inst.rs1) + inst.simm);
                break;
            case Operations::ANDI:
                registers.set(inst.rd, registers.get(inst.rs1) & inst.simm);
                break;
            case Operations::ORI:
                registers.set(inst.rd, registers.get(inst.rs1) | inst.simm);
                break;
            case Operations::XORI:
                registers.set(inst.rd, registers.get(inst.rs1) ^ inst.simm);
                break;
            case Operations::SLLI:
                registers.set(inst.rd, registers.get(inst.rs1) << inst.imm);
                break;

            // For right shifts
            // https://stackoverflow.com/questions/7622/are-the-shift-operators-arithmetic-or-logical-in-c
            case Operations::SRLI: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                registers.set(inst.rd, rs1 >> inst.imm);
                break;
            }
            case Operations::SRAI: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                registers.set(inst.rd, rs1 >> inst.simm);
                break;
            }
            case Operations::SLTI: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                if (rs1 < inst.simm)
                    registers.set(inst.rd, 1);
                else
                    registers.set(inst.rd, 0);
                break;
            }
            case Operations::SLTIU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                if (rs1 < inst.imm)
                    registers.set(inst.rd, 1);
                else
                    registers.set(inst.rd, 0);
                break;
            }
            case Operations::ADD:
                registers.set(inst.rd, registers.get(inst.rs1) + registers.get(inst.rs2));
                break;
            case Operations::SUB:
                registers.set(inst.rd, registers.get(inst.rs1) - registers.get(inst.rs2));
                break;
            case Operations::AND:
                registers.set(inst.rd, registers.get(inst.rs1) & registers.get(inst.rs2));
                break;
            case Operations::OR:
                registers.set(inst.rd, registers.get(inst.rs1) | registers.get(inst.rs2));
                break;
            case Operations::XOR:
                registers.set(inst.rd, registers.get(inst.rs1) ^ registers.get(inst.rs2));
                break;
            case Operations::SLL:
                registers.set(inst.rd, registers.get(inst.rs1) << registers.get(inst.rs2));
                break;

            // For right shifts
            // https://stackoverflow.com/questions/7622/are-the-shift-operators-arithmetic-or-logical-in-c
            case Operations::SRL: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                registers.set(inst.rd, rs1 >> rs2);
                break;
            }
            case Operations::SRA: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                registers.set(inst.rd, rs1 >> rs2);
                break;
            }
            case Operations::SLT: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<int32_t>(registers.get(inst.rs2));
                if (rs1 < rs2)
                    registers.set(inst.rd, 1);
                else
                    registers.set(inst.rd, 0);
                break;
            }
            case Operations::SLTU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                if (rs1 < rs2)
                    registers.set(inst.rd, 1);
                else
                    registers.set(inst.rd, 0);
                break;
            }
            case Operations::LB: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                uint8_t value = memory[rs1 + inst.simm];
                registers.set(inst.rd, signextend<int32_t, 8>(value));
                break;
            }
            case Operations::LBU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                uint8_t value = memory[rs1 + inst.simm];
                registers.set(inst.rd, value);
                break;
            }
            case Operations::LH: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                uint16_t value = memory[rs1 + inst.simm];
                value |= (memory[rs1 + inst.simm + 1] << 8);
                registers.set(inst.rd, signextend<int32_t, 16>(value));
                break;
            }
            case Operations::LHU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                uint32_t value = memory[rs1 + inst.simm];
                value |= (memory[rs1 + inst.simm + 1] << 8);
                registers.set(inst.rd, value);
                break;
            }
            case Operations::LW: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                uint32_t value = memory[rs1 + inst.simm];
                value |= (memory[rs1 + inst.simm + 1] << 8);
                value |= (memory[rs1 + inst.simm + 2] << 16);
                value |= (memory[rs1 + inst.simm + 3] << 24);
                registers.set(inst.rd, value);
                break;
            }
            case Operations::SB: {
                auto mem_addr = registers.get(inst.rs1) + inst.simm;
                memory[mem_addr] = registers.get(inst.rs2) & 0xFF;
                break;
            }
            case Operations::SH: {
                auto mem_addr = registers.get(inst.rs1) + inst.simm;
                auto data = registers.get(inst.rs2);
                memory[mem_addr] = data & 0xFF;
                memory[mem_addr + 1] = (data >> 8) & 0xFF;
                break;
            }
            case Operations::SW: {
                auto mem_addr = registers.get(inst.rs1) + inst.simm;
                auto data = registers.get(inst.rs2);
                memory[mem_addr] = data & 0xFF;
                memory[mem_addr + 1] = (data >> 8) & 0xFF;
                memory[mem_addr + 2] = (data >> 16) & 0xFF;
                memory[mem_addr + 3] = (data >> 24) & 0xFF;
                break;
            }
            default:
                throw std::runtime_error("unhandled instruction");
        }
        printf("PC:%d, x10: %x, x11: %x, x12: %x \n", program_counter, registers[10], registers[11], registers[12]);
        usleep(500000);  // sleep for 500,000 microseconds (0.5 seconds)
        cycle++;
    }

    printf("\n\n\nRan for %d cycles\n", cycle);
}
