// #include <iostream>
#include <set>
#include "catch.hpp"
#include "Storage.h"
#include "Record.h"

#include "utils.h"

using namespace Buxoff;
using namespace std;

TEST_CASE("record-storage-all", "[record-storage]") {
    clean_storage();
    auto c = Connection("test.db");
    Record r{"345.67", "desc", {"tag1", "tag2"}, "cash"};
    RecordStorage rs{&c};
    rs.put(r);
    vector<Record> records = rs.all();
    REQUIRE(records.size() == 1);
}
