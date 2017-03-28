#include "catch.hpp"
#include "UserDefaults.h"

#include "utils.h"

using namespace Buxoff;

TEST_CASE("string", "[userdefaults]") {
    auto s = get_clean_storage();

    ud_put(s, "key", "value");

    std::string str{ud_get(s, "key", std::string{""})};
    REQUIRE(str == "value");

    str = ud_get(s, "unknown", std::string{"boo"});
    REQUIRE(str == "boo");
}

TEST_CASE("bool", "[userdefaults]") {
    auto s = get_clean_storage();

    ud_put(s, "key", true);

    bool b{ud_get(s, "key", false)};
    REQUIRE(b == true);

    b = ud_get(s, "unknow", true);
    REQUIRE(b == true);

    b = ud_get(s, "unknow", false);
    REQUIRE(b == false);
}
