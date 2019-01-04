#include <cstdio>
#include <cstdint>

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

constexpr inline Opcodes get_opcode(uint32_t inst) {
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

enum class InstructionFormats {
	R,
	I,
	S,
	B,
	U,
	J
};

struct InstructionElements {
	// structure to hold all the pieces of an instruction, 
	// only the relevant elements will be set
	// Eg if an instruction doesn't have an rd than it will not be set

	Operations instruction;
	uint8_t funct3;
	uint8_t funct7;
	uint8_t rd;
	uint8_t rs1;
	uint8_t rs2;

	uint32_t imm;
	int32_t simm; // sign extended version
};
constexpr InstructionElements unpack_instruction(InstructionFormats f, uint32_t raw_inst) {
	InstructionElements el = {};

	switch (f) {
		case InstructionFormats::R: {
			el.funct7 = get_funct7(raw_inst);
			el.funct3 = get_funct3(raw_inst);
			el.rs1 = get_rs1(raw_inst);
			el.rs2 = get_rs2(raw_inst);
			el.rd = get_rd(raw_inst);
			break;
		}
		case InstructionFormats::I: {
			el.funct3 = get_funct3(raw_inst);
			el.rs1 = get_rs1(raw_inst);
			el.rd = get_rd(raw_inst);
			el.imm = (raw_inst >> 20) & 0xFFF;
			el.simm = signextend<int32_t, 12>(el.imm);
			break;
		}
		case InstructionFormats::S: {
			el.funct3 = get_funct3(raw_inst);
			el.rs1 = get_rs1(raw_inst);
			el.rs2 = get_rs2(raw_inst);

			el.imm = 0;
			el.imm =          ((raw_inst >> 7)  & 0b000000011111);
			el.imm = el.imm | ((raw_inst >> 20) & 0b111111100000);
			el.simm = signextend<int32_t, 12>(el.imm);
			break;
		}
		case InstructionFormats::B: {
			el.funct3 = get_funct3(raw_inst);
			el.rs1 = get_rs1(raw_inst);
			el.rs2 = get_rs2(raw_inst);

			el.imm = 0;
			el.imm =         ((raw_inst >> 7)  & 0b0000000011110);
			el.imm = el.imm | ((raw_inst >> 20) & 0b0011111100000);
			el.imm = el.imm | ((raw_inst << 4)  & 0b0100000000000);
			el.imm = el.imm | ((raw_inst >> 19) & 0b1000000000000);
			el.simm = signextend<int32_t, 13>(el.imm);
			break;
		}
		case InstructionFormats::U: {
			el.rd = get_rd(raw_inst);
			el.imm = (raw_inst & 0xFFFFF000);
			el.simm = el.imm; // sign extenstion not needed because
					  // most significant bit doesn't move
			break;
		}
		case InstructionFormats::J: {
			el.rd = get_rd(raw_inst);
			
			el.imm = 0;
			el.imm =          ((raw_inst >> 20) & 0b000000000011111111110);
			el.imm = el.imm | ((raw_inst >> 9)  & 0b000000000100000000000);
			el.imm = el.imm | ((raw_inst << 4)  & 0b011111111000000000000);
			el.imm = el.imm | ((raw_inst >> 11) & 0b100000000000000000000);
			el.simm = signextend<int32_t, 21>(el.imm);
			break;
		}
	}

	return el;
}

InstructionElements decode_instruction(uint32_t raw_inst) {
	InstructionElements el = {};

	switch (get_opcode(raw_inst)) {
		case Opcodes::LUI: {
			el = unpack_instruction(InstructionFormats::U, raw_inst);
			el.instruction = Operations::LUI;
			return el;
			break;
		}
		case Opcodes::AUIPC: {
			el = unpack_instruction(InstructionFormats::U, raw_inst);
			el.instruction = Operations::AUIPC;
			return el;
			break;
		}
		case Opcodes::JAL: {
			el = unpack_instruction(InstructionFormats::J, raw_inst);
			el.instruction = Operations::JAL;
			return el;
			break;
		}
		case Opcodes::JALR: {
			el = unpack_instruction(InstructionFormats::I, raw_inst);
			el.instruction = Operations::JALR;
			return el;
			break;
		}
		case Opcodes::BRANCH: {
			el = unpack_instruction(InstructionFormats::B, raw_inst);
			switch (el.funct3) {
				case 0b000:
					el.instruction = Operations::BEQ;
					break;
				case 0b001:
					el.instruction = Operations::BNE;
					break;
				case 0b100:
					el.instruction = Operations::BLT;
					break;
				case 0b101:
					el.instruction = Operations::BGE;
					break;
				case 0b110:
					el.instruction = Operations::BLTU;
					break;
				case 0b111:
					el.instruction = Operations::BGEU;
					break;
			}

			return el;
			break;
		}
		case Opcodes::LOAD: {
			el = unpack_instruction(InstructionFormats::I, raw_inst);
			switch (el.funct3) {
				case 0b000:
					el.instruction = Operations::LB;
					break;
				case 0b001:
					el.instruction = Operations::LH;
					break;
				case 0b010:
					el.instruction = Operations::LW;
					break;
				case 0b100:
					el.instruction = Operations::LBU;
					break;
				case 0b101:
					el.instruction = Operations::LHU;
					break;
			}
			return el;
			break;
		}
		case Opcodes::STORE: {
			el = unpack_instruction(InstructionFormats::S, raw_inst);
			switch (el.funct3) {
				case 0b000:
					el.instruction = Operations::SB;
					break;
				case 0b001:
					el.instruction = Operations::SH;
					break;
				case 0b010:
					el.instruction = Operations::SW;
					break;
			}
			return el;
			break;
		}
		case Opcodes::ARITH_IMM: {
			el = unpack_instruction(InstructionFormats::I, raw_inst);
			switch (el.funct3) {
				case 0b000:
					el.instruction = Operations::ADDI;
					break;
				case 0b010:
					el.instruction = Operations::SLTI;
					break;
				case 0b011:
					el.instruction = Operations::SLTIU;
					break;
				case 0b100:
					el.instruction = Operations::XORI;
					break;
				case 0b110:
					el.instruction = Operations::ORI;
					break;
				case 0b111:
					el.instruction = Operations::ANDI;
					break;
				case 0b001:
					el.instruction = Operations::SLLI;
					break;
				case 0b101:
					// Shift right instruction uses 5 bit immediate 
					// for shift amount, and funct7 for what type of shift
					uint8_t funct7 = get_funct7(raw_inst);
					if (funct7 == 0) {
						el.instruction = Operations::SRLI;
					} else {
						el.instruction = Operations::SRAI;
						el.imm = el.imm & 0x1F;
					}
					break;
			}
			return el;
			break;
		}
		case Opcodes::ARITH: {
			el = unpack_instruction(InstructionFormats::R, raw_inst);
			switch (el.funct3) {
				case 0b000:
					if (el.funct7 == 0) {
						el.instruction = Operations::ADD;
					} else {
						el.instruction = Operations::SUB;
					}
					break;
				case 0b001:
					el.instruction = Operations::SLL;
					break;
				case 0b010:
					el.instruction = Operations::SLT;
					break;
				case 0b011:
					el.instruction = Operations::SLTU;
					break;
				case 0b100:
					el.instruction = Operations::XOR;
					break;
				case 0b101:
					if (el.funct7 == 0) {
						el.instruction = Operations::SRL;
					} else {
						el.instruction = Operations::SRA;
					}
					break;
				case 0b110:
					el.instruction = Operations::OR;
					break;
				case 0b111:
					el.instruction = Operations::AND;
					break;
			}
			return el;
			break;
		}
		case Opcodes::FENCE: {
			el = unpack_instruction(InstructionFormats::I, raw_inst);
			switch (el.funct3) {
				case 0b000:
					el.instruction = Operations::FENCE;
					break;
				case 0b001:
					el.instruction = Operations::FENCEI;
					break;
			}
			break;
		}
	}
}

void print_instruction(uint32_t raw_inst) {
	auto inst = decode_instruction(raw_inst);

	switch (inst.instruction) {
		case Operations::LUI:
			printf("LUI x%d ← #%d\n", inst.rd, inst.imm);
			break;
		case Operations::AUIPC:
			printf("AUIPC x%d ← #(pc + %d)\n", inst.rd, inst.imm);
			break;
		case Operations::JAL: 
			printf("JAL\n");
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


#include "test_instructions.h"

int main(int argc, char* argv[]) {
	print_instruction(0b00000000000100010001000011100011);
	print_instruction(0b10000000000100010000000001100011);
	print_instruction(0b10000000000100010110000011100011);
	
	print_instruction(0b01111110000000000000111111100011);
	print_instruction(0b00000000101010011000100001100011);


	print_instruction(0b01001001000100001000000010010011); //ADDI
	print_instruction(0b00000000000100001100000010010011); //XORI

	print_instruction(0b00000000001000001000000110110011); //ADD

	printf("\n");

	print_instruction(ibeq);
	print_instruction(ibne);
	print_instruction(iblt);
	print_instruction(ibge);
	print_instruction(ibltu);
	print_instruction(ibgeu);
	print_instruction(ijal);
	print_instruction(ijalr);
	print_instruction(ilui);
	print_instruction(iauipc);
	
	print_instruction(iaddi);
	print_instruction(islli);
	print_instruction(islti);
	print_instruction(isltiu);
	print_instruction(ixori);
	print_instruction(isrli);
	print_instruction(israi);
	print_instruction(iori);
	print_instruction(iandi);

	print_instruction(iadd);
	print_instruction(isll);
	print_instruction(islt);
	print_instruction(isltu);
	print_instruction(ixor);
	print_instruction(isrl);
	print_instruction(isra);
	print_instruction(ior);
	print_instruction(iand);

}


