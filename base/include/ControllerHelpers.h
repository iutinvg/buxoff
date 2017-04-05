#ifndef __Buxoff__ControllerHelpers__
#define __Buxoff__ControllerHelpers__

#include <string>

#include "Storage.h"

namespace Buxoff {
    class Storage;
    class Record;
    void controller_add(Connection*, const Record&);
    std::string controller_push(Connection*, const Record&);
};

#endif
