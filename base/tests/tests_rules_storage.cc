#include <set>
#include "catch.hpp"
#include "RulesStorage.h"
#include "Record.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("rules-get", "[rules-storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto rs = RulesStorage(&c);
    rs.put("desc", set<string>{"tag2", "tag1"});
    auto tags = rs.get("desc");
    vector<string> v(tags.begin(), tags.end());
    REQUIRE(tags.size() == 2);
    REQUIRE(v[0] == "tag1");
    REQUIRE(v[1] == "tag2");
}

TEST_CASE("rules-put", "[rules-storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto rs = RulesStorage(&c);
    rs.put("desc", set<string>{"tag2"});
    REQUIRE(rs.count() == 1);
}

TEST_CASE("rules-put-empty", "[rules-storage]") {
    auto ss = RulesStorage(nullptr);
    REQUIRE_THROWS_AS(ss.put("", set<string>{}), StorageError);
    REQUIRE_THROWS_AS(ss.put("foo", set<string>{}), StorageError);
    REQUIRE_THROWS_AS(ss.put("", set<string>{"foo"}), StorageError);
}
