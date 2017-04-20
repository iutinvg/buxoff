#include "TagsStorage.h"
// #include "Record.h"

using namespace Buxoff;
using namespace std;

string TagsStorage::put(const string& value) {
    string key{prefix + value};
    put(key, value);
    return key;
}
