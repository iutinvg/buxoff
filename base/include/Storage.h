#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>

#include <leveldb/db.h>
#include <leveldb/options.h>

#include "Record.h"


using namespace std;

namespace Buxoff {
    class Storage {
        unique_ptr<leveldb::DB> _db;
        leveldb::Options _options;
    public:
        Storage(string filename);

        Record get(const string& key);
        string get_string(const string &key);
        RecordsList get_records();
        int get_records_count();

        string put(const Record& record);
        string put(const Record& record, const string& key);
        void put(const string &key, const string &value);

        void __clear();

        static string random_key(size_t length=10);
    };
};

#endif