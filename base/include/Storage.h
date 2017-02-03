#ifndef __Buxoff__Storage__
#define __Buxoff__Storage__

#include <string>

#include <leveldb/db.h>
#include <leveldb/options.h>

#include "Record.h"


namespace Buxoff {
    const std::string Record_ID_Prefix{"tr_"};

    class Storage {
        leveldb::DB* _db;

    public:
        leveldb::Status last_status;
        Storage(std::string filename);
        ~Storage();

        // TODO: delete copy c-r and assignment?

        Record get(const std::string& key);
        std::string get_string(const std::string& key, const std::string& def="");
        RecordsList get_records();
        int get_records_count();
        int get_total_count();

        void put(const std::string& key, const std::string& value);
        void put(const std::string& key, const Record& record);
        std::string put(const Record& record);
        std::string put(const std::string& amount, const std::string& description, const Tags& tags, const std::string& account);

        // ud is User Defaults
        std::string get_ud(const std::string& key);
        void put_ud(const std::string& key, const std::string& val);

        void clear(const std::string& prefix="");
        void clear_records();
    };

    std::string random_key(size_t length=10);
};

#endif
