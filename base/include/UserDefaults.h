#ifndef __Buxoff__UserDefaults__
#define __Buxoff__UserDefaults__

#include <string>
#include "json.hpp"

#include "Storage.h"

namespace Buxoff {
    const std::string User_ID_Prefix{"user_defaults"};

    template<typename T>
    void put_ud(Storage& s, const std::string& key, T val)
    {
        auto o = nlohmann::json::parse(s.get_string(User_ID_Prefix, "{}"));
        o[key] = val;
        s.put(User_ID_Prefix, o.dump());
    }

    template<typename T=std::string>
    T get_ud(Storage& s, const std::string& key, const T& def)
    {
        auto o = nlohmann::json::parse(s.get_string(User_ID_Prefix, "{}"));
        if (o.count(key)) {
            return o[key];
        }
        return def;
    }
};

#endif
