#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>

#include <leveldb/db.h>
#include <leveldb/options.h>

#include "Record.h"


namespace Buxoff {
    class Storage {
        leveldb::DB* _db;

    public:
        leveldb::Status last_status;
        Storage(std::string filename);
        ~Storage();

        // TODO: delete copy c-r and assignment?

        Record get(const std::string& key);
        std::string get_string(const std::string &key);
        RecordsList get_records();
        int get_records_count();
        int get_total_count();

        std::string put(const Record& record);
        std::string put(const Record& record, const std::string& key);
        std::string put(const std::string& amount, const std::string& description, const Tags& tags, const std::string& account);
        void put(const std::string &key, const std::string &value);

        void __clear();

        static std::string random_key(size_t length=10);
    };
};

#endif
