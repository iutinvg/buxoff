#include "utils.h"

void clean_storage() {
    auto c = Buxoff::Connection("test.db");
    auto s = Buxoff::StringStorage(&c, {});
    s.clear();
}
