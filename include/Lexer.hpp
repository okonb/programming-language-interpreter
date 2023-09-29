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
    TokenizationError(const std::basic_string<T> &msg, const Position &p) :
        std::runtime_error{msg}, pos(p) {}

    [[nodiscard]] const Position &get_position() const noexcept { return pos; }
private:
    Position pos;
};

template<CharType T = char>
class Lexer : public ILexer<T> {
public:
    explicit Lexer(std::basic_istream<T> &in_stream);
    ~Lexer() override;

    Token<T> get_next_token() override;
    static std::basic_string_view<T> get_token_text(const TokenType t);

private:
    std::optional<Token<T>> try_build_etx();
    std::optional<Token<T>> try_build_operator();
    std::optional<Token<T>> try_build_number();
    std::optional<Token<T>> try_build_identifier_or_keyword();
    std::optional<Token<T>> try_build_comment();
    std::optional<Token<T>> try_build_string();
    bool advance_character();
    void skip_whitespace();
    void consume_newline();
    void detect_newline_sequence();
    void feed_line() noexcept;
    bool is_current_digit() const noexcept;
    bool is_current_alpha() const noexcept;
    bool is_current_alnum() const noexcept;
    bool is_current_space() const noexcept;
    bool is_current_blank() const noexcept;
    bool is_current_underscore() const noexcept;
    bool is_current_newline() const noexcept;

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