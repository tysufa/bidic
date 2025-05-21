#include "CLI11.hpp"
#include "ast_parser.hh"
#include "emission.hh"
#include "lexer.hh"
#include "parser/parser.hh"
#include "ast_parser_asm.hh"
#include "emission_asm.hh"
#include "parser/parser_asm.hh"
#include <fstream>
#include <filesystem> // For file existence check


std::string readFileToString(const std::string& filePath) {
  // Check if file exists first (optional but recommended)
  if (!std::filesystem::exists(filePath))
    throw std::runtime_error("File not found: " + filePath);

  // Open and read the file
  std::ifstream file(filePath, std::ios::binary);

  if (!file.is_open())
    throw std::runtime_error("Failed to open file: " + filePath);

  // Read entire content into string
  return std::string(
             (std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>()
         );
}

int main(int argc, char** argv) {
  CLI::App app{"MyCompiler - A simple C++ compiler"};

  // Add options/flags
  std::string input_file;
  bool optimize = false;
  std::string output_file = "a.out";
  bool asm_code = true;

  app.add_option("input_file,-i,--input", input_file,
                 "Input source file")->required()->check(CLI::ExistingFile);
  app.add_flag("-O,--optimize", optimize, "Enable optimizations");
  app.add_option("-o,--output", output_file, "Output executable name");
  app.add_flag("-r, --reverse", asm_code=false, "Compile from assembly to C");

  CLI11_PARSE(app, argc, argv);

  try {
    std::string fileContent = readFileToString(input_file);
    Lexer l(fileContent, !asm_code);

    std::string output;

    if (!asm_code) {
      Parser p(l.Tokens());
      auto scug = eval(p.ParseProgram());
      Emitor e(std::move(scug));

      output = e.Emit();
    }
    else{
      Parser_asm p(l.Tokens());
      Ast_Parser_Asm parser(p.ParseProgram());
      Emitor_asm e(parser.ParseProgram());

      output = e.Emit();
    }

    std::ofstream file(output_file);  // Creates/overwrites the file

    if (file.is_open()) {
      file << output;
      file.close();
    } else
      std::cerr << "Failed to create file!\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}
