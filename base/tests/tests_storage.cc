#include <set>
#include "catch.hpp"
#include "Storage.h"
#include "Record.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("C-r", "[storage]") {
    auto s = Storage("test.db");
    REQUIRE(true);
}

TEST_CASE("get-put", "[storage]") {
    auto s = get_clean_storage();
    auto r1 = Record("1", "d", {"tag1"}, "c");
    auto key = s.put(r1);
    auto r2 = s.get(key);

    REQUIRE(r1.get_line() == r2.get_line());
}

TEST_CASE("get-put-record-props", "[storage]") {
    auto s = get_clean_storage();
    string amount{"1"};
    string desc{"d"};
    Tags tags{"tag1"};
    string acct{"c"};
    auto key = s.put(amount, desc, tags, acct);
    auto r1 = s.get(key);
    auto r2 = Record{amount, desc, tags, acct};
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

TEST_CASE("clear_records", "[storage]") {
    auto s = get_clean_storage();

    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put("key", "value");
    REQUIRE(s.get_records_count() == 2);
    REQUIRE(s.get_total_count() == 3);

    s.clear_records();
    REQUIRE(s.get_records_count() == 0);
    REQUIRE(s.get_total_count() == 1);
}

TEST_CASE("clear all", "[storage]") {
    auto s = get_clean_storage();

    s.put(Record("1", "d", {"tag1"}, "c"));
    s.put("key", "value");
    REQUIRE(s.get_records_count() == 1);
    REQUIRE(s.get_total_count() == 2);

    s.clear();
    REQUIRE(s.get_total_count() == 0);
}

TEST_CASE("random_key", "[storage]") {
    set<string> c;
    auto num = 10000;

    string s;
    for (auto i = 0; i < num; ++i) {
        s = random_key(10);
        c.insert(s);
    }

    REQUIRE(c.size() == num);
}

TEST_CASE("random_key_size", "[storage]") {
    auto s1 = random_key(20);
    REQUIRE(s1.size() == 20);
}
