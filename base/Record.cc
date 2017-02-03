#include <numeric>
#include <stdexcept>

#include "Record.h"
#include "Validation.h"

using namespace Buxoff;
using namespace std;

Record::Record(string amount, string description, Tags tags, string account):
    _amount{amount},
    _description{description},
    _tags{tags},
    _account{account} {
        _tags.erase("");
}

Record::Record(nlohmann::json o):
    _amount{o["amount"].get<string>()},
    _description{o["description"].get<string>()},
    _tags{o["tags"].get<Tags>()},
    _account{o["acct"].get<string>()}
{
        _tags.erase("");
}

string Record::get_line() const {
    string tags{_join_tags(_tags)};
    return _description + " " + _amount + " tags:" + tags + " acct:" + _account;
}

string Record::_join_tags(const Tags &tags) const {
    // return accumulate(tags.begin(), tags.end(), string{},
    //     [](const string& a, const string& b) {
    //         return a.empty() ? b : a + "," + b;
    //     });
    if (tags.empty()) {
        return "";
    }

    string s;
    s.reserve(tags.size() * 10);

    auto final = tags.end();
    --final;

    for (auto it = tags.begin(); it != tags.end(); ++it) {
        s += *it;
        if (it != final) {
            s += ",";
        }
    }

    return s;
}

string Record::get_json_string() const {
    nlohmann::json j;
    j["amount"] = _amount;
    j["description"] = _description;
    j["tags"] = _tags;
    j["acct"] = _account;
    return j.dump();
}

void Record::validate() const {
    validate_amount(_amount);
    validate_account(_account);
}

// account is prefilled in UI, so we ignore it if other fields are empty
bool Record::empty(bool ignore_account) const {
    return _amount.empty() && _description.empty() && _tags.empty() &&
        (ignore_account || _account.empty());
}
