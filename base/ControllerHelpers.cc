#include "ControllerHelpers.h"
#include "Storage.h"
#include "Record.h"
#include "Email.h"

using namespace Buxoff;

void Buxoff::controller_add(Storage& storage, const Record& r)
{
    r.validate();
    storage.put(r);
}

std::string Buxoff::controller_push(Storage& storage, const Record& r)
{
    if (!r.empty()) {
        controller_add(storage, r);
    }
    Email e{storage.get_records()};
    storage.clear_records();
    return e.body();
}
