#ifndef __Buxoff__Validation__
#define __Buxoff__Validation__

#include <stdexcept>


namespace Buxoff {
    // class C {
    // public:
    //     C();
    // };

    // int f(int);
    class ValidationError: public std::logic_error{
    public:
        using logic_error::logic_error;
    };

    class ValidationErrorEmail: public ValidationError {
    public:
        using ValidationError::ValidationError;
        ValidationErrorEmail(): ValidationErrorEmail{"Invalid email"} {}
    };

    class ValidationErrorAmount: public ValidationError {
    public:
        using ValidationError::ValidationError;
        ValidationErrorAmount(): ValidationErrorAmount{"Amount must be a number"} {}
    };

    class ValidationErrorAccount: public ValidationError {
    public:
        using ValidationError::ValidationError;
        ValidationErrorAccount(): ValidationErrorAccount{"Account must be set"} {}
    };

    void validate_email(const std::string& email) throw (ValidationError);
    bool is_valid_email(const std::string& email);
    void validate_amount(const std::string& amount) throw (ValidationError);
    void validate_account(const std::string& account) throw (ValidationError);
};

#endif
