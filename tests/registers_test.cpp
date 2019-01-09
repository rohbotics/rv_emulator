#include <catch2/catch.hpp>
#include <stdexcept>
#include "registers.h"

TEST_CASE("get/set on 0", "[registers]") {
    Registers r;
    REQUIRE(r.get(0) == 0);
    REQUIRE(r[0] == 0);
    REQUIRE(r.set(0, 5) == 0);
    REQUIRE(r.get(0) == 0);
    REQUIRE(r[0] == 0);
}

TEST_CASE("get/set", "[registers]") {
    Registers r;
    for (int i = 1; i < 32; i++) {
        REQUIRE(r.get(i) == 0);
        REQUIRE(r[i] == 0);
        REQUIRE(r.set(i, 5) == 5);
        REQUIRE(r.get(i) == 5);
        REQUIRE(r[i] == 5);
    }
}

TEST_CASE("out of bounds throws", "[registers]") {
    Registers r;
    REQUIRE_THROWS_AS(r.get(100), std::out_of_range);
    REQUIRE_THROWS_AS(r[100], std::out_of_range);
    REQUIRE_THROWS_AS(r.set(100, 1), std::out_of_range);
}

