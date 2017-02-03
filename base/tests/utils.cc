#include "utils.h"

Buxoff::Storage get_clean_storage() {
    auto s = Buxoff::Storage("test.db");
    s.clear();
    return s;
}
