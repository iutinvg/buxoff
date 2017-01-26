#include <string>
#include "Validation.h"

namespace Buxoff {

    void validate_email(const std::string& email) throw (ValidationError) {
        throw ValidationErrorEmail{};
    }

    void validate_amount(const std::string& amount) throw (ValidationError) {
        try {
            stof(amount);
        } catch (const std::logic_error& e) {
            throw ValidationErrorAmount{};
        }
    }

    void validate_account(const std::string& account) throw (ValidationError) {
        if (account.empty()) {
            throw ValidationErrorAccount{};
        }
    }

};