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
        Email(RecordsList records);
        static std::string subject();
        std::string body();
    };
};

#endif