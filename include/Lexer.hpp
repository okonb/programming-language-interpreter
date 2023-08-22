#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include "CharType.hpp"
#include "ILexer.hpp"
#include "light_map.hpp"
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string_view>

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
    explicit Lexer(std::basic_istream<T> &in_stream) :
        current_position({1, 0}), start_position({1, 1}), input_stream(in_stream), newline_sequence(std::nullopt) { 
        
        advance_character();
        input_stream.unsetf(std::ios::skipws);
    };
    ~Lexer() { input_stream.setf(std::ios::skipws); }

    Token<T> get_next_token() override;
    static const std::basic_string_view<T> get_token_text(const TokenType t) {return token_to_text.at(t);}

private:
    std::optional<Token<T>> try_build_etx();
    std::optional<Token<T>> try_build_operator();
    std::optional<Token<T>> try_build_number();
    std::optional<Token<T>> try_build_identifier_or_keyword();
    std::optional<Token<T>> try_build_comment();
    std::optional<Token<T>> try_build_string();
    void skip_whitespace();
    void consume_newline();
    void detect_newline_sequence();
    void feed_line();
    bool advance_character();
    bool is_current_digit() const;
    bool is_current_alpha() const;
    bool is_current_alnum() const;
    bool is_current_space() const;
    bool is_current_blank() const;
    bool is_current_underscore() const;
    bool is_current_newline() const;

private:
    T current_symbol;
    Position current_position, start_position;
    std::basic_istream<T> &input_stream;
    std::optional<std::basic_string<T>> newline_sequence;
    
    static const light_map<TokenType, std::basic_string_view<T>, 46UL> token_to_text;
    static const light_map<std::basic_string_view<T>, TokenType, 18UL> keyword_lookup;
    static const light_map<T, TokenType, 17UL> single_char_operator_lookup;
    static const light_map<std::basic_string_view<T>, TokenType, 4UL> two_char_operator_lookup;
    static const light_map<T, T, 4UL> two_char_operators;
    static const light_map<T, T, 10UL> escapes;

};

#endif // LEXER_HPP