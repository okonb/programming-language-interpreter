#ifndef PARSER_HPP
#define PARSER_HPP

#include "CharType.hpp"
#include "ILexer.hpp"
#include "IInstruction.hpp"
#include "IExpression.hpp"
#include "TypeIdentifier.hpp"
#include "ParameterDefinition.hpp"
#include "MatchLine.hpp"
#include "light_map.hpp"
#include <stdexcept>
#include <vector>
#include <initializer_list>
#include <source_location>


template<CharType T>
class UnexpectedTokenException : public std::runtime_error{
public:
    UnexpectedTokenException(const std::basic_string_view<T> &function_name, const Token<T> &t, const std::initializer_list<TokenType> &types) :
        std::runtime_error{"UnexpectedTokenException"}, function_throwing{function_name}, token{t}, expected_tokens{types} {}
    const std::basic_string<T> &get_function_throwing_name() const {return function_throwing;}
    const Token<T> &get_received_token() const {return token;}
    const std::vector<TokenType> &get_expected_token_list() const {return expected_tokens;}
private:
    const std::basic_string<T> function_throwing;
    const Token<T> token;
    const std::vector<TokenType> expected_tokens;
};

template<CharType T>
class SyntaxErrorException : public std::runtime_error{
public:
    SyntaxErrorException(const std::basic_string_view<T> &function_name, const Token<T> &t, const std::basic_string_view<T> &e) :
        std::runtime_error{"SyntaxErrorException"}, function_throwing{function_name}, token{t}, expected{e} {}
    const std::basic_string<T> &get_function_throwing_name() const {return function_throwing;}
    const Token<T> &get_received_token() const {return token;}
    const std::basic_string<T> &get_expected_production() const {return expected;}
private:
    const std::basic_string<T> function_throwing;
    const Token<T> token;
    const std::basic_string<T> expected;
};

template <CharType T = char>
class ParserBase{
public:
    ParserBase(ILexer<T> &l) : lexer{l}, current_token(l.get_next_token()) {}
    std::unique_ptr<FunctionDefinition<T>> try_parse_function_definition();
    std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>>  try_parse_parameter_list_definition();
    std::unique_ptr<ParameterDefinition<T>> try_parse_parameter_definition();
    std::unique_ptr<TypeIdentifier<T>> try_parse_type_identifier();
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> try_parse_code_block();
    std::unique_ptr<IInstruction<T>> try_parse_statement_or_control_block();
    std::unique_ptr<IInstruction<T>> try_parse_statement();
    std::unique_ptr<IInstruction<T>> try_parse_control_block();
    std::unique_ptr<IInstruction<T>> try_parse_if_block();
    std::unique_ptr<IInstruction<T>> try_parse_while_block();
    std::unique_ptr<IInstruction<T>> try_parse_var_def_assign_or_funcall();
    std::unique_ptr<IInstruction<T>> try_parse_return_statement();
    std::unique_ptr<IInstruction<T>> try_parse_else_block();
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> try_parse_parenths_and_args();
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> try_parse_argument_list();
    std::unique_ptr<IExpression<T>> try_parse_expression();
    std::unique_ptr<IExpression<T>> try_parse_logic_factor();
    std::unique_ptr<IExpression<T>> try_parse_relation();
    std::unique_ptr<IExpression<T>> try_parse_math_expression();
    std::unique_ptr<IExpression<T>> try_parse_factor();
    std::unique_ptr<IExpression<T>> try_parse_term();
    std::unique_ptr<IExpression<T>> try_parse_match_expression();
    std::unique_ptr<IExpression<T>> try_parse_match_logic_factor();
    std::unique_ptr<IExpression<T>> try_parse_match_relation();
    std::unique_ptr<IExpression<T>> try_parse_match_math_expression();
    std::unique_ptr<IExpression<T>> try_parse_match_factor();
    std::unique_ptr<IExpression<T>> try_parse_match_term();
    std::unique_ptr<IExpression<T>> try_parse_literal();
    std::unique_ptr<IExpression<T>> try_parse_identifier_or_funcall();
    std::unique_ptr<IExpression<T>> try_parse_match_operation();
    std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> try_parse_match_block();
    std::unique_ptr<MatchLine<T>> try_parse_match_line();
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> try_parse_pattern();
    std::unique_ptr<IExpression<T>> try_parse_pattern_element();
    std::unique_ptr<IExpression<T>> try_parse_condition();

    bool is_current_token_of_type(TokenType type) const;
    bool is_current_token_a_type() const;
    bool is_current_token_additive_operator() const;
    bool is_current_token_multiplicative_operator() const;
    bool is_current_token_relation_operator() const;
    bool is_current_token_a_literal() const;
    std::basic_string<T> get_string_from_current() const;
    Position get_current_position() const;
    void advance_token();
    bool check_and_advance(TokenType type);
    void expect(TokenType type, const std::source_location &location = std::source_location::current()) const;
    void expect_and_advance(TokenType type, const std::source_location &location = std::source_location::current());
    template<typename P>
    void expect_not_null(std::unique_ptr<P> &pointer, const std::basic_string_view<T> err_message, const std::source_location &location = std::source_location::current()) const;
    
    UnexpectedTokenException<T> get_unexpected_token_exception(const std::initializer_list<TokenType> &types, const std::source_location &location = std::source_location::current()) const;
    SyntaxErrorException<T> get_syntax_error_exception(const std::basic_string_view<T> &text, const std::source_location &location = std::source_location::current()) const;
    
    Type map_to_type(TokenType type) const;
    ExpressionType map_to_expression_type(TokenType type) const;
    ExpressionType map_to_match(ExpressionType type) const;
    ExpressionType map_current_to_expression_type() const;
    ExpressionType map_current_to_match_expression_type() const;

private:
    ILexer<T> &lexer;
    Token<T> current_token;

    const static light_map<TokenType, Type, 6UL> type_map;
    const static light_map<TokenType, ExpressionType, 19UL> expression_type_map;
    const static light_map<ExpressionType, ExpressionType, 15UL> match_expression_type_map;
};


template<CharType T = char>
class Parser : private ParserBase<T>{
public:
    Parser(ILexer<T> &l) : ParserBase<T>{l} {}
    std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> parse();
};


#endif // PARSER_HPP