#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <variant>
#include <cstdint>
#include <string>
#include <exception>

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
    ETX_token,
};

struct Position{
    uint64_t line, column;
};

class TokenTypeValueMismatch : public std::exception {
    public:
    virtual char const* what() const noexcept {
        return "Error: mismatch between token type and its value.";
    }
};

template<typename T>
using token_value_t = std::variant<std::monostate, int64_t, double, std::basic_string<T>>;

template<typename T = char>
class Token{
public:
    explicit Token(TokenType t, const Position &p, token_value_t<T> v) : 
        type(t), position(p), value(std::move(v)) {
        if( (std::holds_alternative<int64_t>(value) && type != TokenType::Integer_literal) ||
            (std::holds_alternative<double>(value) && type != TokenType::Floating_literal) ||
            (std::holds_alternative<std::basic_string<T>>(value) && !(
                type == TokenType::String_literal ||
                type == TokenType::Comment ||
                type == TokenType::Identifier)))
        {
            throw TokenTypeValueMismatch();
        }
    }
    const TokenType &get_type() const { return type; }
    const Position &get_position() const { return position; }
    const token_value_t<T> &get_value() const { return value; }
private:
    TokenType type;
    Position position;
    token_value_t<T> value;
};

#endif // !TOKEN_HPP