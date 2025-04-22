#pragma once

#include <string>
#include <unordered_map>

enum class TokenType{
    kString,
    kInt,
    kVariable,
    kEqual,
    kSemiColon,
    kNumber,
    kReturn,
    kLeftParenthesis,
    kRightParenthesis,
    kRightBracket,
    kLeftBracket,
    kMain,
    kIllegal,
};

inline std::unordered_map<std::string, TokenType> Keywords = {
    {"int", TokenType::kInt},
    {"return", TokenType::kReturn},
    {"main", TokenType::kMain},
};

struct Token{
    TokenType type;
    std::string value;
};
