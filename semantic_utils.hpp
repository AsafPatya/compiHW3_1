#ifndef _SEMANTIC_UTILS_
#define _SEMANTIC_UTILS_

#include "types.hpp"

bool isByteTooLarge(const int& num);

bool isBool(const NodePointer& exp);

bool isFunction(const NodePointer& exp);

bool isInt(const NodePointer& exp);

bool isNum(const NodePointer& exp);

bool isValidCast(const NodePointer& type, const NodePointer& exp);

bool isExpListFitToBeParameters(const std::shared_ptr<Function>& function, const std::shared_ptr<ExpList>& expList);

#endif // _SEMANTIC_UTILS_
