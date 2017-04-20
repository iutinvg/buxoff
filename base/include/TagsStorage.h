#ifndef __Buxoff__TagsStorage__
#define __Buxoff__TagsStorage__

#include "Storage.h"

namespace Buxoff {
    class TagsStorage: public StringStorage {
    public:
        TagsStorage(Connection *adb): StringStorage(adb, "tag_") {};
        using StringStorage::put;
        std::string put(const std::string& value);
    };
};

#endif
