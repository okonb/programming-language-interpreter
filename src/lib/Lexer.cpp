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
bool Lexer<>::advance_character(){
    input_stream >> current_symbol;
    if(!input_stream.eof()){
        current_position.column++;
        return true;
    }
    return false;
}

template<typename T>
bool Lexer<T>::try_build_operator(){
    if(two_char_operators.contains(current_symbol)){
        T temp = current_symbol;
        Position temp_pos = current_position;
        advance_character();
        if(two_char_operators.at(temp) == current_symbol && !input_stream.eof()){
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
bool Lexer<>::try_build_number(){
    if(!std::isdigit(static_cast<unsigned char>(current_symbol))){
        return false;
    }
    Position start_position = current_position;
    uint64_t integer_part = 0;
    if(current_symbol != '0'){
        uint64_t previous;
        integer_part = current_symbol - '0';
        while(advance_character() && std::isdigit(static_cast<unsigned char>(current_symbol))){
            previous = integer_part;
            integer_part = 10 * integer_part + (current_symbol - '0');
            if(integer_part > INT64_MAX || integer_part < previous){
                token = Token(TokenType::Error_token, start_position, "Error building number - integer part digit string too long.");
                return true;
            }
        }
    }
    else{
        if(!advance_character()){
            token = Token(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
            return true;
        }
    }

    if(current_symbol == '.'){
        uint64_t decimal_part = 0;
        uint64_t previous;
        int decimal_places = 0;
        while(advance_character() && std::isdigit(static_cast<unsigned char>(current_symbol))){
            previous = decimal_part;
            decimal_part = 10 * decimal_part + (current_symbol - '0');
            decimal_places++;
            if(decimal_part < previous){
                token = Token(TokenType::Error_token, start_position, "Error building number - floating part digit string too long.");
                return true;
            }
        }
        if(decimal_places == 0){
            token = Token(TokenType::Error_token, start_position, "Error building number - floating part does not exist.");
            return true;
        }
        token = Token(TokenType::Floating_literal, start_position, integer_part + static_cast<double>(decimal_part) / pow(10, decimal_places));
        return true;
    }
    
    token = Token(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
    return true;
}

//TODO
template<typename T>
void Lexer<T>::skip_whitespace(){
    if(input_stream.eof())
        return;
    while(std::isspace(static_cast<unsigned char>(current_symbol))){
        if(std::isblank(static_cast<unsigned char>(current_symbol))){
            if(!advance_character())
                return;
            continue;
        }
        if(!newline_sequence)
            detect_newline_sequence();
        else{
            for(auto iter = newline_sequence->begin(); iter != newline_sequence->end(); ++iter){
                advance_character();
            }
            if(input_stream.eof())
                return;
        }
    }
}

template<>
void Lexer<>::detect_newline_sequence(){
    std::stringstream s;
    if(!std::isspace(static_cast<unsigned char>(current_symbol)) && std::isblank(static_cast<unsigned char>(current_symbol))){
        s << current_symbol;
        if(!advance_character()){
            newline_sequence = s.str();
            return;
        }
    }
    if(!std::isspace(static_cast<unsigned char>(current_symbol)) && std::isblank(static_cast<unsigned char>(current_symbol)) && current_symbol != s.str()[0]){
        s << current_symbol;
        advance_character();
        newline_sequence = s.str();
        return;
    }
    newline_sequence = s.str();
    return;
}

template<>
std::optional<Token> Lexer<>::get_current_token(){
    return token;
}


//TODO
template<>
Token Lexer<>::get_next_token(){
    skip_whitespace();
    if(input_stream.eof()){
        token = Token(TokenType::ETX_token, current_position, "");
        return *token;
    }
    if(try_build_operator()){
        return *token;
    }
    if(try_build_number()){
        return *token;
    }
    return Token(TokenType::Error_token, {}, {});
}