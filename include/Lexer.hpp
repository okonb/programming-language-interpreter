#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <cmath>
#include <exception>
#include "Token.hpp"

class TokenizationError : public std::exception {
    private:
    std::string err_msg;
    Position pos;
    public:
    explicit TokenizationError(const std::string &msg, const Position &p) noexcept :
        err_msg(msg), pos(p) {}
    virtual char const* what() const noexcept {
        return err_msg.c_str();
    }
    const Position &get_position() const noexcept { return pos; }
};

template<typename T = char>
class Lexer{
public:
    explicit Lexer(std::basic_istream<T> &in_stream) : current_position({1, 0}), start_position({1, 1}), input_stream(in_stream), newline_sequence(std::nullopt) { advance_character(); input_stream.unsetf(std::ios::skipws); };
    ~Lexer() { input_stream.setf(std::ios::skipws); }
    Token<T> get_next_token();
private:
    T current_symbol;
    Position current_position, start_position;
    std::basic_istream<T> &input_stream;
    std::optional<std::basic_string<T>> newline_sequence;
    static const std::map<std::basic_string<T>, TokenType> keyword_lookup;
    static const std::map<T, TokenType> single_char_operator_lookup;
    static const std::map<std::basic_string<T>, TokenType> two_char_operator_lookup;
    static const std::map<T, T> two_char_operators;
    static const std::map<T, T> escapes;
    std::optional<Token<T>> try_build_etx();
    std::optional<Token<T>> try_build_operator();
    std::optional<Token<T>> try_build_number();
    std::optional<Token<T>> try_build_identifier_or_keyword();
    std::optional<Token<T>> try_build_comment();
    std::optional<Token<T>> try_build_string();
    bool advance_character(uint64_t number = 1);
    bool is_current_digit();
    bool is_current_alpha();
    bool is_current_alnum();
    bool is_current_space();
    bool is_current_blank();
    void skip_whitespace();
    void consume_newline();
    void detect_newline_sequence();
    void feed_position();
};

#endif // !LEXER_HPP