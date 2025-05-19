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

# --- Ast Library ---
add_library(Ast STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/ast.cc
)

target_include_directories(Ast PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
)


# --- Parser Library ---
add_library(Parser STATIC
  src/parser/parser.cc
  src/parser/parser_asm.cpp
)

target_include_directories(Parser PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
)

target_link_libraries(Parser PRIVATE Ast)

# --- Scug Library (Header only) ---
add_library(Scug INTERFACE)

target_include_directories(Scug INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scug
)

# --- Ast_parser Library (Header only) ---
add_library(Ast_parser STATIC
  src/ast_parser/ast_parser.cc  
  src/ast_parser/ast_parser_asm.cpp 
)

target_include_directories(Ast_parser PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast_parser
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scug
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scav
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer
)

target_link_libraries(Ast_parser PRIVATE Ast Parser)

# --- Emitor Library ---
add_library(Emitor STATIC
  ./src/emission/emission.cc
)

target_include_directories(Emitor PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/scug
  ${CMAKE_CURRENT_SOURCE_DIR}/src/emission
)

# --- Main Executable ---
add_executable(Bidic src/main.cpp)
target_link_libraries(Bidic PRIVATE Lexer Ast Ast_parser Scug Emitor)
target_include_directories(Bidic PRIVATE include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/155b337c938a2953e5675f9dc18c99f05f4c85d0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Manual Test Executable ---
add_executable(LexerTests
  tests/lexer/test_lexer.cpp
)
add_executable(ParserTests
  tests/parser/test_parser.cpp
)
add_executable(AstParserTests
  tests/ast_parser/test_ast_parser.cpp
)
add_executable(EmitorTests
  tests/emitor/test_emitor.cpp
)

target_link_libraries(LexerTests GTest::gtest_main Lexer)

target_link_libraries(ParserTests GTest::gtest_main Lexer Ast Parser)

target_link_libraries(AstParserTests GTest::gtest_main Lexer Ast Parser Scug
  Ast_parser)

target_link_libraries(EmitorTests GTest::gtest_main Lexer Ast Parser Scug Emitor
  Ast_parser)

include(GoogleTest)
gtest_discover_tests(LexerTests)
gtest_discover_tests(ParserTests)
gtest_discover_tests(AstParserTests)
gtest_discover_tests(EmitorTests)


# Optional: Add as a test target
# add_test(NAME LexerTests COMMAND LexerTests)
# add_test(NAME ParserTest COMMAND ParserTest)
