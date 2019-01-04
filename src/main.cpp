#include <cstdio>
#include <cstdint>

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


