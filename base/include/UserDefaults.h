#ifndef __Buxoff__UserDefaults__
#define __Buxoff__UserDefaults__

#include "Storage.h"

namespace Buxoff {
    class UserDefaults: public StringStorage {
    public:
        UserDefaults(Connection *adb): StringStorage(adb, "user_defaults_") {};
    };
};

#endif
