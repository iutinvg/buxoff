#ifndef __Buxoff__Record__
#define __Buxoff__Record__

#include <string>
#include <set>
#include <vector>

#include "json.hpp"

namespace Buxoff {
    typedef std::set<std::string> Tags;

    class Record {
        std::string _amount;
        std::string _description;
        Tags _tags;
        std::string _account;

        std::string _join_tags(const Tags &tags) const;
    public:
        Record(){}; // mostly needed for testing
        Record(std::string amount, std::string description, Tags tags, std::string account);
        Record(nlohmann::json o);
        Record(const std::string& json_str);

        std::string get_line() const;
        std::string get_json_string() const;
        void validate() const;
        bool empty(bool ignore_account=true) const;

        operator std::string() const { return get_json_string(); }
    };

    typedef std::vector<Record> RecordsList;
};

#endif
