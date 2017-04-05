#ifndef __Buxoff__Email__
#define __Buxoff__Email__

#include <vector>
#include <string>

#include "Record.h"

namespace Buxoff {
    class Email
    {
        std::vector<Record> records;
    public:
        Email(const std::vector<Record>& records);
        std::string body();

        static std::string subject();
    };
};

#endif
