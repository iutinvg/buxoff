#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <cassert>

#include <leveldb/db.h>
#include <leveldb/options.h>

namespace Buxoff {
    constexpr char last_suffix[] = "\u02ad";
    std::string random_key(size_t length=10);

    class Connection {
        leveldb::DB* db;
    public:
        leveldb::Status last_status;
        Connection(const std::string& filename);
        ~Connection();

        std::string get(const std::string& key, const std::string& def={});
        void put(const std::string& key, const std::string& value);
        void remove(const std::string& key); // delete is a key word

        template<class Func>
        void for_each(const std::string& first, const std::string& last, Func f) {
            std::unique_ptr<leveldb::Iterator> it(db->NewIterator(leveldb::ReadOptions()));
            for (it->Seek(first); it->Valid(); it->Next()) {
                std::string key = it->key().ToString();
                if (key >= last) { break; }
                f(key, it->value().ToString());
            }
            assert(it->status().ok());
        }

        template<class Func>
        void for_each(const std::string& prefix, Func f) {
            for_each(prefix, prefix + last_suffix, f);
        }
    };

    class StringStorage {
        Connection* db;
        const std::string prefix;
    public:
        StringStorage(Connection *adb, const std::string& pref): db{adb}, prefix{pref} {};
        std::string get(const std::string& key, const std::string& def={}) { return db->get(key, def); };
        void put(const std::string& key, const std::string& value) { db->put(key, value); };
        std::string put(const std::string& value);

        std::vector<std::string> vector();
        std::unordered_map<std::string, std::string> map();
        int count();
        void clear();
    };

    class RecordStorage: public StringStorage {
    public:
        RecordStorage(Connection *adb): StringStorage(adb, "rec_") {};
    };

    class UserDefaults: public StringStorage {
    public:
        UserDefaults(Connection *adb): StringStorage(adb, "user_defaults_") {};
    };

    // const std::string Record_ID_Prefix{"tr_"};

    // class Storage {
    //     leveldb::DB* _db;

    // public:
    //     leveldb::Status last_status;
    //     Storage(std::string filename);
    //     ~Storage();

    //     // TODO: delete copy c-r and assignment?

    //     Record get(const std::string& key);
    //     std::string get_string(const std::string& key, const std::string& def="");
    //     RecordsList get_records();
    //     int get_records_count();
    //     int get_total_count();

    //     void put(const std::string& key, const std::string& value);
    //     void put(const std::string& key, const Record& record);
    //     std::string put(const Record& record);
    //     std::string put(const std::string& amount, const std::string& description, const Tags& tags, const std::string& account);

    //     // ud is User Defaults
    //     std::string get_ud(const std::string& key);
    //     void put_ud(const std::string& key, const std::string& val);

    //     void clear(const std::string& prefix="");
    //     void clear_records();
    // };

    // std::string random_key(size_t length=10);
};

#endif
