#ifndef _TYPES_
#define _TYPES_

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "hw3_output.hpp"
#define YYSTYPE NodePointer

const int NaN = -1;

enum EPrimitiveSymbolType {INT_TYPE = 0, BOOL_TYPE, STRING_TYPE, BYTE_TYPE, B_TYPE, VOID_TYPE};
const std::string EPrimitiveSymbolTypeToString[] = {"INT", "BOOL", "STRING", "BYTE", "B", "VOID"};

struct Node {
    std::string type;
    explicit Node(std::string type = "");
    virtual ~Node() = default;
};
using NodePointer = std::shared_ptr<Node>;

struct Number : public Node {
    int value;
    explicit Number(const std::string& value);
};

struct Bool : public Node {
    bool value;
    explicit Bool(bool value);
};

struct String : public Node {
    std::string value;
    explicit String(std::string value);
};

struct ExpList : public Node {
    std::vector<NodePointer> expList;
    ExpList();
    explicit ExpList(std::vector<NodePointer>& expList);
};

struct Symbol : public Node {
    std::string name;
    int offset;
    Symbol(std::string& name, int offset, std::string type = "");
    explicit Symbol(std::string& name, EPrimitiveSymbolType type);
};
using SymbolPointer = std::shared_ptr<Symbol>;

struct Parameters : public Node {
    std::vector<std::shared_ptr<Symbol>> parameters;
    explicit Parameters(std::vector<std::shared_ptr<Symbol>> parameters = std::vector<std::shared_ptr<Symbol>>());
    bool operator==(const Parameters& other) const;
};

struct Function : public Symbol {
    Parameters parameters;
    std::string returnType;

    Function(std::string& name, Parameters&& parameters, EPrimitiveSymbolType returnType);
    Function(std::string& name, Parameters parameters, std::string& returnTypeString);

    private:
    void _fillParameters();
};

#endif  // _TYPES_
