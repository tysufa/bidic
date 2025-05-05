#pragma once
#include "../nast/nast.hh"
#include <string>

class Emitor {
 public:
  Emitor() = default;
  std::string Emit(std::unique_ptr<nast::Program> program);

 private:
};
