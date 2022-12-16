#include "scope.hpp"

Scope::Scope(const std::string &returnType, bool isWhile, std::vector <SymbolPointer> symbols) : returnType(returnType), isWhile(isWhile), _symbols(std::move(symbols)) {}

std::vector<SymbolPointer> &Scope::getSymbols() {
    return _symbols;
}

void Scope::addSymbol(SymbolPointer &symbol) {
    _symbols.push_back(symbol);
}
