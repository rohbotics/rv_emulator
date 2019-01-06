#include <cstdio>
#include <cstdint>
#include <stdexcept>

#include <unistd.h>
#include "decoding.h"

void print_instruction(uint32_t raw_inst) {
    auto inst = decode_instruction(raw_inst);

    switch (inst.operation) {
        case Operations::LUI:
            printf("LUI x%d ← #%d\n", inst.rd, inst.imm);
            break;
        case Operations::AUIPC:
            printf("AUIPC x%d ← #(pc + %d)\n", inst.rd, inst.imm);
            break;
        case Operations::JAL:
            printf("JAL %d, %d\n", inst.rd, inst.simm);
            break;
        case Operations::JALR:
            printf("JALR x%d, x%d, #%d\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::BEQ:
            printf("BEQ x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::BNE:
            printf("BNE x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::BLT:
            printf("BLT x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::BGE:
            printf("BGE x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::BLTU:
            printf("BLTU x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::BGEU:
            printf("BGEU x%d,x%d,%d\n", inst.rs1, inst.rs2, inst.simm);
            break;
        case Operations::LB:
            printf("LB x%d ← [x%d + %d]\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::LH:
            printf("LH x%d ← [x%d + %d]\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::LW:
            printf("LW x%d ← [x%d + %d]\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::LBU:
            printf("LBU x%d ← [x%d + %d]\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::LHU:
            printf("LHU x%d ← [x%d + %d]\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::SB:
            printf("SB [x%d + #%d] ← x%d\n", inst.rs1, inst.simm, inst.rs2);
            break;
        case Operations::SH:
            printf("SH [x%d + #%d] ← x%d\n", inst.rs1, inst.simm, inst.rs2);
            break;
        case Operations::SW:
            printf("SW [x%d + #%d] ← x%d\n", inst.rs1, inst.simm, inst.rs2);
            break;
        case Operations::ADDI:
            printf("ADDI x%d ← x%d + #%d\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::SLTI:
            printf("SLTI x%d ← x%d < #%d\n", inst.rd, inst.rs1, inst.simm);
            break;
        case Operations::SLTIU:
            printf("SLTIU x%d ← x%d < #%d\n", inst.rd, inst.rs1, inst.imm);
            break;
        case Operations::XORI:
            printf("XORI x%d ← x%d ^ #%d\n", inst.rd, inst.rs1, inst.imm);
            break;
        case Operations::ORI:
            printf("ORI x%d ← x%d | #%d\n", inst.rd, inst.rs1, inst.imm);
            break;
        case Operations::ANDI:
            printf("ANDI x%d ← x%d & #%d\n", inst.rd, inst.rs1, inst.imm);
            break;
        case Operations::SLLI:
            printf("SLLI\n");
            break;
        case Operations::SRLI:
            printf("SRLI\n");
            break;
        case Operations::SRAI:
            printf("SRAI\n");
            break;
        case Operations::ADD:
            printf("ADD x%d ← x%d + x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SUB:
            printf("SUB x%d ← x%d - x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SLL:
            printf("SLL x%d ← x%d << x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SLT:
            printf("SLT x%d ← x%d < x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SLTU:
            printf("SLTU x%d ← x%d < x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::XOR:
            printf("XOR x%d ← x%d ^ x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SRL:
            printf("SRL x%d ← x%d >> x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::SRA:
            printf("SRA x%d ← x%d >> x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::OR:
            printf("OR x%d ← x%d | x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::AND:
            printf("AND x%d ← x%d & x%d\n", inst.rd, inst.rs1, inst.rs2);
            break;
        case Operations::FENCE:
            printf("FENCE\n");
            break;
        case Operations::FENCEI:
            printf("FENCEI\n");
            break;
    }
}

int32_t registers[32] = {};
uint32_t program[10] = {
    0b00000000111100000000010110010011,  // set x11 to 15
    0b00000000000100000000010100010011,  // set x10 to 0
    0b00000000000101010000010100010011,  // ADDI x10, x10, 1
    // 0b11111110101001011100110011100011, // BLT x11, x10, -8
    0b00000000101001011100010001100011,  // BLT x11, x10, +8
    0b11111111100111111111000001101111   // JAL x0, -8
};

unsigned int END_PROGRAM = 5;

int main() {
    uint32_t program_counter = 0;
    uint32_t cycle = 1;

    while (program_counter < END_PROGRAM) {
        const auto raw_inst = program[program_counter];
        printf("-----------------------------------------\n");
        print_instruction(raw_inst);
        const auto inst = decode_instruction(raw_inst);

        bool pc_set = false;
        switch (inst.operation) {
            case Operations::ADDI:
                if (inst.rd != 0)
                    registers[inst.rd] = registers[inst.rs1] + inst.simm;
                break;
            case Operations::JAL:
                if (inst.rd != 0)
                    registers[inst.rd] = program_counter + 1;
                program_counter += (inst.simm / 4);
                pc_set = true;
                break;
            case Operations::BLT:
                if (registers[inst.rs1] < registers[inst.rs2]) {
                    program_counter += (inst.simm / 4);
                    pc_set = true;
                }
                break;
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
