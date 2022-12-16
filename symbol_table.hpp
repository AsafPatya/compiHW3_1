#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

#include <memory>
#include <stack>
#include <unordered_map>
#include <utility>
#include "types.hpp"
#include "scope.hpp"
#include "hw3_output.hpp"

class SymbolTable {
private:
    std::stack<Scope> _scopes;
    std::stack<int> _offsets;
    std::unordered_map<std::string, SymbolPointer> _symbols;

    void _pushDefaultFunctions();

public:
    SymbolTable();

    bool isNewSymbol(const std::string& symbolName);

    void pushScope(const std::string& returnType = "", bool isWhile = false);

    void popScope();

    void pushSymbolToTopScope(SymbolPointer symbol, bool increaseOffset = true);

    SymbolPointer getSymbol(const std::string& symbolName);

    int getCurrentOffset();

    Scope& getScope();

    std::unordered_map<std::string, SymbolPointer>& getSymbols();

    // TODO- remove; this is just for the tests
    std::stack<Scope>& _getScopes() {
        return _scopes;
    }
    std::stack<int>& _getOffsets() {
        return _offsets;
    }
};

#endif  // _SYMBOL_TABLE_
