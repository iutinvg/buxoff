#include <random>
#include <cassert>
#include "json.hpp"

#include "Storage.h"
#include "Record.h"

using namespace Buxoff;
using namespace std;

Storage::Storage(string filename) {
    srand(time(0));
    _options.create_if_missing = true;
    auto status = leveldb::DB::Open(_options, filename, &_db);
    assert(status.ok());
}

Storage::~Storage() {
    delete _db;
}

string Storage::get_string(const string &key) {
    std::string value;
    auto status = _db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());
    return value;
}

Record Storage::get(const string& key) {
    std::string value {get_string(key)};
    return Record(nlohmann::json::parse(value));
}

RecordsList Storage::get_records() {
    auto last = Record_ID_Prefix + "\u02ad";

    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    RecordsList res;
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

int Storage::get_total_count() {
    auto i = 0;
    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ++i;
    }
    return i;
}

string Storage::put(const Record& record) {
    return put(record, Record_ID_Prefix + Storage::random_key());
}

string Storage::put(const string& amount, const string& description, const Tags& tags, const string& account) {
    return put(Record(amount, description, tags, account));
}

string Storage::put(const Record& record, const string& key) {
    string value {record.get_json_string()};
    put(key, value);
    return key;
}

void Storage::put(const string &key, const string &value) {
    auto status = _db->Put(leveldb::WriteOptions(), key, value);
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
