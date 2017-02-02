#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "json.hpp"
#include "Record.h"


using namespace Buxoff;


TEST_CASE("get_line_N_tags", "[record]") {
    Record r = Record("345.67", "desc", {"tag1", "tag2"}, "cash");
    REQUIRE(r.get_line() == "desc 345.67 tags:tag1,tag2 acct:cash");
}

TEST_CASE("get_line_1_tag", "[record]") {
    Record r = Record("345.67", "desc", {"tag1"}, "cash");
    REQUIRE(r.get_line() == "desc 345.67 tags:tag1 acct:cash");
}

TEST_CASE("from_json", "[record]") {
    nlohmann::json o = "{\"acct\":\"cash\",\"amount\":\"345.67\",\"description\":\"desc\",\"tags\":[\"tag1\",\"tag2\"]}"_json;
    Record r = Record(o);
    REQUIRE(r.get_line() == "desc 345.67 tags:tag1,tag2 acct:cash");
}

TEST_CASE("get_line_no_tags", "[record]") {
    Record r = Record("345.67", "desc", {}, "cash");
    REQUIRE(r.get_line() == "desc 345.67 tags: acct:cash");
}

TEST_CASE("get_json_string", "[record]") {
    Record r = Record("345.67", "desc", {"tag1"}, "cash");
    REQUIRE(r.get_json_string() == "{\"acct\":\"cash\",\"amount\":\"345.67\",\"description\":\"desc\",\"tags\":[\"tag1\"]}");

    r = Record("345.67", "desc", {"tag1", "tag2"}, "cash");
    REQUIRE(r.get_json_string() == "{\"acct\":\"cash\",\"amount\":\"345.67\",\"description\":\"desc\",\"tags\":[\"tag1\",\"tag2\"]}");
}

TEST_CASE("is_empty", "[record]") {
    Record r{};
    REQUIRE(r.empty() == true);

    r = Record("", "", {}, "");
    REQUIRE(r.empty() == true);

    r = Record("", "", {""}, "");
    REQUIRE(r.empty() == true);

    r = Record("1", "", {}, "");
    REQUIRE(r.empty() == false);

    r = Record("", "d", {}, "");
    REQUIRE(r.empty() == false);

    r = Record("", "", {}, "act");
    REQUIRE(r.empty() == true);
    r = Record("", "", {}, "act");
    REQUIRE(r.empty(false) == false);
}
