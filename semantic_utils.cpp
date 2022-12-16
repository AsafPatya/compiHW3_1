#include "semantic_utils.hpp"

bool isByteTooLarge(const int &num) {
    return !(num >= 0 && num <= 255);
}

bool isBool(const NodePointer &exp) {
    return exp->type == EPrimitiveSymbolTypeToString[BOOL_TYPE];
}

bool isFunction(const NodePointer &exp) {
    return exp->type.find("->") != string::npos;
}

bool isInt(const NodePointer &exp) {
    return exp->type == EPrimitiveSymbolTypeToString[INT_TYPE];
}

bool isNum(const NodePointer &exp) {
    return isInt(exp) || exp->type == EPrimitiveSymbolTypeToString[BYTE_TYPE];
}

bool isValidCast(const NodePointer &type, const NodePointer &exp) {
    return (exp->type == type->type) ||
           (type->type == EPrimitiveSymbolTypeToString[INT_TYPE] && exp->type == EPrimitiveSymbolTypeToString[BYTE_TYPE]) ||
           (type->type == EPrimitiveSymbolTypeToString[BYTE_TYPE] && exp->type == EPrimitiveSymbolTypeToString[INT_TYPE]);
}

bool isExpListFitToBeParameters(const shared_ptr<Function> &function, const shared_ptr<ExpList> &expList) {
    if (function->parameters.parameters.size() != expList->expList.size()) {
        return false;
    }

    for (int parameterNum = 0; parameterNum < function->parameters.parameters.size(); parameterNum++) {
        if (!isValidCast(function->parameters.parameters[parameterNum], expList->expList[parameterNum])) {
            return false;
        }
    }

    return true;
}
