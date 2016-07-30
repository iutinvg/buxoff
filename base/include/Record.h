#ifndef __Buxoff__Record__
#define __Buxoff__Record__

#include <string>
#include <set>

#ifndef NLOHMANN_JSON_HPP
namespace nlohmann {
    class json;
}
#endif

using namespace std;

typedef set<string> Tags;

namespace Buxoff {
    const string Record_ID_Prefix = "tr_";
    class Record {
        string _amount;
        string _description;
        Tags _tags;
        string _account;

        string _join_tags(const Tags &tags) const;
    public:
        Record(string amount, string description, Tags tags, string account);
        Record(nlohmann::json o);
        string get_line() const;
        string get_json_string() const;
    };
};

#endif
