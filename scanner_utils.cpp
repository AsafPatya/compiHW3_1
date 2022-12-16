#include "scanner_utils.hpp"

int tokenToNode(const std::string &value, int token) {
    switch (token) {
        case NUM:
            yylval = std::make_shared<Number>(value);
            break;

        case TRUE:
            yylval = std::make_shared<Bool>(true);
            break;

        case FALSE:
            yylval = std::make_shared<Bool>(false);
            break;

        case ID:
        case STRING:
            yylval = std::make_shared<String>(value);
            break;

        case INT:
            yylval = std::make_shared<Node>(EPrimitiveSymbolTypeToString[INT_TYPE]);
            break;

        case BYTE:
            yylval = std::make_shared<Node>(EPrimitiveSymbolTypeToString[BYTE_TYPE]);
            break;

        case BOOL:
            yylval = std::make_shared<Node>(EPrimitiveSymbolTypeToString[BOOL_TYPE]);
            break;

        case VOID:
            yylval = std::make_shared<Node>(EPrimitiveSymbolTypeToString[VOID_TYPE]);
            break;

        default:
            yylval = nullptr;
            break;
    }
    return token;
}

void errorLex(int lineno) {
    output::errorLex(lineno);
    exit(0);
}
