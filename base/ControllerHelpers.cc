#include "ControllerHelpers.h"
#include "Storage.h"
#include "Record.h"
#include "Email.h"
#include "TagsStorage.h"
#include "RulesStorage.h"

using namespace Buxoff;

void Buxoff::controller_add(Connection* c, const Record& r)
{
    r.validate();
    RecordStorage rs{c};
    TagsStorage ts{c};
    RulesStorage rls{c};

    try {
        auto tags = r.tags();
        rs.put(r);
        ts.put_all(tags);
        rls.put(r.description(), tags);
    } catch (StorageError& e) {
        // from user point of view, it's totally fine
        // to store empty tags or descriptions
        // so we just swallow this type of exceptions
        // because all we want is just avoid storing empty strings
    }
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
