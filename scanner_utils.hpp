#ifndef _SCANNER_UTILS_
#define _SCANNER_UTILS_

#include <memory>
#include "types.hpp"
#include "parser.tab.hpp"
#include "hw3_output.hpp"

int tokenToNode(const std::string& value, int token);

void errorLex(int lineno);

#endif // _SCANNER_UTILS_
