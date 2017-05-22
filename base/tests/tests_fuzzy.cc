#include "catch.hpp"
#include "FuzzySearch.h"

using namespace Buxoff;

// it can be wrong on utf text, I'm sure

TEST_CASE("similar-UTFru", "[fuzzy]") {
    REQUIRE(search("ша", "шахта"));
}

TEST_CASE("similar-UTFru1", "[fuzzy]") {
    REQUIRE(search("шхт", "шахта"));
}

TEST_CASE("similar-UTF1", "[fuzzy]") {
    REQUIRE(search("语言", "php语言"));
}

TEST_CASE("similar-UTF2", "[fuzzy]") {
    REQUIRE(search("hp语", "php语言"));
}

TEST_CASE("similar-UTF3", "[fuzzy]") {
    REQUIRE(search("Py开发", "Python开发者"));
}

TEST_CASE("similar-UTF4", "[fuzzy]") {
    REQUIRE(!search("Py 开发", "Python开发者"));
}

TEST_CASE("similar-UTF5", "[fuzzy]") {
    REQUIRE(search("爪哇进阶", "爪哇开发进阶"));
}

TEST_CASE("similar-UTF6", "[fuzzy]") {
    REQUIRE(search("格式工具", "非常简单的格式化工具"));
}

TEST_CASE("similar-UTF7", "[fuzzy]") {
    REQUIRE(search("正则", "学习正则表达式怎么学习"));
}

TEST_CASE("similar-UTF8", "[fuzzy]") {
    REQUIRE(!search("学习正则", "正则表达式怎么学习"));
}

TEST_CASE("similar1", "[fuzzy]") {
    REQUIRE(search("car", "cartwheel"));
}

TEST_CASE("similar2", "[fuzzy]") {
    REQUIRE(search("cwhl", "cartwheel"));
}

TEST_CASE("similar3", "[fuzzy]") {
    REQUIRE(search("cwheel", "cartwheel"));
}

TEST_CASE("similar4", "[fuzzy]") {
    REQUIRE(search("cartwheel", "cartwheel"));
}

TEST_CASE("not_similar", "[fuzzy]") {
    REQUIRE(!search("cwheeel", "cartwheel"));
}

TEST_CASE("not_similar2", "[fuzzy]") {
    REQUIRE(!search("lw", "cartwheel"));
}
