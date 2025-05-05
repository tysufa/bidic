#pragma once
#include "../nast/nast.hh"
#include <string>

class Emitor {
public:
  Emitor() = default;
  std::string Emit(const nast::Program &program);

private:
};
