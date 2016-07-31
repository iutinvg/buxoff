#include "catch.hpp"
#include "Record.h"
#include "Email.h"

using namespace Buxoff;

TEST_CASE("body", "[email]") {
    auto e = Email({Record(), Record()});
    auto body = e.body();
    auto expected = "  tags: acct:\n  tags: acct:\n";
    REQUIRE(expected == body);
}
