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

# --- Parser Library ---
add_library(Parser STATIC
  src/parser/parser.cc
)

target_include_directories(Parser PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
)

# --- Main Executable ---
add_executable(Bidic src/main.cpp)
target_link_libraries(Bidic PRIVATE Lexer Ast)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/155b337c938a2953e5675f9dc18c99f05f4c85d0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  hello_test
  tests/hello_test.cc
)
target_link_libraries(
  hello_test
  GTest::gtest_main
  Lexer
)


# --- Manual Test Executable ---
add_executable(LexerTests
  tests/lexer/TestLexer.cpp
)
add_executable(ParserTests
  tests/parser/TestParser.cpp
)

target_link_libraries(LexerTests GTest::gtest_main Lexer)

target_link_libraries(ParserTests GTest::gtest_main Lexer Ast Parser)

include(GoogleTest)
gtest_discover_tests(LexerTests)
gtest_discover_tests(ParserTests)


# Optional: Add as a test target
# add_test(NAME LexerTests COMMAND LexerTests)
# add_test(NAME ParserTest COMMAND ParserTest)
