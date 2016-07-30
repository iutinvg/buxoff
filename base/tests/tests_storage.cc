#include <set>
#include "catch.hpp"
#include "Storage.h"
#include "Record.h"

using namespace Buxoff;

TEST_CASE("C-r", "[storage]") {
    Storage s = Storage("test.db");
    REQUIRE(true);
}

TEST_CASE("get-put", "[storage]") {
    Storage s = Storage("test.db");
    Record r1 = Record("1", "d", {"tag1"}, "c");
    string key = s.put(r1);
    Record r2 = s.get(key);

    REQUIRE(r1.get_line() == r2.get_line());
}

// TEST_CASE("test", "[storage]") {
//     Storage s = Storage("test.db");
//     Record r1 = Record("1", "d", {"tag1"}, "c");
//     string key = s.put(r1);
//     s.test(key);
// }

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
