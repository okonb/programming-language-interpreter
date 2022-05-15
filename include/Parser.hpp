#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "Program.hpp"
#include "Type.hpp"



template <typename T = char>
class Parser{
public:
    Parser(Lexer<T> &l) : lexer{l}, current_token(l.get_next_token()), function_definitions{} {}
    Program<T> parse();

private:
    bool try_parse_function_definition();
    bool is_current_token_of_type(TokenType type) const;
    bool check_and_advance(TokenType type);
    bool is_current_token_a_type() const;
    Lexer<T> &lexer;
    Token<T> current_token;
    FunctionDefinitionsMap<T> function_definitions;
    Type map_type(TokenType type) const;
    void get_next_token();
    static std::map<TokenType, Type> type_map;
};



#endif // PARSER_HPP