#ifndef COMMENT_FILTER_LEXER_HPP
#define COMMENT_FILTER_LEXER_HPP

#include "ILexer.hpp"
#include "Lexer.hpp"

template<CharType T = char>
class CommentFilterLexer : public ILexer<T>{
public:
    CommentFilterLexer(ILexer<T> &lex) : 
        lexer{lex}, current_token{Token<T>({}, {}, {})} {}
    Token<T> get_next_token() override{
        skip_comments();
        return current_token;
    }
private:
    ILexer<T> &lexer;
    Token<T> current_token;
    void skip_comments(){
        current_token = lexer.get_next_token();
        while(current_token.get_type() == TokenType::Comment){
            current_token = lexer.get_next_token();
        }
    }
};

#endif // !COMMENT_FILTER_LEXER_HPP