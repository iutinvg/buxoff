#include <numeric>
#include <stdexcept>

#include "Record.h"
#include "Validation.h"

using namespace Buxoff;
using namespace std;

Record::Record(const std::string& json_str):
    Record(nlohmann::json::parse(json_str)) {
}

Record::Record(nlohmann::json o):
    _amount{o["amount"].get<string>()},
    _description{o["description"].get<string>()},
    _tags{o["tags"].get<Tags>()},
    _account{o["acct"].get<string>()}
{
        _tags.erase("");
}

Record::Record(string amount, string description, Tags tags, string account):
    _amount{amount},
    _description{description},
    _tags{tags},
    _account{account} {
        _tags.erase("");
}

Tags Record::tags() const {
    return _tags;
}

string Record::get_line() const {
    string tags{_join_tags(_tags)};
    return _description + " " + _amount + " tags:" + tags + " acct:" + _account;
}

string Record::_join_tags(const Tags &tags) const {
    return join_strings(tags);
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

vector<Record> RecordStorage::all() {
    vector<Record> res;
    auto f = [&res](const string& key, const string& value) {
        res.push_back(value);
    };
    db->for_each(prefix, f);
    return res;
}

set<string> RecordStorage::all_tags() {
    set<string> tags_set;
    for (const Record &r: all()) {
        tags_set.insert(r.tags().begin(), r.tags().end());
        // for (const string &t: r.tags()) {
        //     tags_set.insert(t);
        // }
    }
    return tags_set;
}
