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
    explicit Lexer(std::basic_istream<T> &in_stream) : input_stream(in_stream), newline_sequence(std::nullopt), current_position({1, 0}) { this->advance_character(); };
    static const T ETX_CHAR = '\x03';
    std::optional<Token> get_current_token();
    Token get_next_token();
private:
    std::basic_istream<T> &input_stream;
    T current_symbol;
    bool advance_character();
    std::optional<Token> token;
    std::optional<std::string> newline_sequence;
    Position current_position;
    static const std::map<std::string, TokenType> keyword_lookup;
    static const std::map<char, TokenType> single_char_operator_lookup;
    static const std::map<std::string, TokenType> two_char_operator_lookup;
    static const std::map<char, char> two_char_operators;
    bool try_build_operator();
    bool try_build_number();
    void skip_whitespace();
    void detect_newline_sequence();
};

#endif // !LEXER_HPP