#ifndef __Buxoff__FuzzySearch__
#define __Buxoff__FuzzySearch__

#include <string>


using namespace std;

namespace Buxoff {

bool search(const string& needle, const string& haystack) {
    auto nlen = needle.size(), hlen = haystack.size();
    if (nlen > hlen) {
        return false;
    }
    if (nlen == hlen) {
        return needle == haystack;
    }
    decltype(nlen) i{0}, j{0};

    outer:
    while (i < nlen) {
        auto c = needle[i++];
        while (j < hlen) {
            if (haystack[j++] == c) {
                goto outer;
            }
        }
        return false;
    }
    return true;
}

// wstring str2wstr(const string& s) {
//     wstring tmp;
//     copy(s.begin(), s.end(), back_inserter(tmp));
//     return tmp;
// }

// bool search(const char* needle, const char* haystack) {
//     return search(str2wstr(needle), str2wstr(haystack));
// }

} // namespace end

#endif
