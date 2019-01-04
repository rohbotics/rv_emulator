#include <cstdio>
#include <cstdint>

namespace Register {
enum Registers {
	x0 = 0,
	x1,
	x2,
	x3,
	x4,
	x5,
	x6,
	x7,
	x8,
	x9,
	x10,
	x11,
	x12,
	x13,
	x14,
	x15,
	x16,
	x17,
	x18,
	x19,
	x20,
	x21,
	x22,
	x23,
	x24,
	x25,
	x26,
	x27,
	x28,
	x29,
	x30,
	x31,
	pc,
	COUNT // Number of elements in the enum
};
}

namespace Instructions {
enum Instructions {
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
}

enum class Opcodes {
	LUI        = 0b0110111,
	AUIPC      = 0b0010111,
	JAL        = 0b1101111,
	JALR       = 0b1100111,
	BRANCH     = 0b1100011,
	LOAD       = 0b0000011,
	STORE      = 0b0100011,
	ARITH_IMM  = 0b0010011,
	ARITH      = 0b0110011,
	FENCE      = 0b0001111 
};

constexpr inline Opcodes opcode_from_inst(uint32_t inst) {
	uint8_t opcode = inst & 0x7f;
	return static_cast<Opcodes>(opcode);
}

template <typename T, unsigned B>
inline T signextend(const T x) {
	static_assert(sizeof(T) * 8 > B, "size of type must be larger than sign extended value");
	struct {T x:B;} s;
	return s.x = x;
}

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

void decode(uint32_t inst) {
	switch (opcode_from_inst(inst)) {
		case Opcodes::LUI: {
			uint8_t rd = get_rd(inst);
			uint16_t imm = inst & 0xFFFFF000;
			printf("LUI x%d ← #%d\n", rd, imm);
			break;
		}
		case Opcodes::AUIPC: {
			uint8_t rd = get_rd(inst);
			uint16_t imm = inst & 0xFFFFF000;
			printf("AUIPC x%d ← #(pc + %d)\n", rd, imm);
			break;
		}
		case Opcodes::JAL: {
			int32_t offset = 0;
			offset = offset | ((inst >> 20) & 0b000000000011111111110);
			offset = offset | ((inst >> 9)  & 0b000000000100000000000);
			offset = offset | ((inst << 4)  & 0b011111111000000000000);
			offset = offset | ((inst >> 11) & 0b100000000000000000000);
			offset = signextend<int32_t, 21>(offset);
			printf("JAL\n");
			break;
		}
		case Opcodes::JALR: {
			uint8_t rd = get_rd(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t offset = (inst >> 20) & 0xFFF;
			offset = signextend<int16_t, 12>(offset);
			printf("JALR x%d, x%d, #%d\n", rd, rs1, offset);
			break;
		}
		case Opcodes::BRANCH: {
			printf("BRANCH: ");
			uint8_t funct3 = get_funct3(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t rs2 = get_rs2(inst);
			int16_t offset = 0;
			offset = offset | ((inst >> 7)  & 0b0000000011110);
			offset = offset | ((inst >> 20) & 0b0011111100000);
			offset = offset | ((inst << 4)  & 0b0100000000000);
			offset = offset | ((inst >> 19) & 0b1000000000000);
			offset = signextend<int16_t, 13>(offset);

			switch (funct3) {
				case 0b000:
					printf("BEQ x%d,x%d,%d\n", rs1, rs2, offset);
					break;
				case 0b001:
					printf("BNE x%d,x%d,%d\n", rs1, rs2, offset);
					break;
				case 0b100:
					printf("BLT x%d,x%d,%d\n", rs1, rs2, offset);
					break;
				case 0b101:
					printf("BGE x%d,x%d,%d\n", rs1, rs2, offset);
					break;
				case 0b110:
					printf("BLTU x%d,x%d,%d\n", rs1, rs2, offset);
					break;
				case 0b111:
					printf("BGEU x%d,x%d,%d\n", rs1, rs2, offset);
					break;
			}
			break;
		}
		case Opcodes::LOAD: {
			printf("LOAD: ");
			uint8_t funct3 = get_funct3(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t rd = get_rd(inst);
			uint8_t offset = (inst >> 20) & 0xFFF;
			offset = signextend<int16_t, 12>(offset);
			switch (funct3) {
				case 0b000:
					printf("LB x%d ← [x%d + %d]\n", rd, rs1, offset);
					break;
				case 0b001:
					printf("LH x%d ← [x%d + %d]\n", rd, rs1, offset);
					break;
				case 0b010:
					printf("LW x%d ← [x%d + %d]\n", rd, rs1, offset);
					break;
				case 0b100:
					printf("LBU x%d ← [x%d + %d]\n", rd, rs1, offset);
					break;
				case 0b101:
					printf("LHU x%d ← [x%d + %d]\n", rd, rs1, offset);
					break;
			}
			break;
		}
		case Opcodes::STORE: {
			printf("STORE: ");
			uint8_t funct3 = get_funct3(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t rs2 = get_rs2(inst);
			int16_t offset = 0;
			offset = offset | ((inst >> 7)  & 0b000000011111);
			offset = offset | ((inst >> 20) & 0b111111100000);
			offset = signextend<int16_t, 12>(offset);
			switch (funct3) {
				case 0b000:
					printf("SB [x%d + #%d] ← x%d\n", rs1, offset, rs2);
					break;
				case 0b001:
					printf("SH [x%d + #%d] ← x%d\n", rs1, offset, rs2);
					break;
				case 0b010:
					printf("SW [x%d + #%d] ← x%d\n", rs1, offset, rs2);
					break;
			}
			break;
		}
		case Opcodes::ARITH_IMM: {
			printf("ARITH_IMM: ");
			uint8_t funct3 = get_funct3(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t rd = get_rd(inst);
			uint16_t imm = (inst >> 20) & 0xFFF;
			int16_t simm = signextend<int16_t, 12>(imm);
			switch (funct3) {
				case 0b000:
					printf("ADDI x%d ← x%d + #%d\n", rd, rs1, simm);
					break;
				case 0b010:
					printf("SLTI x%d ← x%d < #%d\n", rd, rs1, simm);
					break;
				case 0b011:
					printf("SLTIU x%d ← x%d < #%d\n", rd, rs1, imm);
					break;
				case 0b100:
					printf("XORI x%d ← x%d ^ #%d\n", rd, rs1, imm);
					break;
				case 0b110:
					printf("ORI x%d ← x%d | #%d\n", rd, rs1, imm);
					break;
				case 0b111:
					printf("ANDI x%d ← x%d & #%d\n", rd, rs1, imm);
					break;
				case 0b001:
					printf("SLLI\n");
					break;
				case 0b101:
					uint8_t funct7 = get_funct7(inst);
					if (funct7 == 0) {
						printf("SRLI\n");
					} else {
						printf("SRAI\n");
					}
					break;
			}
			break;
		}
		case Opcodes::ARITH: {
			printf("ARITH: ");
			uint8_t funct3 = get_funct3(inst);
			uint8_t funct7 = get_funct7(inst);
			uint8_t rs1 = get_rs1(inst);
			uint8_t rs2 = get_rs2(inst);
			uint8_t rd = get_rd(inst);
			uint16_t imm = (inst >> 20) & 0xFFF;
			switch (funct3) {
				case 0b000:
					if (funct7 == 0) {
						printf("ADD x%d ← x%d + x%d\n", rd, rs1, rs2);
					} else {
						printf("SUB x%d ← x%d - x%d\n", rd, rs1, rs2);
					}
					break;
				case 0b001:
					printf("SLL x%d ← x%d << x%d\n", rd, rs1, rs2);
					break;
				case 0b010:
					printf("SLT x%d ← x%d < x%d\n", rd, rs1, rs2);
					break;
				case 0b011:
					printf("SLTU x%d ← x%d < x%d\n", rd, rs1, rs2);
					break;
				case 0b100:
					printf("XOR x%d ← x%d ^ x%d\n", rd, rs1, rs2);
					break;
				case 0b101:
					if (funct7 == 0) {
						printf("SRL x%d ← x%d >> x%d\n", rd, rs1, rs2);
					} else {
						printf("SRA x%d ← x%d >> x%d\n", rd, rs1, rs2);
					}
					break;
				case 0b110:
					printf("OR x%d ← x%d | x%d\n", rd, rs1, rs2);
					break;
				case 0b111:
					printf("AND x%d ← x%d & x%d\n", rd, rs1, rs2);
					break;
			}
			break;
		}
		case Opcodes::FENCE: {
			printf("FENCE: ");
			uint8_t funct3 = (inst >> 12) & 0x07;
			switch (funct3) {
				case 0b000:
					printf("FENCE\n");
					break;
				case 0b001:
					printf("FENCEI\n");
					break;
			}
			break;
		}
	}
}

uint32_t reg[Register::COUNT];


#include "test_instructions.h"

int main(int argc, char* argv[]) {
	decode(0b00000000000100010001000011100011);
	decode(0b10000000000100010000000001100011);
	decode(0b10000000000100010110000011100011);
	
	decode(0b01111110000000000000111111100011);
	decode(0b00000000101010011000100001100011);


	decode(0b01001001000100001000000010010011); //ADDI
	decode(0b00000000000100001100000010010011); //XORI

	decode(0b00000000001000001000000110110011); //ADD

	printf("\n");

	decode(ibeq);
	decode(ibne);
	decode(iblt);
	decode(ibge);
	decode(ibltu);
	decode(ibgeu);
	decode(ijal);
	decode(ijalr);
	decode(ilui);
	decode(iauipc);
	
	decode(iaddi);
	decode(islli);
	decode(islti);
	decode(isltiu);
	decode(ixori);
	decode(isrli);
	decode(israi);
	decode(iori);
	decode(iandi);

	decode(iadd);
	decode(isll);
	decode(islt);
	decode(isltu);
	decode(ixor);
	decode(isrl);
	decode(isra);
	decode(ior);
	decode(iand);

}


