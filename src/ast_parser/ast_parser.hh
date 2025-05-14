#pragma once
#include "scug.hh"
#include "ast.hh"
#include <memory>

std::string MakeTemporaryVariableName();
std::unique_ptr<scug::Expression> ParseAstExpression(
    scug::FunctionDeclaration* scug,
    const std::unique_ptr<Expression>& expr);
std::unique_ptr<scug::Program> eval(std::unique_ptr<Program> ast);
