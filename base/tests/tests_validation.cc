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

TEST_CASE("email_is", "[validation]") {
    REQUIRE(is_valid_email("user@domain.com") == true);
    REQUIRE(is_valid_email("") == false);
    REQUIRE(is_valid_email("user") == false);
    REQUIRE(is_valid_email("domain.com") == false);
    REQUIRE(is_valid_email("user@domain@com") == false);
    REQUIRE(is_valid_email("user.domain.com") == false);
}
