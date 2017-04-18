#include <set>
#include <algorithm>

#include "catch.hpp"
#include "Storage.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("storage-put-contaimer", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    auto ss = StringStorage(&c, "test_");

    vector<string> v{"a", "b"};
    set<string> s{"c", "d"};
    ss.put_all(v);
    ss.put_all(s);

    auto all = ss.all();
    sort(all.begin(), all.end());
    REQUIRE(all[0] == "a");
    REQUIRE(all[1] == "b");
    REQUIRE(all[2] == "c");
    REQUIRE(all[3] == "d");
}

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

TEST_CASE("storage-all_map_clear_key", "[storage]") {
    clean_storage();
    auto c = Connection("test.db");
    string prefix{"test_"};
    auto ss = StringStorage(&c, prefix);

    std::vector<string> keys;
    std::vector<string> values;
    for (int i = 0; i < 10; ++i) {
        auto k = random_key();
        auto v = random_key();
        ss.put(ss.prefix + k, v);
        keys.push_back(k);
        values.push_back(v);
    }
    auto res = ss.all_map(true);

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
