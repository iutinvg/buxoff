#include "Email.h"

using namespace Buxoff;

Email::Email(const RecordsList& records):
    records{records} {
}

std::string Email::subject() {
    return "expense";
}

std::string Email::body() {
    if (records.empty()) {
        return "";
    }

    std::string s;
    s.reserve(records.size() * 70);

    for (auto it = records.begin(); it != records.end(); ++it) {
        s += it->get_line() + "\n";
    }

    return s;
}
