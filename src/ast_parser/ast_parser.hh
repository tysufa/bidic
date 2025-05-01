#pragma once
#include "nast.hh"
#include "ast.hh"
#include <memory>

std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast);
