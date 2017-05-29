#include "catch.hpp"
#include "Record.h"
#include "TagsStorage.h"
#include "ControllerHelpers.h"
#include "utils.h"

using namespace Buxoff;

TEST_CASE("add-record", "[controller]") {
    clean_storage();
    auto c = Connection("test.db");
    controller_add(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"});
    RecordStorage rs{&c};
    REQUIRE(rs.all().size() == 1);
}

TEST_CASE("add-tags", "[controller]") {
    clean_storage();
    auto c = Connection("test.db");
    controller_add(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"});
    TagsStorage ts{&c};
    auto tags = ts.all();
    sort(tags.begin(), tags.end());

    REQUIRE(tags.size() == 2);
    REQUIRE(tags[0] == "tag1");
    REQUIRE(tags[1] == "tag2");
}

TEST_CASE("push-clear", "[controller]") {
    clean_storage();
    auto c = Connection("test.db");
    controller_add(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"});
    controller_push(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"}, true);

    auto rs = RecordStorage{&c};
    REQUIRE(rs.all().size() == 0);
}

TEST_CASE("push-not-clear", "[controller]") {
    clean_storage();
    auto c = Connection("test.db");
    controller_add(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"});
    controller_push(&c, Record{"345.67", "desc", {"tag1", "tag2"}, "cash"}, false);

    auto rs = RecordStorage{&c};
    REQUIRE(rs.all().size() == 2);
}
