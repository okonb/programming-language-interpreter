#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "CharType.hpp"
#include "Position.hpp"
#include <variant>
#include <cstdint>
#include <string>
#include <stdexcept>

enum class TokenType{
    ETX_token,
    Integer_literal,
    Floating_literal,
    String_literal,
    Boolean_literal,
    Identifier,
    Comment,
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
};


class TokenTypeValueMismatch : public std::runtime_error {
public:
    TokenTypeValueMismatch() : std::runtime_error{"Error: mismatch between token type and its value."} {}
};

template<CharType T>
using token_value_t = std::variant<std::monostate, std::basic_string<T>, int64_t, double, bool>;

template<CharType T = char>
class Token{
public:
    Token(TokenType t, const Position &p, token_value_t<T> v) : 
        type(t), position(p), value(std::move(v)) {
        if( (std::holds_alternative<int64_t>(value) && type != TokenType::Integer_literal)  ||
            (std::holds_alternative<double>(value) && type != TokenType::Floating_literal)  ||
            (std::holds_alternative<bool>(value) && type != TokenType::Boolean_literal)     ||
            (std::holds_alternative<std::basic_string<T>>(value) && !(
                type == TokenType::String_literal   ||
                type == TokenType::Comment          ||
                type == TokenType::Identifier)))
        {
            throw TokenTypeValueMismatch();
        }
    }
    TokenType get_type() const { return type; }
    const Position &get_position() const { return position; }
    const token_value_t<T> &get_value() const { return value; }
private:
    TokenType type;
    Position position;
    token_value_t<T> value;
};

#endif // TOKEN_HPP