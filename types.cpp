#include "types.hpp"

Node::Node(std::string type) : type(std::move(type)) {}

Number::Number(const string &value) : value(std::stoi(value)), Node(EPrimitiveSymbolTypeToString[INT_TYPE]) {}

Bool::Bool(bool value) : value(value), Node(EPrimitiveSymbolTypeToString[BOOL_TYPE]) {}

String::String(std::string value) : value(std::move(value)), Node(EPrimitiveSymbolTypeToString[STRING_TYPE]) {}

ExpList::ExpList(vector<NodePointer> &expList) : expList(std::move(expList)), Node() {}

ExpList::ExpList() : expList() {}

Symbol::Symbol(string &name, int offset, std::string type) : name(name), offset(offset), Node(std::move(type)) {}

Symbol::Symbol(string &name, EPrimitiveSymbolType type) : name(name), offset(NaN), Node(EPrimitiveSymbolTypeToString[type]) {}

Parameters::Parameters(std::vector<std::shared_ptr<Symbol>> parameters) : parameters(std::move(parameters)) {}

bool Parameters::operator==(const Parameters &other) const {
    return parameters == other.parameters;
}

Function::Function(string &name, Parameters &&parameters, EPrimitiveSymbolType returnType) : Symbol(name, 0), parameters(std::move(parameters)), returnType(EPrimitiveSymbolTypeToString[returnType]) {
    _fillParameters();
}

Function::Function(string &name, Parameters parameters, string &returnTypeString) : Symbol(name, 0), parameters(std::move(parameters)), returnType(returnTypeString) {
    _fillParameters();
}

void Function::_fillParameters() {
    std::vector<std::string> parametersType;
    int parameter_offset = -1;
    for (auto& parameter : this->parameters.parameters) {
        parameter->offset = parameter_offset;
        parameter_offset--;
        parametersType.push_back(parameter->type);
    }
    this->type = output::makeFunctionType(this->returnType, parametersType);
}
