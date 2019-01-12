#include <cstdio>
#include <cstdint>
#include <stdexcept>
#include <fmt/format.h>
#include <unistd.h>

#include "decoding.h"
#include "registers.h"

uint32_t program[10] = {
    0b00000000111100000000010110010011,  // set x11 to 15
    0b00000000000100000000010100010011,  // set x10 to 0
    0b00000000000101010000010100010011,  // ADDI x10, x10, 1
    // 0b11111110101001011100110011100011, // BLT x11, x10, -8
    0b00000000101001011100010001100011,  // BLT x11, x10, +8
    0b11111111100111111111000001101111   // JAL x0, -8
};
unsigned int END_PROGRAM = 5;

Registers registers;

int main() {
    uint32_t program_counter = 0;
    uint32_t cycle = 1;

    while (program_counter < END_PROGRAM) {
        const auto raw_inst = program[program_counter];
        const auto inst = Instruction(raw_inst);
        puts("-----------------------------------------");
        fmt::print("{}\n", inst);

        bool pc_set = false;
        switch (inst.operation) {
            case Operations::JAL:
                registers.set(inst.rd, program_counter + 1);
                program_counter += (inst.simm / 4);
                pc_set = true;
                break;
            case Operations::BEQ: {
                if (registers.get(inst.rs1) == registers.get(inst.rs2)) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
            }
            case Operations::BNE: {
                if (registers.get(inst.rs1) != registers.get(inst.rs2)) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
            }
            case Operations::BLT: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<int32_t>(registers.get(inst.rs2));
                if (rs1 < rs2) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
            }
            case Operations::BLTU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                if (rs1 < rs2) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
            }
            case Operations::BGE: {
                auto rs1 = static_cast<int32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<int32_t>(registers.get(inst.rs2));
                if (rs1 >= rs2) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
            }
            case Operations::BGEU: {
                auto rs1 = static_cast<uint32_t>(registers.get(inst.rs1));
                auto rs2 = static_cast<uint32_t>(registers.get(inst.rs2));
                if (rs1 >= rs2) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
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
            default:
                throw std::runtime_error("unhandled instruction");
        }
        if (!pc_set)
            program_counter++;
        printf("PC:%d, x10: %d, x11: %d, x12: %d \n", program_counter, registers[10], registers[11], registers[12]);
        // usleep(500000); // sleep for 500,000 microseconds (0.5 seconds)
        cycle++;
    }

    printf("\n\n\nRan for %d cycles\n", cycle);
}
