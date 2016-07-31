#ifndef __Buxoff__Record__
#define __Buxoff__Record__

#include <string>
#include <set>
#include <vector>

#ifndef NLOHMANN_JSON_HPP
namespace nlohmann {
    class json;
}
#endif

using namespace std;


namespace Buxoff {
    typedef set<string> Tags;

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
        Record();
        string get_line() const;
        string get_json_string() const;
    };

    typedef vector<Record> RecordsList;
};

#endif
