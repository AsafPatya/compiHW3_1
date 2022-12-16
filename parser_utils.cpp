#include "parser_utils.hpp"

void Parser::_checkFunction(const NodePointer &exp, int lineno) {
    if (isFunction(exp)) {
        std::shared_ptr<Function> functionSymbol = dynamic_pointer_cast<Function>(exp);
        output::errorUndef(lineno, functionSymbol->name);
        exit(0);
    }
}

void Parser::_checkUndefFunction(const NodePointer &exp, int lineno) {
    if (isFunction(exp)) {
        std::shared_ptr<String> functionName = std::dynamic_pointer_cast<String>(exp);
        std::shared_ptr<Symbol> symbolName = std::dynamic_pointer_cast<Symbol>(exp);
        if ((functionName && _symbolTable.isNewSymbol(functionName->value)) || (symbolName || _symbolTable.isNewSymbol(symbolName->name))) {
            output::errorUndefFunc(lineno, functionName ? functionName->value : symbolName->name);
            exit(0);
        }
    }
}

void Parser::_checkUndefId(const NodePointer &exp, int lineno) {
    std::shared_ptr<String> id = std::dynamic_pointer_cast<String>(exp);
    if (_symbolTable.isNewSymbol(id->value)) {
        output::errorUndef(lineno, id->value);
        exit(0);
    }
}

void Parser::_checkAlreadyDefId(const string &idName, int lineno) {
    if (!_symbolTable.isNewSymbol(idName)) {
        output::errorDef(lineno, idName);
        exit(0);
    }
}

void Parser::_checkCast(const NodePointer &type, const NodePointer &exp, int lineno) {
    if (!isValidCast(type, exp)) {
        output::errorMismatch(lineno);
        exit(0);
    }
}

Parser &Parser::instance() {
    static Parser parser;
    return parser;
}

void Parser::deriveBool(const NodePointer &exp, int lineno) {
    _checkFunction(exp, lineno);

    if (!isBool(exp)) {
        output::errorMismatch(lineno);
        exit(0);
    }
}

NodePointer Parser::deriveNumB(const NodePointer &num, int lineno) {
    std::shared_ptr<Number> derivedNumB = std::dynamic_pointer_cast<Number>(num);
    if (isByteTooLarge(derivedNumB->value)) {
        output::errorByteTooLarge(lineno, std::to_string(derivedNumB->value));
        exit(0);
    }
    return derivedNumB;
}

NodePointer Parser::deriveId(const NodePointer &id, int lineno) {
    _checkUndefId(id, lineno);
    std::shared_ptr<String> derivedString = std::dynamic_pointer_cast<String>(id);
    return _symbolTable.getSymbol(derivedString->value);
}

NodePointer Parser::deriveNot(const NodePointer &exp, int lineno) {
    deriveBool(exp, lineno);
    return exp;
}

NodePointer Parser::deriveAndOr(const NodePointer &exp1, const NodePointer &exp2, int lineno) {
    deriveBool(exp1, lineno);
    deriveBool(exp2, lineno);
    return exp1;
}

NodePointer Parser::deriveRelop(const NodePointer &exp1, const NodePointer &exp2, int lineno) {
    _checkFunction(exp1, lineno);
    _checkFunction(exp2, lineno);
    if (!isNum(exp1) || !isNum(exp2)) {
        output::errorMismatch(lineno);
        exit(0);
    }
    return make_shared<Bool>(true);
}

NodePointer Parser::deriveCast(const NodePointer &type, const NodePointer &exp, int lineno) {
    _checkFunction(exp, lineno);
    _checkCast(type, exp, lineno);
    exp->type = type->type;
    return exp;
}

NodePointer Parser::deriveBinop(const NodePointer &exp1, const NodePointer &exp2, int lineno) {
    _checkFunction(exp1, lineno);
    _checkFunction(exp2, lineno);

    if (!isNum(exp1) || !isNum(exp2)) {
        output::errorMismatch(lineno);
        exit(0);
    }

    return isInt(exp1) ? exp1 : exp2;
}

NodePointer Parser::deriveExpList(const NodePointer &exp, NodePointer expList) {
    std::shared_ptr<ExpList> expListNode = std::make_shared<ExpList>();
    if (expList) {
        expListNode = std::dynamic_pointer_cast<ExpList>(expList);
    }

    expListNode->expList.push_back(exp);
    return expListNode;
}

NodePointer Parser::deriveCall(int lineno, const NodePointer &id, const NodePointer &expList) {
    _checkUndefFunction(id, lineno);
    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    SymbolPointer idSymbol = _symbolTable.getSymbol(idName->value);
    if (!idSymbol || !isFunction(idSymbol)) {
        output::errorUndefFunc(lineno, idName->value);
        exit(0);
    }

    std::shared_ptr<Function> functionSymbol = std::dynamic_pointer_cast<Function>(idSymbol);
    std::shared_ptr<ExpList> expListNode = std::make_shared<ExpList>();
    if (expList) {
        std::vector<NodePointer> expListVector = std::dynamic_pointer_cast<ExpList>(expList)->expList;
        std::reverse(expListVector.begin(),expListVector.end());
        expListNode = std::make_shared<ExpList>(expListVector);
    }

    if (!isExpListFitToBeParameters(functionSymbol, expListNode)) {
        vector<std::string> expectedParameters;
        for (const auto& symbol : functionSymbol->parameters.parameters) {
            expectedParameters.push_back(symbol->type);
        }
        output::errorPrototypeMismatch(lineno, functionSymbol->name, expectedParameters);
        exit(0);
    }

    return std::make_shared<Node>(functionSymbol->returnType);
}

NodePointer Parser::deriveFormalDecl(const NodePointer &type, const NodePointer &id, int lineno) {
    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    _checkAlreadyDefId(idName->value, lineno);
    return std::make_shared<Symbol>(idName->value, _symbolTable.getCurrentOffset() + 1, type->type);
}

std::shared_ptr<Parameters> Parser::deriveFormals(const NodePointer &formalsList) {
    std::shared_ptr<Parameters> parametersNode = std::make_shared<Parameters>();
    if (formalsList) {
        std::vector<std::shared_ptr<Symbol>> parametersVector = (*(std::dynamic_pointer_cast<Parameters>(formalsList))).parameters;
        std::reverse(parametersVector.begin(), parametersVector.end());
        parametersNode = std::make_shared<Parameters>(parametersVector);
    }
    return parametersNode;
}

std::shared_ptr<Parameters> Parser::deriveFormalsList(const NodePointer &formalDecl, const NodePointer &formalsDeclList) {
    std::shared_ptr<Symbol> formalDeclSymbol = std::dynamic_pointer_cast<Symbol>(formalDecl);

    std::shared_ptr<Parameters> parametersList = std::make_shared<Parameters>();
    if (formalsDeclList) {
        parametersList = std::dynamic_pointer_cast<Parameters>(formalsDeclList);
    }

    parametersList->parameters.push_back(formalDeclSymbol);
    return parametersList;
}

NodePointer
Parser::deriveFuncSignature(const NodePointer &returnType, const NodePointer &id, NodePointer &formals, int lineno) {
    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    _checkAlreadyDefId(idName->value, lineno);

    std::shared_ptr<Parameters> parameters = std::dynamic_pointer_cast<Parameters>(formals);
    std::shared_ptr<Function> newFunction = std::make_shared<Function>(idName->value, *parameters, returnType->type);

    _symbolTable.pushSymbolToTopScope(newFunction, false);
    _symbolTable.pushScope(returnType->type);

    for (const auto& parameter : newFunction->parameters.parameters) {
        _checkAlreadyDefId(parameter->name, lineno);
        _symbolTable.pushSymbolToTopScope(parameter);
    }

    return newFunction;
}

void Parser::deriveStatement(const NodePointer &type, const NodePointer &id, int lineno) {
    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    _checkAlreadyDefId(idName->value, lineno);
    _symbolTable.pushSymbolToTopScope(std::make_shared<Symbol>(idName->value, _symbolTable.getCurrentOffset(), type->type));
}

void Parser::deriveDeclaration(const NodePointer &type, const NodePointer &id, const NodePointer &exp, int lineno) {  // TODO- check auto
    _checkUndefFunction(exp, lineno);

    if (type) {
        _checkCast(type, exp, lineno);
    }

    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    _checkAlreadyDefId(idName->value, lineno);
    _symbolTable.pushSymbolToTopScope(std::make_shared<Symbol>(idName->value, _symbolTable.getCurrentOffset(), type ? type->type : exp->type));
}

void Parser::deriveAssignment(const NodePointer &id, const NodePointer &exp, int lineno) {
    _checkUndefId(id, lineno);
    _checkUndefFunction(exp, lineno);

    std::shared_ptr<String> idName = std::dynamic_pointer_cast<String>(id);
    SymbolPointer symbol = _symbolTable.getSymbol(idName->value);
    _checkFunction(symbol, lineno);

    _checkCast(symbol, exp, lineno);
}

void Parser::deriveReturnVoid(int lineno) {
    _checkCast(std::make_shared<Node>(_symbolTable.getScope().returnType), std::make_shared<Node>(EPrimitiveSymbolTypeToString[VOID_TYPE]), lineno);
}

void Parser::deriveReturnExp(const NodePointer &exp, int lineno) {
    _checkUndefFunction(exp, lineno);

    if (_symbolTable.getScope().returnType == EPrimitiveSymbolTypeToString[VOID_TYPE]) {
        output::errorMismatch(lineno);
        exit(0);
    }

    _checkCast(std::make_shared<Node>(_symbolTable.getScope().returnType), exp, lineno);
}

void Parser::deriveBreak(int lineno) {
    if (!_symbolTable.getScope().isWhile) {
        output::errorUnexpectedBreak(lineno);
        exit(0);
    }
}

void Parser::deriveContinue(int lineno) {
    if (!_symbolTable.getScope().isWhile) {
        output::errorUnexpectedContinue(lineno);
        exit(0);
    }
}

void Parser::deriveProgram() {
    for (auto& symbol : _symbolTable.getSymbols()) {
        if (isFunction(symbol.second)) {
            std::shared_ptr<Function> function = std::dynamic_pointer_cast<Function>(symbol.second);

            if (function->name == "main" && function->parameters.parameters.empty() && function->returnType == EPrimitiveSymbolTypeToString[VOID_TYPE]) {
                popScope();
                return;
            }
        }
    }

    output::errorMainMissing();
    exit(0);
}

void Parser::pushScope(bool isWhile) {
    _symbolTable.pushScope("", isWhile);
}

void Parser::popScope() {
    _symbolTable.popScope();
}
