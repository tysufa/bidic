#pragma once

#include <string>
#include <unordered_map>

enum class TokenType{
    kString,
    kInt,
    kIdentifier,
    kEqual,
    kSemiColon,
    kNumber,
    kReturn,
    kLeftParenthesis,
    kRightParenthesis,
    kRightBracket,
    kLeftBracket,
    kMain,
    kEof,
    kIllegal,
};

inline std::string StringTokenType(const TokenType& tok){
    switch(tok){
        case TokenType::kString:
            return "kString";
            break;
        case TokenType::kInt:
            return "kInt";
            break;
        case TokenType::kIdentifier:
            return "kVariable";
            break;
        case TokenType::kEqual:
            return "kEqual";
            break;
        case TokenType::kSemiColon:
            return "kSemiColon";
            break;
        case TokenType::kNumber:
            return "kNumber";
            break;
        case TokenType::kReturn:
            return "kReturn";
            break;
        case TokenType::kLeftParenthesis:
            return "kLeftParenthesis";
            break;
        case TokenType::kRightParenthesis:
            return "kRightParenthesis";
            break;
        case TokenType::kRightBracket:
            return "kRightBracket";
            break;
        case TokenType::kLeftBracket:
            return "kLeftBracket";
            break;
        case TokenType::kMain:
            return "kMain";
            break;
        case TokenType::kIllegal:
            return "kIllegal";
            break;
        case TokenType::kEof:
            return "kEof";
            break;
    }
    return "";
}

inline std::unordered_map<std::string, TokenType> Keywords = {
    {"int", TokenType::kInt},
    {"return", TokenType::kReturn},
    {"main", TokenType::kMain},
};

struct Token{
    TokenType type;
    std::string value;
};
