#ifndef __Buxoff__ViewHelpers__
#define __Buxoff__ViewHelpers__

#include <string>

namespace Buxoff {
    bool view_enable_add(const std::string& amount, const std::string& account) {
        return !(amount.empty() || account.empty());
    }

    bool view_enable_push(int records_count) {
        return records_count > 0;
    }
};

#endif
