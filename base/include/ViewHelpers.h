#ifndef __Buxoff__ViewHelpers__
#define __Buxoff__ViewHelpers__

#include <string>
#include <set>
#include "Record.h"
#include "Validation.h"
#include "RulesStorage.h"


namespace Buxoff {
    bool view_enable_add(const std::string& amount,
            const std::string& account) {

        return !(amount.empty() || account.empty());
    }

    bool view_enable_push(int records_count, const std::string& amount,
            const std::string& account, const std::string& email) {

        return is_valid_email(email) &&
            (records_count > 0 || view_enable_add(amount, account));
    }

    std::string tags_for_description(Connection* c, const std::string& desc) {

        auto rs = RulesStorage{c};
        auto tags = rs.get(desc);
        return join_strings(tags);
    }
};

#endif
