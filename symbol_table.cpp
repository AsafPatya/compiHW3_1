#include "symbol_table.hpp"

void SymbolTable::_pushDefaultFunctions() {
    // push print
    Parameters printParameters;
    std::string printFunctionParameterName = "string";
    std::string printFunctionName = "print";
    printParameters.parameters.emplace_back(std::make_shared<Symbol>(printFunctionParameterName, STRING_TYPE));
    Function printFunction(printFunctionName, std::move(printParameters), VOID_TYPE);
    pushSymbolToTopScope(std::make_shared<Function>(printFunction), false);

    // push printi
    Parameters printiParameters;
    std::string printiFunctionParameterName = "int";
    std::string printiFunctionName = "printi";
    printiParameters.parameters.emplace_back(std::make_shared<Symbol>(printiFunctionParameterName, INT_TYPE));
    Function printiFunction(printiFunctionName, std::move(printiParameters), VOID_TYPE);
    pushSymbolToTopScope(std::make_shared<Function>(printiFunction), false);
}

SymbolTable::SymbolTable() : _scopes(), _offsets(), _symbols() {
    // Add the global scope
    pushScope();
    _pushDefaultFunctions();
}

bool SymbolTable::isNewSymbol(const string &symbolName) {
    return _symbols.find(symbolName) == _symbols.end();
}

void SymbolTable::pushScope(const string &returnType, bool isWhile) {
    std::string scopeReturnType = (!_scopes.empty() && returnType.empty()) ? _scopes.top().returnType : returnType;
    _scopes.push(std::move(Scope(scopeReturnType, (!_scopes.empty() && !isWhile) ? _scopes.top().isWhile : isWhile)));
    _offsets.push(_offsets.empty() ? 0 : getCurrentOffset());
}

void SymbolTable::popScope() {
    output::endScope();
    for (const auto& symbol : _scopes.top().getSymbols()) {
        output::printID(symbol->name, symbol->offset, symbol->type);
        _symbols.erase(symbol->name);
    }
    _offsets.pop();
    _scopes.pop();
}

void SymbolTable::pushSymbolToTopScope(SymbolPointer symbol, bool increaseOffset) {
    _symbols.emplace(symbol->name, symbol);
    _scopes.top().addSymbol(symbol);

    if (increaseOffset) {
        int currentOffset = getCurrentOffset();
        _offsets.pop();
        _offsets.push( symbol->offset >= 0 ? currentOffset + 1 : currentOffset);
    }
}

SymbolPointer SymbolTable::getSymbol(const string &symbolName) {
    return _symbols[symbolName];
}

int SymbolTable::getCurrentOffset() {
    return _offsets.top();
}

Scope &SymbolTable::getScope() {
    return _scopes.top();
}

std::unordered_map<std::string, SymbolPointer> &SymbolTable::getSymbols() {
    return _symbols;
}
