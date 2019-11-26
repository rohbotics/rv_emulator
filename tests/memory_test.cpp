#include <catch2/catch.hpp>
#include "memory.h"

TEST_CASE("word load/stores", "[memory]") {
    Memory memory(128);
    memory.store32(0, 0xDEADBEEF);
    REQUIRE(memory.load32(0) == 0xDEADBEEF);
    memory.store32(4, 0xDEADBEEF);
    REQUIRE(memory.load32(4) == 0xDEADBEEF);

    // Unaligned read write
    memory.store32(1, 0xDEADBEEF);
    REQUIRE(memory.load32(1) == 0xDEADBEEF);
}

TEST_CASE("half load/stores", "[memory]") {
    Memory memory(128);
    memory.store16(0, 0xBEEF);
    REQUIRE(memory.load16(0) == 0xBEEF);
    memory.store16(4, 0xBEEF);
    REQUIRE(memory.load16(4) == 0xBEEF);

    // Unaligned read write
    memory.store16(1, 0xBEEF);
    REQUIRE(memory.load16(1) == 0xBEEF);
}

TEST_CASE("little endian", "[memory]") {
    Memory memory(128);
    memory.store32(0, 0xDEADBEEF);
    REQUIRE(memory.load8(0) == 0xEF);
    REQUIRE(memory.load8(1) == 0xBE);
    REQUIRE(memory.load8(2) == 0xAD);
    REQUIRE(memory.load8(3) == 0xDE);
    REQUIRE(memory.load16(0) == 0xBEEF);
    REQUIRE(memory.load16(2) == 0xDEAD);

    memory.store32(0, 0);
    memory.store8(0, 0xEF);
    memory.store8(1, 0xBE);
    memory.store8(2, 0xAD);
    memory.store8(3, 0xDE);
    REQUIRE(memory.load32(0) == 0xDEADBEEF);
    REQUIRE(memory.load16(0) == 0xBEEF);
    REQUIRE(memory.load16(2) == 0xDEAD);

    memory.store32(0, 0);
    memory.store16(0, 0xBEEF);
    memory.store16(2, 0xDEAD);
    REQUIRE(memory.load32(0) == 0xDEADBEEF);
    REQUIRE(memory.load8(0) == 0xEF);
    REQUIRE(memory.load8(1) == 0xBE);
    REQUIRE(memory.load8(2) == 0xAD);
    REQUIRE(memory.load8(3) == 0xDE);
}
