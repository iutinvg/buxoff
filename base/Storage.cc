#include <random>
#include <cassert>
#include "json.hpp"

#include "Storage.h"
#include "Record.h"

using namespace Buxoff;
using namespace std;

Storage::Storage(string filename) {
    _options.create_if_missing = true;
    leveldb::DB *db;
    auto status = leveldb::DB::Open(_options, filename, &db);
    assert(status.ok());
    _db.reset(db);
}

string Storage::get_string(const string &key) {
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());
    return value;
}

Record Storage::get(const string& key) {
    std::string value = get_string(key);
    return Record(nlohmann::json::parse(value));
}

string Storage::put(const Record& record) {
    return put(record, Storage::random_key());
}

string Storage::put(const Record& record, const string& key) {
    string value = record.get_json_string();
    put(key, value);
    return key;
}

void Storage::put(const string &key, const string &value) {
    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, value);
    assert(status.ok());
}

string Storage::random_key(size_t length) {
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    string str(length, 0);
    generate_n(str.begin(), length, randchar);
    return str;
}
