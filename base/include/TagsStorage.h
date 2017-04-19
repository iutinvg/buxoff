#ifndef __Buxoff__TagsStorage__
#define __Buxoff__TagsStorage__

#include "Storage.h"

namespace Buxoff {
    class TagsStorage: public StringStorage {
    public:
        TagsStorage(Connection *adb): StringStorage(adb, "tag_") {};
        // std::set<std::string> all_tags();
        // value is also used as a key to make
        // every tags store once
        std::string put(const std::string& value) {
            std::string key{prefix + value};
            db->put(key, value);
            return key;
        }
    };
};

#endif
