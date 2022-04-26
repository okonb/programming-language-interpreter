#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <cmath>
#include "Token.hpp"

template<typename T = char>
class Lexer{
public:
    Lexer() = delete;
    explicit Lexer(std::basic_istream<T> &in_stream) : current_position({1, 0}), input_stream(in_stream), newline_sequence(std::nullopt) { advance_character(); input_stream.unsetf(std::ios::skipws); };
    const std::optional<Token<T>> &get_current_token() const;
    const Token<T> &get_next_token();
private:
    T current_symbol;
    Position current_position;
    std::basic_istream<T> &input_stream;
    std::optional<Token<T>> token;
    std::optional<std::basic_string<T>> newline_sequence;
    static const std::map<std::basic_string<T>, TokenType> keyword_lookup;
    static const std::map<T, TokenType> single_char_operator_lookup;
    static const std::map<std::basic_string<T>, TokenType> two_char_operator_lookup;
    static const std::map<T, T> two_char_operators;
    static const std::map<T, T> escapes;
    bool try_build_etx();
    bool try_build_operator();
    bool try_build_number();
    bool try_build_identifier_or_keyword();
    bool try_build_comment();
    bool try_build_string();
    bool advance_character();
    void skip_whitespace();
    void consume_newline();
    void detect_newline_sequence();
    void feed_position();
};

#endif // !LEXER_HPP