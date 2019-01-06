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
};

Instruction decode_instruction(uint32_t raw_inst);

/*** Inline utility functions ***/

template <typename T, unsigned B>
constexpr inline T signextend(const T x) {
    static_assert(sizeof(T) * 8 > B, "size of type must be larger than sign extended value");
    struct {
        T x : B;
    } s = { 0 };
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
