#include <set>
#include "catch.hpp"
#include "Storage.h"
#include "Record.h"

#include "utils.hpp"

using namespace Buxoff;

TEST_CASE("C-r", "[storage]") {
    Storage s = Storage("test.db");
    REQUIRE(true);
}

TEST_CASE("get-put", "[storage]") {
    auto s = get_clean_storage();
    Record r1 = Record("1", "d", {"tag1"}, "c");
    string key = s.put(r1);
    Record r2 = s.get(key);

    REQUIRE(r1.get_line() == r2.get_line());
}

TEST_CASE("get_records", "[storage]") {
    auto s = get_clean_storage();

    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put(Record("1", "d", {"tag1"}, "c"));

    auto l = s.get_records();
    REQUIRE(l.size() == 3);
}

TEST_CASE("get_records_except_others", "[storage]") {
    auto s = get_clean_storage();

    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put("key", "value");

    REQUIRE(s.get_records().size() == 1);
}

TEST_CASE("get_records_count", "[storage]") {
    auto s = get_clean_storage();

    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put(Record("1", "d", {"tag1"}, "c"));

    REQUIRE(s.get_records_count() == 2);
}

TEST_CASE("random_key", "[storage]") {
    set<string> c;
    auto num = 10000;

    for (auto i = 0; i < num; ++i) {
        string s = Storage::random_key(10);
        c.insert(s);
    }

    REQUIRE(c.size() == num);
}

TEST_CASE("random_key_size", "[storage]") {
    string s1 = Storage::random_key(20);
    REQUIRE(s1.size() == 20);
}
