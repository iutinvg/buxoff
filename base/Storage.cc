// #include <iostream>

#include "json.hpp"

#include "Storage.h"

using namespace Buxoff;
using namespace std;

string Buxoff::random_key(size_t length) {
    auto randchar = []() -> char
    {
        constexpr char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        constexpr size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    string str(length, 0);
    generate_n(str.begin(), length, randchar);
    return str;
}

string Buxoff::random_key(const string& prefix, size_t length) {
    return prefix + random_key(length);
}

Connection::Connection(const std::string& filename) {
    srand(time(0));
    leveldb::Options options;
    options.create_if_missing = true;
    last_status = leveldb::DB::Open(options, filename, &db);
    assert(last_status.ok());
}

Connection::~Connection() {
    delete db;
}

string Connection::get(const string &key, const string &def) {
    string value{def};
    last_status = db->Get(leveldb::ReadOptions(), key, &value);
    return value;
}

void Connection::put(const string& key, const string& value) {
    last_status = db->Put(leveldb::WriteOptions(), key, value);
    assert(last_status.ok());
}

void Connection::remove(const string& key) {
    last_status = db->Delete(leveldb::WriteOptions(), key);
    assert(last_status.ok());
}


void StringStorage::validate_key_value (
        const string& key,
        const string& value) throw (StorageError) {
    if (key.empty() || value.empty())
        throw StorageError{"empty key or value"};
}

void StringStorage::put(
        const string& key,
        const string& value) throw (StorageError) {
    validate_key_value(key, value);
    db->put(key, value);
};

string StringStorage::put(const string& value) throw (StorageError) {
    // cout << "base put\n";
    string key{random_key(prefix)};
    put(key, value);
    return key;
}

std::vector<std::string> StringStorage::all() {
    vector<string> res;
    auto f = [&res](const string& key, const string& value) {
        res.push_back(value);
    };
    db->for_each(prefix, f);
    return res;
}

std::unordered_map<std::string, std::string> StringStorage::all_map(
        bool clear_key) {
    unordered_map<string, string> res;
    if (clear_key) {
        auto f = [&res, this](const string& key, const string& value) {
            res[key.substr(prefix.size())] = value;
        };
        db->for_each(prefix, f);
    } else {
        auto f = [&res](const string& key, const string& value) {
            res[key] = value;
        };
        db->for_each(prefix, f);
    }
    return res;
}

std::set<std::string> StringStorage::keys(bool clear_key) {
    std::set<string> res;
    if (clear_key) {
        auto f = [&res, this](const string& key, const string& value) {
            res.insert(key.substr(prefix.size()));
        };
        db->for_each(prefix, f);
    } else {
        auto f = [&res](const string& key, const string& value) {
            res.insert(key);
        };
        db->for_each(prefix, f);
    }
    return res;
}

int StringStorage::count() {
    auto i = 0;
    auto f = [&i](const string& key, const string& value) { ++i; };
    db->for_each(prefix, f);
    return i;
}

void StringStorage::clear() {
    auto f = [this](const string& key, const string& value) {
        db->remove(key);
    };
    db->for_each(prefix, f);
};
