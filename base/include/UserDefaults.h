#ifndef __Buxoff__UserDefaults__
#define __Buxoff__UserDefaults__

#include <string>
#include "json.hpp"

#include "Storage.h"

// TODO: how to avoid put_ud(s, key, std::string{"val"})?
//      should be: put_ud(s, key, "val")

namespace Buxoff {
    const std::string User_ID_Prefix{"user_defaults"};

    template<typename T>
    void put_ud(Storage& s, const std::string& key, const T& val)
    {
        auto o = nlohmann::json::parse(s.get_string(User_ID_Prefix, "{}"));
        o[key] = val;
        s.put(User_ID_Prefix, o.dump());
    }

    template<typename T>
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
