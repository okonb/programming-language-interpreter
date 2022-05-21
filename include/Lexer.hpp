#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include "CharType.hpp"
#include "ILexer.hpp"
#include <map>
#include <optional>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <utility>

template<CharType T>
class TokenizationError : public std::runtime_error {
public:
    explicit TokenizationError(std::basic_string<T> msg, const Position &p) noexcept :
        std::runtime_error{msg}, pos(p) {}

    [[nodiscard]] const Position &get_position() const noexcept { return pos; }
private:
    Position pos;
};

template<CharType T = char>
class Lexer : public ILexer<T> {
public:
    explicit Lexer(std::basic_istream<T> &in_stream) : current_position({1, 0}), start_position({1, 1}), input_stream(in_stream), newline_sequence(std::nullopt) { advance_character(); input_stream.unsetf(std::ios::skipws); };
    ~Lexer() { input_stream.setf(std::ios::skipws); }
    Token<T> get_next_token() override;
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

#endif // LEXER_HPP