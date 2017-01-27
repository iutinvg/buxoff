#include "catch.hpp"
#include "Validation.h"

using namespace Buxoff;

TEST_CASE("amount", "[validation]") {
    REQUIRE_NOTHROW(validate_amount("0"));
    REQUIRE_NOTHROW(validate_amount("1.12"));
    REQUIRE_THROWS_AS(validate_amount("zero"), ValidationError);
    REQUIRE_THROWS_AS(validate_amount(""), ValidationError);
}

TEST_CASE("account", "[validation]") {
    REQUIRE_NOTHROW(validate_account("something"));
    REQUIRE_THROWS_AS(validate_account(""), ValidationError);
}

TEST_CASE("email", "[validation]") {
    REQUIRE_NOTHROW(validate_email("user@domain.com"));
    REQUIRE_THROWS_AS(validate_email(""), ValidationError);
    REQUIRE_THROWS_AS(validate_email("user"), ValidationError);
    REQUIRE_THROWS_AS(validate_email("domain.com"), ValidationError);
    REQUIRE_THROWS_AS(validate_email("user@domain@com"), ValidationError);
    REQUIRE_THROWS_AS(validate_email("user.domain.com"), ValidationError);
}
