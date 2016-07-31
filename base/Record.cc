#include "json.hpp"
#include "Record.h"

using namespace Buxoff;

Record::Record():
    _amount(""),
    _description(""),
    _tags({}),
    _account("") {
}

Record::Record(string amount, string description, Tags tags, string account):
    _amount(amount),
    _description(description),
    _tags(tags),
    _account(account) {
}

Record::Record(nlohmann::json o):
    _amount(o["amount"].get<string>()),
    _description(o["description"].get<string>()),
    _tags(o["tags"].get<Tags>()),
    _account(o["acct"].get<string>())
{
}

string Record::get_line() const {
    string tags = _join_tags(_tags);
    return _description + " " + _amount + " tags:" + tags + " acct:" + _account;
}

string Record::_join_tags(const Tags &tags) const {
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
