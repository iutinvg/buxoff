#ifndef __Buxoff__FuzzySearch__
#define __Buxoff__FuzzySearch__

#include <string>

namespace Buxoff {

// inspired by https://github.com/bevacqua/fuzzysearch
bool search(const std::string& needle, const std::string& haystack);

} // namespace end

#endif
