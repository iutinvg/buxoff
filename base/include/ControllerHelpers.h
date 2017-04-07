#ifndef __Buxoff__ControllerHelpers__
#define __Buxoff__ControllerHelpers__

#include <string>

#include "Storage.h"

namespace Buxoff {
    class Storage;
    class Record;
    void controller_add(Connection* c, const Record& record);
    std::string controller_push(Connection* c, const Record& record);
};

#endif
