#ifndef __Buxoff__RulesStorage__
#define __Buxoff__RulesStorage__

#include <set>
#include "Storage.h"

namespace Buxoff {
    class RulesStorage: public StringStorage {
    public:
        RulesStorage(Connection *adb): StringStorage(adb, "rule_") {};

        using StringStorage::put;
        void put(const std::string& desc, const std::set<std::string>& tags)
            throw (StorageError);
        std::set<std::string> get(const std::string& desc);
    };
};

#endif
