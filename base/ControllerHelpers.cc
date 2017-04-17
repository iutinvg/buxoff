#include "ControllerHelpers.h"
#include "Storage.h"
#include "Record.h"
#include "Email.h"
#include "TagsStorage.h"

using namespace Buxoff;

void Buxoff::controller_add(Connection* c, const Record& r)
{
    r.validate();
    RecordStorage rs{c};
    rs.put(r);
    TagsStorage ts{c};
    ts.put_all(r.tags());
}

std::string Buxoff::controller_push(Connection* c, const Record& r)
{
    if (!r.empty()) {
        controller_add(c, r);
    }
    RecordStorage rs{c};
    Email e{rs.all()};
    rs.clear();
    return e.body();
}
