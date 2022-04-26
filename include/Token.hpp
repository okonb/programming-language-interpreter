#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>
#include <cstdint>
#include <string>

enum class TokenType{
    Opening_parenth,
    Closing_parenth,
    Opening_curly,
    Closing_curly,
    Colon,
    Semicolon,
    Comma,
    Underscore,
    Assign,
    Plus,
    Minus,
    Multiplication,
    Division,
    Modulo,
    String_concat,
    And,
    Or,
    Not,
    True,
    False,
    Equals,
    Not_equals,
    Lt,
    Gt,
    Lte,
    Gte,
    Integer_type,
    Floating_type,
    String_type,
    File_type,
    Bool_type,
    Void_type,
    Return_keywd,
    Function_keywd,
    If_keywd,
    Else_keywd,
    While_keywd,
    Const_keywd,
    Match_keywd,
    Integer_literal,
    Floating_literal,
    String_literal,
    Identifier,
    Comment,
    Error_token,
    ETX_token,
};

struct Position{
    int line, column;
};

template<typename T = char>
struct Token{
    Token(TokenType t, const Position &p, const std::variant<int64_t, double, std::basic_string<T>> &v) : 
        type(t), position(p), value(v) {};
    TokenType type;
    Position position;
    std::variant<int64_t, double, std::basic_string<T>> value;
};

#endif // !TOKEN_HPP