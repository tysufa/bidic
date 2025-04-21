cmake_minimum_required(VERSION 3.15)
project(Bidic LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Lexer Library ---
add_library(Lexer STATIC
  src/lexer/lexer.hh # adding hh files is optionnal
  src/lexer/lexer.cc
)
target_include_directories(Lexer PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Ast Library (Header only) ---
add_library(Ast INTERFACE)

target_include_directories(Ast INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
)

# --- Main Executable ---
add_executable(Bidic src/main.cpp)
target_link_libraries(Bidic PRIVATE Lexer Ast)

# --- Manual Test Executable ---
add_executable(LexerTests
  tests/lexer/TestLexer.cpp
)
add_executable(ParserTest
  tests/parser/TestParser.cpp
)

target_link_libraries(LexerTests PRIVATE Lexer)

target_link_libraries(ParserTest PRIVATE Lexer Ast)

# Optional: Add as a test target
enable_testing()
add_test(NAME LexerTests COMMAND LexerTests)
add_test(NAME ParserTest COMMAND ParserTest)
