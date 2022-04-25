#include "Lexer.hpp"


template<typename T>
const std::map<std::string, TokenType> Lexer<T>::keyword_lookup{
    {"and",     TokenType::And},
    {"or",      TokenType::Or},
    {"not",     TokenType::Not},
    {"true",    TokenType::True},
    {"false",   TokenType::False},
    {"int",     TokenType::Integer_type},
    {"float",   TokenType::Floating_type},
    {"str",     TokenType::String_type},
    {"file",    TokenType::File_type},
    {"bool",    TokenType::Bool_type},
    {"void",    TokenType::Void_type},
    {"return",  TokenType::Return_keywd},
    {"fun",     TokenType::Function_keywd},
    {"if",      TokenType::If_keywd},
    {"else",    TokenType::Else_keywd},
    {"while",   TokenType::While_keywd},
    {"const",   TokenType::Const_keywd},
    {"match",   TokenType::Match_keywd},
};

template<typename T>
const std::map<char, TokenType> Lexer<T>::single_char_operator_lookup{
    {'(',   TokenType::Opening_parenth},
    {')',   TokenType::Closing_parenth},
    {'{',   TokenType::Opening_curly},
    {'}',   TokenType::Closing_curly},
    {':',   TokenType::Colon},
    {';',   TokenType::Semicolon},
    {',',   TokenType::Comma},
    {'_',   TokenType::Underscore},
    {'#',   TokenType::Hash},
    {'=',   TokenType::Assign},
    {'+',   TokenType::Plus},
    {'-',   TokenType::Minus},
    {'*',   TokenType::Multiplication},
    {'/',   TokenType::Division},
    {'%',   TokenType::Modulo},
    {'|',   TokenType::String_concat},
    {'>',   TokenType::Gt},
    {'<',   TokenType::Lt},
};

template<typename T>
const std::map<char, char> Lexer<T>::two_char_operators{
    {'>', '='},
    {'<', '='},
    {'=', '='},
    {'!', '='},
};

template<typename T>
const std::map<std::string, TokenType> Lexer<T>::two_char_operator_lookup{
    {">=",  TokenType::Gte},
    {"<=",  TokenType::Lte},
    {"==",  TokenType::Equals},
    {"!=",  TokenType::Not_equals},
};


template<>
void Lexer<>::advance_character(){
    input_stream >> current_symbol;
    std::cout << "advanced " << current_symbol << std::endl;
}

template<typename T>
bool Lexer<T>::try_build_operator(){
    if(two_char_operators.contains(current_symbol)){
        T temp = current_symbol;
        Position temp_pos = current_position;
        advance_character();
        if(two_char_operators.at(temp) == current_symbol){
            std::stringstream stream;
            stream << temp << current_symbol;
            token = Token(two_char_operator_lookup.at(stream.str()), current_position, {});
            return true;
        }
        else if(single_char_operator_lookup.contains(temp)){
            token = Token(single_char_operator_lookup.at(temp), temp_pos, {});
            return true;
        }
        else{
            token = Token(TokenType::Error_token, temp_pos, "Bad multicharacter operator");
            return true;
        }
    }
    else if(single_char_operator_lookup.contains(current_symbol)){
        token = Token(single_char_operator_lookup.at(current_symbol), current_position, {});
        advance_character();
        return true;
    }
    else{
        return false;
    }
}

template<>
Token Lexer<>::get_next_token(){
    if(try_build_operator()){
        return *token;
    }
    return Token(TokenType::Error_token, {}, {});
}