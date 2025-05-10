#pragma once
#include "nast.hh"
#include "ast.hh"
#include <memory>

std::unique_ptr<nast::Expression> ParseAstExpression(nast::Program* nast,
    const std::unique_ptr<nast::Expression>& expr);
std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast);
