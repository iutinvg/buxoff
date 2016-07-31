#include "Storage.h"


Buxoff::Storage get_clean_storage() {
    auto s = Buxoff::Storage("test.db");
    s.__clear();
    return s;
}