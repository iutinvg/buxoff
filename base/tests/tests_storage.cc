#include <set>
#include <algorithm>

#include "catch.hpp"
#include "Storage.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("storage-get-put", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    auto val = random_key();

    auto key = ss.put(val);
    auto val2 = ss.get(key);

    REQUIRE(val == val2);
}

TEST_CASE("storage-get-put-with-key", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    auto key = random_key();
    auto val = random_key();

    ss.put(key, val);
    auto val2 = ss.get(key);

    REQUIRE(val == val2);
}

TEST_CASE("storage-get-default", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    auto key = random_key();
    auto def = random_key();

    auto val = ss.get(key, def);

    REQUIRE(val == def);
}

TEST_CASE("storage-all", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    std::vector<string> v1;
    for (int i = 0; i < 10; ++i) {
        auto v = random_key();
        ss.put(v);
        v1.push_back(v);
    }
    auto v2 = ss.all();

    REQUIRE(v1.size() == v2.size());
    for (auto v : v2) {
        REQUIRE(any_of(v1.begin(), v1.end(), [&v](string& s) { return s == v; }));
    }
}

TEST_CASE("storage-all_map", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    std::vector<string> keys;
    std::vector<string> values;
    for (int i = 0; i < 10; ++i) {
        auto k = random_key(ss.prefix);
        auto v = random_key();
        ss.put(k, v);
        keys.push_back(k);
        values.push_back(v);
    }
    auto res = ss.all_map();

    REQUIRE(res.size() == keys.size());
    for (int i = 0; i < keys.size(); ++i) {
        REQUIRE(res[keys[i]] == values[i]);
    }
}

TEST_CASE("storage-count", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    for (int i = 0; i < 10; ++i) {
        ss.put(random_key());
    }

    REQUIRE(ss.count() == 10);
}

TEST_CASE("storage-clear", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    for (int i = 0; i < 10; ++i) {
        ss.put(random_key());
    }

    REQUIRE(ss.count() == 10);
    ss.clear();
    REQUIRE(ss.count() == 0);
}
