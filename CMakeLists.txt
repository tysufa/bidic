cmake_minimum_required(VERSION 3.15)
project(Bidic LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Lexer Library ---
add_library(Lexer STATIC
  src/lexer/lexer.hh
  src/lexer/lexer.cc
)
target_include_directories(Lexer PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
)

# --- Main Executable ---
add_executable(Bidic src/main.cpp)
target_link_libraries(Bidic PRIVATE Lexer)

# --- Manual Test Executable ---
add_executable(LexerTests
  tests/lexer/TestLexer.cpp
)
target_link_libraries(LexerTests PRIVATE Lexer)

# Optional: Add as a test target
enable_testing()
add_test(NAME LexerTests COMMAND LexerTests)