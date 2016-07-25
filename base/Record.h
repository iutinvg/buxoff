#ifndef __Buxoff__Record__
#define __Buxoff__Record__

#include <string>
#include <set>

#include "deps/json.hpp"


using namespace std;
using json = nlohmann::json;

typedef set<string> Tags;

namespace Buxoff {
    class Record {
        string _amount;
        string _description;
        Tags _tags;
        string _account;

        string _join_tags(const Tags &tags) const;
    public:
        Record(string amount, string description, Tags tags, string account);
        Record(json o);
        string get_line() const;
        string get_json_string() const;
    };
};

#endif
