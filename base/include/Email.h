#ifndef __Buxoff__Email__
#define __Buxoff__Email__

#include <vector>
#include <string>

#include "Record.h"

namespace Buxoff {
    class Email
    {
        RecordsList records;
    public:
        Email(const RecordsList& records);
        std::string body();

        static std::string subject();
    };
};

#endif
