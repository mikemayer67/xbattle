#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <set>
#include <string>

typedef std::vector<std::string> StringList_t;
typedef std::set<std::string>    StringSet_t;

StringList_t split_string(std::string,char);

#endif // _UTIL_H_
