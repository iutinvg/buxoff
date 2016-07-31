#include "catch.hpp"
#include "Record.h"
#include "Email.h"

using namespace Buxoff;

TEST_CASE("body", "[email]") {
    auto e = Email({Record("12", "desc1", {"t1"}, "cash"),
        Record("21", "desc2", {"t2", "t3"}, "cash2")});
    auto body = e.body();
    auto expected = "desc1 12 tags:t1 acct:cash\ndesc2 21 tags:t2,t3 acct:cash2\n";
    REQUIRE(expected == body);
}
