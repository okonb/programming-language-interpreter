#include "Lexer.hpp"


template<typename T>
const std::map<std::basic_string<T>, TokenType> Lexer<T>::keyword_lookup{
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
const std::map<T, TokenType> Lexer<T>::single_char_operator_lookup{
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
const std::map<T, T> Lexer<T>::two_char_operators{
    {'>', '='},
    {'<', '='},
    {'=', '='},
    {'!', '='},
};

template<typename T>
const std::map<std::basic_string<T>, TokenType> Lexer<T>::two_char_operator_lookup{
    {">=",  TokenType::Gte},
    {"<=",  TokenType::Lte},
    {"==",  TokenType::Equals},
    {"!=",  TokenType::Not_equals},
};

template<typename T>
const std::map<T, T> Lexer<T>::escapes{
    {'a', '\a'},
    {'b', '\b'},
    {'f', '\f'},
    {'n', '\n'},
    {'r', '\r'},
    {'t', '\t'},
    {'v', '\v'},
    {'\\', '\\'},
    {'\"', '\"'},
    {'\?', '\?'},  
};

template<typename T>
const std::optional<Token<T>>& Lexer<T>::get_current_token() const{
    return token;
}

template<typename T>
const Token<T>& Lexer<T>::get_next_token(){
    skip_whitespace();
    if(try_build_etx()){
        return *token;
    }
    if(try_build_comment()){
        return *token;
    }
    if(try_build_operator()){
        return *token;
    }
    if(try_build_string()){
        return *token;
    }
    if(try_build_number()){
        return *token;
    }
    if(try_build_identifier_or_keyword()){
        return *token;
    }
    token = Token<T>(TokenType::Error_token, current_position, "Illegal character sequence.");
    return *token;
}

template<typename T>
bool Lexer<T>::try_build_etx(){
    if(!input_stream.eof())
        return false;
    token = Token<T>(TokenType::ETX_token, current_position, "");
    return true;
}

template<typename T>
bool Lexer<T>::try_build_operator(){
    Position start_position = current_position;
    if(two_char_operators.contains(current_symbol)){
        T temp = current_symbol;
        advance_character();
        if(two_char_operators.at(temp) == current_symbol && !input_stream.eof()){
            std::basic_stringstream<T> stream;
            stream << temp << current_symbol;
            token = Token<T>(two_char_operator_lookup.at(stream.str()), start_position, {});
            return true;
        }
        else if(single_char_operator_lookup.contains(temp)){
            token = Token<T>(single_char_operator_lookup.at(temp), start_position, {});
            return true;
        }
        else{
            token = Token<T>(TokenType::Error_token, start_position, "Bad multicharacter operator.");
            return true;
        }
    }
    else if(single_char_operator_lookup.contains(current_symbol)){
        token = Token<T>(single_char_operator_lookup.at(current_symbol), start_position, {});
        advance_character();
        return true;
    }
    else{
        return false;
    }
}

template<typename T>
bool Lexer<T>::try_build_number(){
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
                token = Token<T>(TokenType::Error_token, start_position, "Error building number - integer part digit string too long.");
                return true;
            }
        }
    }
    else{
        if(!advance_character()){
            token = Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
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
                token = Token<T>(TokenType::Error_token, start_position, "Error building number - floating part digit string too long.");
                return true;
            }
        }
        if(decimal_places == 0){
            token = Token<T>(TokenType::Error_token, start_position, "Error building number - floating part does not exist.");
            return true;
        }
        token = Token<T>(TokenType::Floating_literal, start_position, integer_part + static_cast<double>(decimal_part) / pow(10, decimal_places));
        return true;
    }
    
    token = Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
    return true;
}

template<typename T>
bool Lexer<T>::try_build_identifier_or_keyword(){
    if(!std::isalpha(static_cast<unsigned char>(current_symbol)))
        return false;
    std::basic_stringstream<T> s;
    Position start_position = current_position;
    s << current_symbol;
    while(advance_character() && (
        std::isalnum(static_cast<unsigned char>(current_symbol)) ||
        current_symbol == '_'
    )){
        s << current_symbol;
    }
    if(keyword_lookup.contains(s.str())){
        token = Token<T>(keyword_lookup.at(s.str()), start_position, {});
    }
    else{
        token = Token<T>(TokenType::Identifier, start_position, s.str());
    }
    return true;
}

template<typename T>
bool Lexer<T>::try_build_comment(){
    if(current_symbol != '#')
        return false;
    Position start_position = current_position;
    std::basic_stringstream<T> s;
    while(advance_character() && (
        !std::isspace(static_cast<unsigned char>(current_symbol)) ||
        std::isblank(static_cast<unsigned char>(current_symbol))
    )){
        s << current_symbol;
    }
    token = Token<T>(TokenType::Comment, start_position, s.str());
    return true;
}

template<typename T>
bool Lexer<T>::try_build_string(){
    if(current_symbol != '\"')
        return false;
    Position start_position = current_position;
    std::basic_stringstream<T> s;
    while(advance_character() && (
        !std::isspace(static_cast<unsigned char>(current_symbol)) ||
        std::isblank(static_cast<unsigned char>(current_symbol))
    )){
        if(current_symbol == '\\'){
            if(!advance_character()){
                token = Token<T>(TokenType::Error_token, current_position, "Error when building string literal - ETX.");
                return true;
            }
            if(!escapes.contains(current_symbol)){
                token = Token<T>(TokenType::Error_token, current_position, "Error when building string literal - invalid escaped character.");
                return true;
            }
            s << escapes.at(current_symbol);
        }
        
        else if(current_symbol == '\"'){
            break;
        }
        else{
            s << current_symbol;
        }
    }
    if(current_symbol != '\"'){
        token = Token<T>(TokenType::Error_token, current_position, "Error when building string literal - newline before ending quotation mark.");
        return true;
    }
    advance_character();
    token = Token<T>(TokenType::String_literal, start_position, s.str());
    return true;
}

template<typename T>
bool Lexer<T>::advance_character(){
    input_stream >> current_symbol;
    if(!input_stream.eof()){
        current_position.column++;
        return true;
    }
    return false;
}

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
        else if(!newline_sequence)
            detect_newline_sequence();
        else{
            consume_newline();
            if(input_stream.eof())
                return;
        }
    }
}

template<typename T>
void Lexer<T>::consume_newline(){
    for(auto iter = newline_sequence->begin(); iter != newline_sequence->end(); ++iter){
        advance_character();
    }
    feed_position();
}

template<typename T>
void Lexer<T>::detect_newline_sequence(){
    std::basic_stringstream<T> s;
    if(std::isspace(static_cast<unsigned char>(current_symbol)) && !std::isblank(static_cast<unsigned char>(current_symbol))){
        s << current_symbol;
        if(!advance_character()){
            newline_sequence = s.str();
            feed_position();
            return;
        }
    }
    if(std::isspace(static_cast<unsigned char>(current_symbol)) && !std::isblank(static_cast<unsigned char>(current_symbol)) && current_symbol != s.str()[0]){
        s << current_symbol;
        advance_character();
        newline_sequence = s.str();
        feed_position();
        return;
    }
    feed_position();
    newline_sequence = s.str();
    return;
}

template<typename T>
void Lexer<T>::feed_position(){
    current_position.line++;
    current_position.column = 1;
}

template class Lexer<char>;