#include "Lexer.hpp"


template<>
const std::map<std::basic_string<char>, TokenType> Lexer<char>::keyword_lookup{
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

template<>
const std::map<std::basic_string<wchar_t>, TokenType> Lexer<wchar_t>::keyword_lookup{
    {L"and",     TokenType::And},
    {L"or",      TokenType::Or},
    {L"not",     TokenType::Not},
    {L"true",    TokenType::True},
    {L"false",   TokenType::False},
    {L"int",     TokenType::Integer_type},
    {L"float",   TokenType::Floating_type},
    {L"str",     TokenType::String_type},
    {L"file",    TokenType::File_type},
    {L"bool",    TokenType::Bool_type},
    {L"void",    TokenType::Void_type},
    {L"return",  TokenType::Return_keywd},
    {L"fun",     TokenType::Function_keywd},
    {L"if",      TokenType::If_keywd},
    {L"else",    TokenType::Else_keywd},
    {L"while",   TokenType::While_keywd},
    {L"const",   TokenType::Const_keywd},
    {L"match",   TokenType::Match_keywd},
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

template<>
const std::map<std::basic_string<char>, TokenType> Lexer<char>::two_char_operator_lookup{
    {">=",  TokenType::Gte},
    {"<=",  TokenType::Lte},
    {"==",  TokenType::Equals},
    {"!=",  TokenType::Not_equals},
};

template<>
const std::map<std::basic_string<wchar_t>, TokenType> Lexer<wchar_t>::two_char_operator_lookup{
    {L">=",  TokenType::Gte},
    {L"<=",  TokenType::Lte},
    {L"==",  TokenType::Equals},
    {L"!=",  TokenType::Not_equals},
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
Token<T> Lexer<T>::get_next_token(){
    skip_whitespace();
    start_position = current_position;
    if(auto token = try_build_etx()){
        return *token;
    }
    if(auto token = try_build_comment()){
        return *token;
    }
    if(auto token = try_build_operator()){
        return *token;
    }
    if(auto token = try_build_string()){
        return *token;
    }
    if(auto token = try_build_number()){
        return *token;
    }
    if(auto token = try_build_identifier_or_keyword()){
        return *token;
    }
    throw TokenizationError("Illegal character sequence.", current_position);
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_etx(){
    if(!input_stream.eof()){
        return std::nullopt;
    }
    return Token<T>(TokenType::ETX_token, start_position, {});
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_operator(){
    if(auto iter = two_char_operators.find(current_symbol); iter != two_char_operators.end()){
        T temp = current_symbol;
        if(advance_character() && iter->second == current_symbol){
            std::basic_stringstream<T> s{};
            s << temp << current_symbol;
            return Token<T>(two_char_operator_lookup.at(s.str()), start_position, {});
        }
        if(auto iter2 = single_char_operator_lookup.find(temp); iter2 != single_char_operator_lookup.end()){
            return Token<T>(iter2->second, start_position, {});
        }
        throw TokenizationError("Bad multicharacter operator.", current_position);
    }
    if(auto iter3 = single_char_operator_lookup.find(current_symbol); iter3 != single_char_operator_lookup.end()){
        advance_character();
        return Token<T>(iter3->second, start_position, {});
    }
    return std::nullopt;
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_number(){
    if(!is_current_digit()){
        return std::nullopt;
    }
    uint64_t integer_part = 0;
    const int number_base = 10;
    if(current_symbol != '0'){
        integer_part = current_symbol - '0';
        while(advance_character() && is_current_digit()){
            if((std::numeric_limits<int64_t>::max() - (current_symbol - '0')) / double(number_base) < integer_part){
                throw TokenizationError("Error building number - integer part digit string too long.", start_position);
            }
            integer_part = number_base * integer_part + (current_symbol - '0');
        }
    }
    else if(!advance_character()){
        return Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
    }
    else if(is_current_digit()){
        throw TokenizationError("Error - numeric literal must not start with a zero.", start_position);
    }

    if(is_current_alpha() || current_symbol == '_'){
        throw TokenizationError("Error - integer numeral ending with character OR identifier starting with a number.", start_position);
    }

    if(current_symbol == '.'){
        uint64_t decimal_part = 0;
        int decimal_places = 0;
        while(advance_character() && is_current_digit()){
            if((std::numeric_limits<int64_t>::max() - (current_symbol - '0')) / static_cast<double>(number_base) < integer_part){
                throw TokenizationError("Error building number - floating part digit string too long.", start_position);
            }
            decimal_part = number_base * decimal_part + (current_symbol - '0');
            decimal_places++;
        }
        if(decimal_places == 0){
            throw TokenizationError("Error building number - floating part does not exist.", start_position);
        }
        if(current_symbol == '.' || is_current_alpha() || current_symbol == '_'){
            throw TokenizationError("Error building number - wrong floating point literal format.", current_position);
        }
        return Token<T>(TokenType::Floating_literal, start_position, integer_part + static_cast<double>(decimal_part) / pow(number_base, decimal_places));
    }
    return Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_identifier_or_keyword(){
    if(!is_current_alpha()){
        return std::nullopt;
    }

    std::basic_stringstream<T> s{};
        s << current_symbol;
    while(advance_character() && (is_current_alnum() || current_symbol == '_' )){
        s << current_symbol;
    }
    if(auto iter = keyword_lookup.find(s.str()); iter != keyword_lookup.end()){
        return Token<T>(iter->second, start_position, {});
    }
    return Token<T>(TokenType::Identifier, start_position, s.str());
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_comment(){
    if(current_symbol != '#'){
        return std::nullopt;
    }
    std::basic_stringstream<T> s{};
    while(advance_character() && (!is_current_space() || is_current_blank())){
        s << current_symbol;
    }
    return Token<T>(TokenType::Comment, start_position, s.str());
}

template<typename T>
std::optional<Token<T>> Lexer<T>::try_build_string(){
    if(current_symbol != '\"'){
        return std::nullopt;
    }
    std::basic_stringstream<T> s{};
    while(advance_character() && (!is_current_space() || is_current_blank()) && current_symbol != '\"'){
        if(current_symbol == '\\'){
            if(!advance_character()){
                throw TokenizationError("Error when building string literal - ETX.", current_position);
            }
            if(auto iter = escapes.find(current_symbol); iter != escapes.end()){
                s << iter->second;
            }
            else{
                throw TokenizationError("Error when building string literal - invalid escaped character.", current_position);
            }
        }
        else{
            s << current_symbol;
        }
    }
    if(current_symbol != '\"'){
        throw TokenizationError("Error when building string literal - newline before ending quotation mark.", current_position);
    }
    advance_character();
    return Token<T>(TokenType::String_literal, start_position, s.str());
}

template<>
bool Lexer<char>::is_current_digit(){
    return static_cast<bool>(std::isdigit(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_alpha(){
    return static_cast<bool>(std::isalpha(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_alnum(){
    return static_cast<bool>(std::isalnum(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_space(){
    return static_cast<bool>(std::isspace(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_blank(){
    return static_cast<bool>(std::isblank(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<wchar_t>::is_current_digit(){
    return static_cast<bool>(std::iswdigit(current_symbol));
}

template<>
bool Lexer<wchar_t>::is_current_alpha(){
    return static_cast<bool>(std::iswalpha(current_symbol));
}

template<>
bool Lexer<wchar_t>::is_current_alnum(){
    return static_cast<bool>(std::iswalnum(current_symbol));
}

template<>
bool Lexer<wchar_t>::is_current_space(){
    return static_cast<bool>(std::iswspace(current_symbol));
}

template<>
bool Lexer<wchar_t>::is_current_blank(){
    return static_cast<bool>(std::iswblank(current_symbol));
}

template<typename T>
bool Lexer<T>::advance_character(uint64_t number){
    for(auto i = static_cast<uint64_t>(0); i < number; ++i){
        input_stream >> current_symbol;
        if(input_stream.eof()){
            return false;
        }
        current_position.column++;
    }
    return true;
}

template<typename T>
void Lexer<T>::skip_whitespace(){
    if(input_stream.eof()){
        return;
    }

    while(is_current_space()){
        if(is_current_blank()){
            if(!advance_character()){
                return;
            }
        }
        else if(!newline_sequence){
            detect_newline_sequence();
        }
        else{
            consume_newline();
            if(input_stream.eof()){
                return;
            }
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
    std::basic_stringstream<T> s{};
    if(is_current_space() && !is_current_blank()){
        s << current_symbol;
        if(!advance_character()){
            newline_sequence = s.str();
            feed_position();
            return;
        }
    }
    if(is_current_space() && !is_current_blank() && current_symbol != s.str()[0]){
        s << current_symbol;
        advance_character();
        newline_sequence = s.str();
        feed_position();
        return;
    }
    feed_position();
    newline_sequence = s.str();
}

template<typename T>
void Lexer<T>::feed_position(){
    current_position.line++;
    current_position.column = 1;
}

template class Lexer<char>;
template class Lexer<wchar_t>;