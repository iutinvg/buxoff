#include <set>
#include "catch.hpp"
#include "TagsStorage.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("put", "[tags-storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = TagsStorage(&c);
    auto val = random_key();
    // put the same tag twice
    ss.put(val);
    ss.put(val);
    // but it's stored once
    auto m = ss.all_map(true);
    REQUIRE(m.size() == 1);
    REQUIRE(m[val] == val);
}

TEST_CASE("put-via-put-all", "[tags-storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto val = random_key();
    auto ss = TagsStorage(&c);
    // put the same tag twice
    ss.put_all(vector<string>{val, val});
    // but it's stored once
    auto m = ss.all_map(true);
    REQUIRE(m.size() == 1);
    REQUIRE(m[val] == val);
}
