#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <random>
#include <cassert>

#include <leveldb/db.h>
#include <leveldb/options.h>

namespace Buxoff {
    constexpr char last_suffix[] = "\u02ad";
    std::string random_key(size_t length=10);
    std::string random_key(const std::string& prefix, size_t length=10);

    class Connection {
        leveldb::DB* db;
    public:
        leveldb::Status last_status;
        Connection(const std::string& filename);
        ~Connection();

        std::string get(const std::string& key, const std::string& def={});
        void put(const std::string& key, const std::string& value);
        void remove(const std::string& key); // delete is a key word

        template<typename Func>
        void for_each(const std::string& first, const std::string& last, Func f) {
            std::unique_ptr<leveldb::Iterator> it(db->NewIterator(leveldb::ReadOptions()));
            for (it->Seek(first); it->Valid(); it->Next()) {
                std::string key = it->key().ToString();
                if (key >= last) { break; }
                f(key, it->value().ToString());
            }
            assert(it->status().ok());
        }

        template<typename Func>
        void for_each(const std::string& prefix, Func f) {
            for_each(prefix, prefix + last_suffix, f);
        }
    };

    class StringStorage {
    protected:
        Connection* db;
    public:
        const std::string prefix;
        StringStorage(Connection *adb, const std::string& pref): db{adb}, prefix{pref} {};
        std::string get(const std::string& key, const std::string& def={}) { return db->get(key, def); };
        void put(const std::string& key, const std::string& value) { db->put(key, value); };
        std::string put(const std::string& value);
        // TODO: create a template with name put
        template<typename C>
        void put_all(const C &c) {
            for (const std::string &s: c) {
                put(s);
            }
        }

        // all items as a vector
        std::vector<std::string> all();
        // all items as a map {key->value}
        // if clear_key is true the prefix will be removed from the key
        std::unordered_map<std::string, std::string> all_map(bool clear_key=false);
        int count();
        void clear();
    };
};

#endif
