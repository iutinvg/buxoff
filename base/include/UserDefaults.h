#ifndef __Buxoff__UserDefaults__
#define __Buxoff__UserDefaults__

#include "Storage.h"

namespace Buxoff {
    class UserDefaults: public StringStorage {
    protected:
        void validate_key_value(
                const std::string& key,
                const std::string& value) throw (StorageError) override {
            // allow empty string storage
        };
    public:
        UserDefaults(Connection *adb): StringStorage(adb, "user_defaults_") {};
    };
};

#endif
