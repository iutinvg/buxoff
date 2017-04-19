#ifndef __Buxoff__RulesStorage__
#define __Buxoff__RulesStorage__

#include "Storage.h"

namespace Buxoff {
    class RulesStorage: public StringStorage {
    public:
        RulesStorage(Connection *adb): StringStorage(adb, "rule_") {};
    };
};

#endif
