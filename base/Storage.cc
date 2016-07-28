#include <random>
#include <cassert>

#include "Storage.h"
#include "Record.h"

using namespace Buxoff;
using namespace std;

Storage::Storage(string filename) {
    _options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(_options, filename, &_db);
    assert(status.ok());
}

Storage::~Storage() {
    delete _db;
}

Record Storage::get(const string& key) {
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());
    return Record(json::parse(value));
}

void Storage::test(const string& key) {
    cout << key;
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
    assert(status.ok());
    cout << value;
}

string Storage::put(const Record& record) {
    return put(record, Storage::random_key());
}

string Storage::put(const Record& record, const string& key) {
    string value = record.get_json_string();
    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, record.get_json_string());
    assert(status.ok());
    return key;
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
