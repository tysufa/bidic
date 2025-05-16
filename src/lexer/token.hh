#pragma once

#include <string>
#include <unordered_map>

enum class TokenType {
  // C tokens
  kString,
  kInt,
  kEqual,
  kSemiColon,
  kLeftParenthesis,
  kRightParenthesis,
  kRightBracket,
  kLeftBracket,
  kMain,
  kPlus,
  kStar,
  kSlash,
  kTilde,
  kDecrement,

  // C and Asm tokens
  kIdentifier,
  kMinus,
  kReturn,
  kNumber,

  // Asm tokens
  kNL,
  kMov,
  kImul,
  kAdd,
  kNeg,
  kComma,
  kLeftSquareBracket,
  kRightSquareBracket,
  kRegister,
  kColon,

  // Other
  kEof,
  kIllegal,
};

inline std::string StringTokenType(const TokenType& tok) {
  switch (tok) {
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

    case TokenType::kPlus:
      return "kPlus";
      break;

    case TokenType::kMinus:
      return "kMinus";
      break;

    case TokenType::kStar:
      return "kStar";
      break;

    case TokenType::kSlash:
      return "kSlash";
      break;

    case TokenType::kTilde:
      return "kTilde";
      break;

    case TokenType::kDecrement:
      return "kDecrement";
      break;

    case TokenType::kNL:
      return "\\n";
      break;

    case TokenType::kMov:
      return "kMov";
      break;

    case TokenType::kImul:
      return "kImul";
      break;

    case TokenType::kAdd:
      return "kAdd";
      break;

    case TokenType::kNeg:
      return "kNeg";
      break;

    case TokenType::kComma :
      return "kComma" ;
      break;

    case TokenType::kLeftSquareBracket :
      return "kLeftSquareBracket";
      break;

    case TokenType::kRightSquareBracket :
      return "kRightSquareBracket";
      break;

    case TokenType::kRegister :
      return "kRegister";
      break;

    case TokenType::kColon :
      return "kColon";
      break;
  }

  return "";
}

inline std::unordered_map<std::string, TokenType> CKeywords = {
  {"int", TokenType::kInt},
  {"return", TokenType::kReturn},
  {"main", TokenType::kMain},
};

inline std::unordered_map<std::string, TokenType> AsmKeywords = {
  {"mov", TokenType::kMov},
  {"imul", TokenType::kImul},
  {"neg", TokenType::kNeg},
  {"add", TokenType::kAdd},
  {"ret", TokenType::kReturn},
  {"eax", TokenType::kRegister},
  {"ebx", TokenType::kRegister},
  {"ecx", TokenType::kRegister},
  {"edx", TokenType::kRegister},
  {"ebp", TokenType::kRegister},
  {"esp", TokenType::kRegister},
  {"esi", TokenType::kRegister},
  {"edi", TokenType::kRegister},
};

struct Token {
  TokenType type;
  std::string value;
};
