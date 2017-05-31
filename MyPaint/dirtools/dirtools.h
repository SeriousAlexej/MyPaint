#ifndef DIR_TOOLS_H
#define DIR_TOOLS_H
#include <string>
#include <vector>

namespace mp
{

namespace dir
{

char                        GetDirDelimiter();
std::vector<std::string>    GetLibrariesInDir(const std::string& dirname);

}

}

#endif
