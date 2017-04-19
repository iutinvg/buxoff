#ifndef __Buxoff__ViewHelpers__
#define __Buxoff__ViewHelpers__

#include <string>
#include "Validation.h"

namespace Buxoff {
    bool view_enable_add(const std::string& amount, const std::string& account) {
        return !(amount.empty() || account.empty());
    }

    bool view_enable_push(int records_count, const std::string& amount, const std::string& account, const std::string& email) {
        return is_valid_email(email) && (records_count > 0 || view_enable_add(amount, account));
    }

    std::string tag_for_description(const std::string& desc) {
        return "";
    }
};

#endif
