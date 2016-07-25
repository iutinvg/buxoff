#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>

#include <leveldb/db.h>
#include <leveldb/options.h>


using namespace std;

namespace Buxoff {
    class Record;

    class Storage {
        leveldb::DB *_db;
        leveldb::Options _options;
    public:
        Storage(string filename);
        ~Storage();

        Record get(const string& key);
        string put(const Record& record);
        string put(const Record& record, const string& key);
        void test(const string& key);

        static string random_key(size_t length=10);
    };
};

#endif