#include "Lexer.hpp"

#include <cctype>
#include <cstdint>
#include <cmath>

using std::string_view_literals::operator""sv;

template<>
constexpr light_map<TokenType, std::basic_string_view<char>, 46UL> Lexer<char>::token_to_text{
    std::array<std::pair<TokenType, std::basic_string_view<char>>, 46UL>{{
        {TokenType::And             , "and"             },
        {TokenType::Or              , "or"              },
        {TokenType::Not             , "not"             },
        {TokenType::True            , "true"            },
        {TokenType::False           , "false"           },
        {TokenType::Integer_type    , "int"             },
        {TokenType::Floating_type   , "float"           },
        {TokenType::String_type     , "str"             },
        {TokenType::File_type       , "file"            },
        {TokenType::Bool_type       , "bool"            },
        {TokenType::Void_type       , "void"            },
        {TokenType::Return_keywd    , "return"          },
        {TokenType::Function_keywd  , "fun"             },
        {TokenType::If_keywd        , "if"              },
        {TokenType::Else_keywd      , "else"            },
        {TokenType::While_keywd     , "while"           },
        {TokenType::Const_keywd     , "const"           },
        {TokenType::Match_keywd     , "match"           },
        {TokenType::Opening_parenth , "("               },
        {TokenType::Closing_parenth , ")"               },
        {TokenType::Opening_curly   , "{"               },
        {TokenType::Closing_curly   , "}"               },
        {TokenType::Colon           , ":"               },
        {TokenType::Semicolon       , ";"               },
        {TokenType::Comma           , ","               },
        {TokenType::Underscore      , "_"               },
        {TokenType::Assign          , "="               },
        {TokenType::Plus            , "+"               },
        {TokenType::Minus           , "-"               },
        {TokenType::Multiplication  , "*"               },
        {TokenType::Division        , "/"               },
        {TokenType::Modulo          , "%"               },
        {TokenType::String_concat   , "|"               },
        {TokenType::Gt              , ">"               },
        {TokenType::Lt              , "<"               },
        {TokenType::Gte             , ">="              },
        {TokenType::Lte             , "<="              },
        {TokenType::Equals          , "=="              },
        {TokenType::Not_equals      , "!="              },
        {TokenType::ETX_token       , "ETX_TOKEN"      },
        {TokenType::Integer_literal , "Integer_literal" },
        {TokenType::Floating_literal, "Floating_literal"},
        {TokenType::String_literal  , "String_literal"  },
        {TokenType::Boolean_literal , "Boolean_literal" },
        {TokenType::Identifier      , "Identifier"      },
        {TokenType::Comment         , "Comment"         },
    }}
};


template<>
constexpr light_map<std::basic_string_view<char>, TokenType, 18UL> Lexer<char>::keyword_lookup{
    std::array<std::pair<std::basic_string_view<char>, TokenType>, 18UL>{{
        {"and"sv,   TokenType::And},
        {"or"sv,    TokenType::Or},
        {"not"sv,   TokenType::Not},
        {"true"sv,  TokenType::True},
        {"false"sv, TokenType::False},
        {"int"sv,   TokenType::Integer_type},
        {"float"sv, TokenType::Floating_type},
        {"str"sv,   TokenType::String_type},
        {"file"sv,  TokenType::File_type},
        {"bool"sv,  TokenType::Bool_type},
        {"void"sv,  TokenType::Void_type},
        {"return"sv,TokenType::Return_keywd},
        {"fun"sv,   TokenType::Function_keywd},
        {"if"sv,    TokenType::If_keywd},
        {"else"sv,  TokenType::Else_keywd},
        {"while"sv, TokenType::While_keywd},
        {"const"sv, TokenType::Const_keywd},
        {"match"sv, TokenType::Match_keywd},
    }}
};

/*
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
*/

template<CharType T>
constexpr light_map<T, TokenType, 17UL> Lexer<T>::single_char_operator_lookup{
    std::array<std::pair<T, TokenType>, 17UL> {{
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
    }}
};

template<>
constexpr light_map<std::basic_string_view<char>, TokenType, 4UL> Lexer<char>::two_char_operator_lookup{
    std::array<std::pair<std::basic_string_view<char>, TokenType>, 4UL> {{
        {">="sv,    TokenType::Gte},
        {"<="sv,    TokenType::Lte},
        {"=="sv,    TokenType::Equals},
        {"!="sv,    TokenType::Not_equals},
    }}
};
/*
template<>
const std::map<std::basic_string<wchar_t>, TokenType> Lexer<wchar_t>::two_char_operator_lookup{
    {L">=",  TokenType::Gte},
    {L"<=",  TokenType::Lte},
    {L"==",  TokenType::Equals},
    {L"!=",  TokenType::Not_equals},
};
*/


template<CharType T>
constexpr light_map<T, T, 4UL> Lexer<T>::two_char_operators{
    std::array<std::pair<T, T>, 4UL>{{
        {'>', '='},
        {'<', '='},
        {'=', '='},
        {'!', '='},
    }}
};


template<CharType T>
constexpr light_map<T, T, 10UL> Lexer<T>::escapes{
    std::array<std::pair<T, T>, 10UL>{{
        {'a',   '\a'},
        {'b',   '\b'},
        {'f',   '\f'},
        {'n',   '\n'},
        {'r',   '\r'},
        {'t',   '\t'},
        {'v',   '\v'},
        {'\\',  '\\'},
        {'\"',  '\"'},
        {'\?',  '\?'},  
    }}
};


template<CharType T>
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
    throw TokenizationError<T>("Illegal character sequence.", current_position);
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_etx(){
    if(!input_stream.eof()){
        return std::nullopt;
    }
    return Token<T>(TokenType::ETX_token, start_position, {});
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_operator(){
    if(const auto iter = two_char_operators.find(current_symbol); iter != two_char_operators.cend()){
        const T first_symbol = current_symbol;
        if(advance_character() && iter->second == current_symbol){
            std::basic_stringstream<T> s{};
            s << first_symbol << current_symbol;
            advance_character();
            return Token<T>(two_char_operator_lookup.at(s.str()), start_position, {});
        }
        if(const auto iter2 = single_char_operator_lookup.find(first_symbol); iter2 != single_char_operator_lookup.cend()){
            return Token<T>(iter2->second, start_position, {});
        }
        throw TokenizationError<T>("Bad multicharacter operator.", current_position);
    }
    if(const auto iter = single_char_operator_lookup.find(current_symbol); iter != single_char_operator_lookup.cend()){
        advance_character();
        return Token<T>(iter->second, start_position, {});
    }
    return std::nullopt;
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_number(){
    if(!is_current_digit()){
        return std::nullopt;
    }
    int64_t integer_part = 0;
    constexpr int number_base = 10;
    if(current_symbol != '0'){
        integer_part = current_symbol - '0';
        while(advance_character() && is_current_digit()){
            const int current_digit = current_symbol - '0';
            if((std::numeric_limits<int64_t>::max() - current_digit) / static_cast<double>(number_base) < integer_part){
                throw TokenizationError<T>("Error building number - integer part digit string too long.", start_position);
            }
            integer_part = number_base * integer_part + current_digit;
        }
    }
    else if(!advance_character()){
        return Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
    }
    else if(is_current_digit()){
        throw TokenizationError<T>("Error - numeric literal must not start with a zero.", start_position);
    }

    if(is_current_alpha() || is_current_underscore()){
        throw TokenizationError<T>("Error - integer numeral ending with character OR identifier starting with a number.", start_position);
    }

    if(current_symbol == '.'){
        int64_t decimal_part = 0;
        int decimal_places = 0;
        while(advance_character() && is_current_digit()){
            const int current_digit = current_symbol - '0';
            if((std::numeric_limits<int64_t>::max() - current_digit) / static_cast<double>(number_base) < integer_part){
                throw TokenizationError<T>("Error building number - floating part digit string too long.", start_position);
            }
            decimal_part = number_base * decimal_part + current_digit;
            decimal_places++;
        }
        if(decimal_places == 0){
            throw TokenizationError<T>("Error building number - floating part does not exist.", start_position);
        }
        if(current_symbol == '.' || is_current_alpha() || is_current_underscore()){
            throw TokenizationError<T>("Error building number - wrong floating point literal format.", current_position);
        }
        return Token<T>(TokenType::Floating_literal, start_position, static_cast<double>(integer_part) + static_cast<double>(decimal_part) / pow(number_base, decimal_places));
    }
    return Token<T>(TokenType::Integer_literal, start_position, static_cast<int64_t>(integer_part));
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_identifier_or_keyword(){
    if(!is_current_alpha()){
        return std::nullopt;
    }

    std::basic_stringstream<T> s{};
    s << current_symbol;
    while(advance_character() && (is_current_alnum() || is_current_underscore())){
        s << current_symbol;
    }
    if(auto iter = keyword_lookup.find(s.str()); iter != keyword_lookup.cend()){
        if(iter->second == TokenType::True || iter->second == TokenType::False){
            return Token<T>(TokenType::Boolean_literal, start_position, iter->second == TokenType::True);
        }
        return Token<T>(iter->second, start_position, {});
    }
    return Token<T>(TokenType::Identifier, start_position, s.str());
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_comment(){
    if(current_symbol != '#'){
        return std::nullopt;
    }
    std::basic_stringstream<T> s{};
    while(advance_character() && !is_current_newline()){
        s << current_symbol;
    }
    return Token<T>(TokenType::Comment, start_position, s.str());
}

template<CharType T>
std::optional<Token<T>> Lexer<T>::try_build_string(){
    if(current_symbol != '\"'){
        return std::nullopt;
    }
    std::basic_stringstream<T> s{};
    while(advance_character() && !is_current_newline() && current_symbol != '\"'){
        if(current_symbol == '\\'){
            if(!advance_character()){
                throw TokenizationError<T>("Error when building string literal - ETX.", current_position);
            }
            if(auto iter = escapes.find(current_symbol); iter != escapes.cend()){
                s << iter->second;
            }
            else{
                throw TokenizationError<T>("Error when building string literal - invalid escaped character.", current_position);
            }
        }
        else{
            s << current_symbol;
        }
    }
    if(current_symbol != '\"'){
        throw TokenizationError<T>("Error when building string literal - newline before ending quotation mark.", current_position);
    }
    advance_character();
    return Token<T>(TokenType::String_literal, start_position, s.str());
}

template<CharType T>
bool Lexer<T>::advance_character(){
    input_stream >> current_symbol;
    if(input_stream.eof()){
        return false;
    }
    current_position.column++;
    return true;
}

template<CharType T>
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

template<CharType T>
void Lexer<T>::consume_newline(){
    for([[maybe_unused]] const auto &_ : *newline_sequence){
        advance_character();
    }
    feed_line();
}

template<CharType T>
void Lexer<T>::detect_newline_sequence(){
    std::basic_stringstream<T> s{};
    if(is_current_newline()){
        s << current_symbol;
        if(!advance_character()){
            newline_sequence = s.str();
            feed_line();
            return;
        }
    }
    if(is_current_newline() && current_symbol != s.str()[0]){
        s << current_symbol;
        advance_character();
    }
    feed_line();
    newline_sequence = s.str();
}

template<CharType T>
void Lexer<T>::feed_line(){
    current_position.line++;
    current_position.column = 1;
}

template<>
bool Lexer<char>::is_current_digit() const{
    return static_cast<bool>(std::isdigit(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_alpha() const{
    return static_cast<bool>(std::isalpha(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_alnum() const{
    return static_cast<bool>(std::isalnum(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_space() const{
    return static_cast<bool>(std::isspace(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_blank() const{
    return static_cast<bool>(std::isblank(static_cast<unsigned char>(current_symbol)));
}

template<>
bool Lexer<char>::is_current_underscore() const{
    return current_symbol == '_';
}

template<>
bool Lexer<char>::is_current_newline() const{
    return is_current_space() && !is_current_blank();
}


/*
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
*/


template class Lexer<char>;
//template class Lexer<wchar_t>;