#ifndef _SCOPE_
#define _SCOPE_

#include <utility>
#include <memory>
#include <vector>
#include "types.hpp"

class Scope {
private:
    std::vector<SymbolPointer> _symbols;

public:
    std::string returnType;
    bool isWhile;

    explicit Scope(const std::string& returnType, bool isWhile = false, std::vector<SymbolPointer> symbols = std::vector<SymbolPointer>());
    ~Scope() = default;

    std::vector<SymbolPointer>& getSymbols();

    void addSymbol(SymbolPointer& symbol);
};

#endif  // _SCOPE_
