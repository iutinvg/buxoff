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

RecordsList Storage::get_records() {
    RecordsList res;
    auto last = Record_ID_Prefix + "\u02ad";

    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->Seek(Record_ID_Prefix);
        it->Valid() && it->key().ToString() < last;
        it->Next()) {
        res.push_back(Record(nlohmann::json::parse(it->value().ToString())));
    }
    assert(it->status().ok());
    return res;
}

int Storage::get_records_count() {
    auto i = 0;
    auto last = Record_ID_Prefix + "\u02ad";

    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->Seek(Record_ID_Prefix);
        it->Valid() && it->key().ToString() < last;
        it->Next()) {
        ++i;
    }
    assert(it->status().ok());
    return i;
}

string Storage::put(const Record& record) {
    return put(record, Record_ID_Prefix + Storage::random_key());
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

void Storage::__clear() {
    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        auto s = _db->Delete(leveldb::WriteOptions(), it->key());
        assert(s.ok());
    }
    assert(it->status().ok());
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
