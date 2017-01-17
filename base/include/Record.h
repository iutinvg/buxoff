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

namespace Buxoff {
    typedef std::set<std::string> Tags;

    const std::string Record_ID_Prefix{"tr_"};

    class Record {
        std::string _amount;
        std::string _description;
        Tags _tags;
        std::string _account;

        std::string _join_tags(const Tags &tags) const;
    public:
        Record(std::string amount, std::string description, Tags tags, std::string account);
        Record(nlohmann::json o);
        Record();
        std::string get_line() const;
        std::string get_json_string() const;
    };

    typedef std::vector<Record> RecordsList;
};

#endif
