#include <random>
#include <cassert>
#include "json.hpp"

#include "Storage.h"
#include "Record.h"

using namespace Buxoff;
using namespace std;

Storage::Storage(string filename) {
    srand(time(0));
    leveldb::Options options;
    options.create_if_missing = true;
    last_status = leveldb::DB::Open(options, filename, &_db);
    assert(last_status.ok());
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
    last_status = it->status();
    assert(it->status().ok());
    return i;
}

int Storage::get_total_count() {
    auto i = 0;
    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ++i;
    }
    last_status = it->status();
    assert(it->status().ok());
    return i;
}

void Storage::put(const string& key, const string& value) {
    last_status = _db->Put(leveldb::WriteOptions(), key, value);
    assert(last_status.ok());
}

void Storage::put(const string& key, const Record& record) {
    put(key, record.get_json_string());
}

string Storage::put(const Record& record) {
    string key{Record_ID_Prefix + random_key()};
    put(key, record);
    return key;
}

string Storage::put(const string& amount, const string& description, const Tags& tags, const string& account) {
    return put(Record{amount, description, tags, account});
}

void Storage::clear_records() {
    clear(Record_ID_Prefix);
}

void Storage::clear(const string& prefix) {
    auto last = prefix + "\u02ad";
    unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
    for (it->Seek(prefix); it->Valid() && it->key().ToString() < last; it->Next()) {
        auto s = _db->Delete(leveldb::WriteOptions(), it->key());
        assert(s.ok());
    }
    assert(it->status().ok());
}

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
