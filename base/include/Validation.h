#ifndef __Buxoff__Validation__
#define __Buxoff__Validation__

#include <stdexcept>


namespace Buxoff {
    class ValidationError: public std::logic_error{
    public:
        using logic_error::logic_error;
    };

    class ValidationErrorEmail: public ValidationError {
    public:
        ValidationErrorEmail(): ValidationError{"Invalid email"} {}
    };

    class ValidationErrorAmount: public ValidationError {
    public:
        ValidationErrorAmount():ValidationError{"Amount must be a number"} {}
    };

    class ValidationErrorAccount: public ValidationError {
    public:
        ValidationErrorAccount(): ValidationError{"Account must be set"} {}
    };

    void validate_email(const std::string& email) throw (ValidationError);
    bool is_valid_email(const std::string& email);
    void validate_amount(const std::string& amount) throw (ValidationError);
    void validate_account(const std::string& account) throw (ValidationError);
};

#endif
