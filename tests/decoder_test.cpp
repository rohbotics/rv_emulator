#include <catch2/catch.hpp>

#include "decoding.h"
#include "test_instructions.h"

TEST_CASE("lui", "[instruction]" ) {
    REQUIRE(decode_instruction(ilui).operation == Operations::LUI);
}
