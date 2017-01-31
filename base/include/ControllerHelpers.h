#ifndef __Buxoff__ControllerHelpers__
#define __Buxoff__ControllerHelpers__

#include <string>

namespace Buxoff {
    class Storage;
    class Record;
    void controller_add(Storage&, const Record&);
    std::string controller_push(Storage&, const Record&);
};

#endif
