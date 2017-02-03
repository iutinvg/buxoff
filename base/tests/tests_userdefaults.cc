#include "catch.hpp"
#include "UserDefaults.h"

#include "utils.h"

using namespace Buxoff;

TEST_CASE("string", "[userdefaults]") {
    auto s = get_clean_storage();

    put_ud(s, "key", "value");

    std::string str{get_ud(s, "key", std::string{""})};
    REQUIRE(str == "value");

    str = get_ud(s, "unknown", std::string{"boo"});
    REQUIRE(str == "boo");
}

TEST_CASE("bool", "[userdefaults]") {
    auto s = get_clean_storage();

    put_ud(s, "key", true);

    bool b{get_ud(s, "key", false)};
    REQUIRE(b == true);

    b = get_ud(s, "unknow", true);
    REQUIRE(b == true);

    b = get_ud(s, "unknow", false);
    REQUIRE(b == false);
}
