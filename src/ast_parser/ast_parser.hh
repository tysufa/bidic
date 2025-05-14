#pragma once
#include "nast.hh"
#include "ast.hh"
#include <memory>

std::string MakeTemporaryVariableName();
std::unique_ptr<nast::Expression> ParseAstExpression(
    nast::FunctionDeclaration* nast,
    const std::unique_ptr<Expression>& expr);
std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast);
