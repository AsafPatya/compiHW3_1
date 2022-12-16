#ifndef _PARSER_UTILS_
#define _PARSER_UTILS_

#include <memory>
#include <algorithm>
#include "string"
#include "types.hpp"
#include "symbol_table.hpp"
#include "semantic_utils.hpp"

class Parser {
private:
    SymbolTable _symbolTable;
    Parser() : _symbolTable() {}

    void _checkFunction(const NodePointer& exp, int lineno);

    void _checkUndefFunction(const NodePointer& exp, int lineno);

    void _checkUndefId(const NodePointer& exp, int lineno);

    void _checkAlreadyDefId(const std::string& idName, int lineno);

    void _checkCast(const NodePointer& type, const NodePointer& exp, int lineno);

public:
    Parser(Parser&) = delete;
    Parser operator=(Parser&) = delete;

    static Parser &instance();

    void deriveBool(const NodePointer& exp, int lineno);

    NodePointer deriveNumB(const NodePointer& num, int lineno);

    NodePointer deriveId(const NodePointer& id, int lineno);

    NodePointer deriveNot(const NodePointer& exp, int lineno);

    NodePointer deriveAndOr(const NodePointer& exp1, const NodePointer& exp2, int lineno);

    NodePointer deriveRelop(const NodePointer& exp1, const NodePointer& exp2, int lineno);

    NodePointer deriveCast(const NodePointer& type, const NodePointer& exp, int lineno);

    NodePointer deriveBinop(const NodePointer& exp1, const NodePointer& exp2, int lineno);

    NodePointer deriveExpList(const NodePointer& exp, NodePointer expList = nullptr);

    NodePointer deriveCall(int lineno, const NodePointer& id, const NodePointer& expList = nullptr);

    NodePointer deriveFormalDecl(const NodePointer& type, const NodePointer& id, int lineno);

    std::shared_ptr<Parameters> deriveFormals(const NodePointer& formalsList = nullptr);

    std::shared_ptr<Parameters> deriveFormalsList(const NodePointer& formalDecl, const NodePointer& formalsDeclList = nullptr);

    NodePointer deriveFuncSignature(const NodePointer& returnType, const NodePointer& id, NodePointer& formals, int lineno);

    void deriveStatement(const NodePointer& type, const NodePointer& id, int lineno);

    void deriveDeclaration(const NodePointer& type, const NodePointer& id, const NodePointer& exp, int lineno);

    void deriveAssignment(const NodePointer& id, const NodePointer& exp, int lineno);

    void deriveReturnVoid(int lineno);

    void deriveReturnExp(const NodePointer& exp, int lineno);

    void deriveBreak(int lineno);

    void deriveContinue(int lineno);

    void deriveProgram();

    void pushScope(bool isWhile = false);

    void popScope();
};

#endif // _PARSER_UTILS_
