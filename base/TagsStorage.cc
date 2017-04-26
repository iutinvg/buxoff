// #include <iostream>

#include "TagsStorage.h"
// #include "Record.h"

using namespace Buxoff;
using namespace std;

string TagsStorage::put(const string& value) throw (StorageError) {
    // cout << "proper put\n";

    string key{prefix + value};
    StringStorage::put(key, value);
    return key;
}
