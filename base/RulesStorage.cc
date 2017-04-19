#include <iostream>
#include "RulesStorage.h"
#include "json.hpp"

using namespace Buxoff;
using namespace std;

void RulesStorage::put(const string& desc, const set<string>& tags)
        throw (StorageError) {
    if (tags.size() == 0 || desc.empty())
        throw StorageError{"empty tags list or description"};
    nlohmann::json j(tags);
    put(prefix + desc, j.dump());
}

set<string> RulesStorage::get(const string& desc) {
    auto json_str = StringStorage::get(prefix + desc, "[]");
    return nlohmann::json::parse(json_str);
}
