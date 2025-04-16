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
};

inline std::unordered_map<std::string, TokenType> Keywords = {
    {"int", TokenType::kInt},
    {"=", TokenType::kEqual},
    {";", TokenType::kSemiColon},
};

struct Token{
    TokenType type;
    std::string value;
};