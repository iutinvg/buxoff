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
    std::string value{def};
    last_status = db->Get(leveldb::ReadOptions(), key, &value);
    return value;
}

void Connection::put(const string& key, const string& value) {
    last_status = db->Put(leveldb::WriteOptions(), key, value);
    assert(last_status.ok());
}

void Connection::remove(const std::string& key) {
    last_status = db->Delete(leveldb::WriteOptions(), key);
    assert(last_status.ok());
}


string StringStorage::put(const string& value) {
    string key{prefix + random_key()};
    db->put(key, value);
    return key;
}

std::vector<std::string> StringStorage::vector() {
    std::vector<std::string> res;
    auto f = [&res](const string& key, const string& value) { res.push_back(value); };
    db->for_each(prefix, f);
    return res;
}

std::unordered_map<std::string, std::string> StringStorage::map() {
    std::unordered_map<std::string, std::string> res;
    auto f = [&res](const string& key, const string& value) { res[key] = value; };
    db->for_each(prefix, f);
    return res;
}

int StringStorage::count() {
    auto i = 0;
    auto f = [&i](const string& key, const string& value) { ++i; };
    db->for_each(prefix, f);
    return i;
}

void StringStorage::clear() {
    auto f = [this](const string& key, const string& value) { db->remove(key); };
    db->for_each(prefix, f);
};

// Storage::Storage(string filename) {
//     srand(time(0));
//     leveldb::Options options;
//     options.create_if_missing = true;
//     last_status = leveldb::DB::Open(options, filename, &_db);
//     assert(last_status.ok());
// }

// Storage::~Storage() {
//     delete _db;
// }

// string Storage::get_string(const string &key, const string &def) {
//     std::string value{def};
//     _db->Get(leveldb::ReadOptions(), key, &value);
//     return value;
// }

// Record Storage::get(const string& key) {
//     std::string value {get_string(key)};
//     return Record(nlohmann::json::parse(value));
// }

// RecordsList Storage::get_records() {
//     auto last = Record_ID_Prefix + "\u02ad";

//     unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
//     RecordsList res;
//     for (it->Seek(Record_ID_Prefix);
//         it->Valid() && it->key().ToString() < last;
//         it->Next()) {
//         res.push_back(Record(nlohmann::json::parse(it->value().ToString())));
//     }
//     assert(it->status().ok());
//     return res;
// }

// int Storage::get_records_count() {
//     auto i = 0;
//     auto last = Record_ID_Prefix + "\u02ad";

//     unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
//     for (it->Seek(Record_ID_Prefix);
//         it->Valid() && it->key().ToString() < last;
//         it->Next()) {
//         ++i;
//     }
//     last_status = it->status();
//     assert(it->status().ok());
//     return i;
// }

// int Storage::get_total_count() {
//     auto i = 0;
//     unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
//     for (it->SeekToFirst(); it->Valid(); it->Next()) {
//         ++i;
//     }
//     last_status = it->status();
//     assert(it->status().ok());
//     return i;
// }

// void Storage::put(const string& key, const string& value) {
//     last_status = _db->Put(leveldb::WriteOptions(), key, value);
//     assert(last_status.ok());
// }

// void Storage::put(const string& key, const Record& record) {
//     put(key, record.get_json_string());
// }

// string Storage::put(const Record& record) {
//     string key{Record_ID_Prefix + random_key()};
//     put(key, record);
//     return key;
// }

// string Storage::put(const string& amount, const string& description, const Tags& tags, const string& account) {
//     return put(Record{amount, description, tags, account});
// }

// void Storage::clear_records() {
//     clear(Record_ID_Prefix);
// }

// void Storage::clear(const string& prefix) {
//     auto last = prefix + "\u02ad";
//     unique_ptr<leveldb::Iterator> it(_db->NewIterator(leveldb::ReadOptions()));
//     for (it->Seek(prefix); it->Valid() && it->key().ToString() < last; it->Next()) {
//         auto s = _db->Delete(leveldb::WriteOptions(), it->key());
//         assert(s.ok());
//     }
//     assert(it->status().ok());
// }


// std::string Storage::get_ud(const std::string& key)
// {
//     auto o = nlohmann::json::parse(get_string(User_ID_Prefix, "{}"));
//     return o[key];
// }


// void Storage::put_ud(const std::string& key, const std::string& val)
// {
//     auto o = nlohmann::json::parse(get_string(User_ID_Prefix, "{}"));
//     o[key]
//     put(User_ID_Prefix + key, val);
// }


// string Buxoff::random_key(size_t length) {
//     auto randchar = []() -> char
//     {
//         constexpr char charset[] =
//         "0123456789"
//         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//         "abcdefghijklmnopqrstuvwxyz";
//         constexpr size_t max_index = (sizeof(charset) - 1);
//         return charset[ rand() % max_index ];
//     };
//     string str(length, 0);
//     generate_n(str.begin(), length, randchar);
//     return str;
// }
