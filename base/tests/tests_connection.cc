#define CATCH_CONFIG_MAIN

#include <set>
#include <iostream>
#include <algorithm>

#include "catch.hpp"
#include "Storage.h"
#include "Record.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;


TEST_CASE("C-r", "[connection]") {
    auto c = Connection("test.db");
    REQUIRE(true);
}

TEST_CASE("random_key", "[connection]") {
    set<string> keys;
    auto c = Connection("test.db");
    auto num = 10000;

    string s;
    for (auto i = 0; i < num; ++i) {
        s = c.random_key(10);
        keys.insert(s);
    }

    REQUIRE(keys.size() == num);
}

TEST_CASE("random_key_size", "[connection]") {
    auto s1 = Connection::random_key(20);
    REQUIRE(s1.size() == 20);
}

TEST_CASE("get-put", "[connection]") {
    clean_storage();
    auto c = Connection("test.db");

    string key{c.random_key()};
    string value{c.random_key()};

    c.put(key, value);
    auto res = c.get(key);

    REQUIRE(res == value);
}

TEST_CASE("remove", "[connection]") {
    clean_storage();
    auto c = Connection("test.db");

    string key{c.random_key()};
    string value{c.random_key()};
    string def{c.random_key()};

    c.put(key, value);
    c.remove(key);
    auto res = c.get(key, def);
    REQUIRE(res == def);
}

TEST_CASE("for_each", "[connection]") {
    clean_storage();
    auto c = Connection("test.db");
    auto keys = std::vector<string>{};
    auto vals = std::vector<string>{};

    string prefix{"test_"};

    for (int i = 0; i < 10; ++i) {
        auto k = prefix + c.random_key();
        auto v = prefix + c.random_key();
        keys.push_back(k);
        vals.push_back(v);
        c.put(k, v);
    }
    sort(keys.begin(), keys.end());
    sort(vals.begin(), vals.end());

    auto keys2 = std::vector<string>{};
    auto vals2 = std::vector<string>{};
    auto l = [&keys2, &vals2](const string& key, const string& value) {
        keys2.push_back(key);
        vals2.push_back(value);
    };

    c.for_each(prefix, prefix + Connection::last_prefix(), l);
    sort(keys2.begin(), keys2.end());
    sort(vals2.begin(), vals2.end());

    REQUIRE(keys == keys2);
    REQUIRE(vals == vals2);
}

TEST_CASE("for_each_prefix", "[connection]") {
    clean_storage();
    auto c = Connection("test.db");
    auto keys = std::vector<string>{};
    auto vals = std::vector<string>{};

    string prefix{"test_"};

    for (int i = 0; i < 10; ++i) {
        auto k = prefix + c.random_key();
        auto v = prefix + c.random_key();
        keys.push_back(k);
        vals.push_back(v);
        c.put(k, v);
    }
    sort(keys.begin(), keys.end());
    sort(vals.begin(), vals.end());

    auto keys2 = std::vector<string>{};
    auto vals2 = std::vector<string>{};
    auto l = [&keys2, &vals2](const string& key, const string& value) {
        keys2.push_back(key);
        vals2.push_back(value);
    };

    c.for_each(prefix, l);
    sort(keys2.begin(), keys2.end());
    sort(vals2.begin(), vals2.end());

    REQUIRE(keys == keys2);
    REQUIRE(vals == vals2);
}
