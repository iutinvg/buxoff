#include <string>
#include <regex>

#include "Validation.h"

using namespace std;

namespace Buxoff {

    // not a real validation, just to catch "fat fingers" typos
    void validate_email(const string& email) throw (ValidationError) {
        regex email_pattern{R"(^\S+@\S+\.\S+$)"};
        if (regex_match(email, email_pattern)) {
            return;
        }
        throw ValidationErrorEmail{};
    }

    void validate_amount(const string& amount) throw (ValidationError) {
        try {
            stof(amount);
        } catch (const logic_error& e) {
            throw ValidationErrorAmount{};
        }
    }

    void validate_account(const string& account) throw (ValidationError) {
        if (account.empty()) {
            throw ValidationErrorAccount{};
        }
    }

};