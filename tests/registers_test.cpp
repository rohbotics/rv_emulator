/*
   Copyright 2019 Rohan Agrawal

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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

