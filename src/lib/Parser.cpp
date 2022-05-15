#include "Parser.hpp"

template<typename T>
std::map<TokenType, Type> Parser<T>::type_map{
    {TokenType::Integer_type,   Type::Integer},
    {TokenType::Floating_type,  Type::Floating},
    {TokenType::String_type,    Type::String},
    {TokenType::File_type,      Type::File},
    {TokenType::Bool_type,      Type::Bool},
    {TokenType::Void_type,      Type::Void},
};

// start_symbol = {function_definition};
template<typename T>
Program<T> Parser<T>::parse(){
    while(try_parse_function_definition())
    ;
    return Program(function_definitions);
}

// function_definition = Function_keywd, Identifier, Opening_parenth, parameter_list_definition, Closing_parenth,
//                       Colon, (type_identifier | Void_type), code_block;
template<typename T>
bool Parser<T>::try_parse_function_definition(){
    if(!check_and_advance(TokenType::Function_keywd)){
        return false;
    }
    if(!is_current_token_of_type(TokenType::Identifier)){
        throw "TODO";
    }
    std::basic_string<T> function_name = std::get<std::basic_string<T>>(current_token.get_value());
    get_next_token();
    if(!check_and_advance(TokenType::Opening_parenth)){
        throw "TODO";
    }
    // TODO PARAMETER LIST
    if(!check_and_advance(TokenType::Closing_parenth)){
        throw "TODO";
    }
    if(!check_and_advance(TokenType::Colon)){
        throw "TODO";
    }
    if(!is_current_token_a_type()){
        throw "TODO";
    }
    Type function_type = map_type(current_token.get_type());

    //DO WYWALENIA
    {
        function_type = function_type;
    }

    //TODO BLOCK

    return true;
}

template<typename T>
Type Parser<T>::map_type(TokenType type) const{
    return type_map.at(type);
}

template<typename T>
bool Parser<T>::is_current_token_a_type() const{
    return type_map.contains(current_token.get_type());
}

template<typename T>
void Parser<T>::get_next_token(){
    current_token = lexer.get_next_token();
}

template<typename T>
bool Parser<T>::is_current_token_of_type(TokenType type) const{
    return current_token.get_type() == type;
}

template<typename T>
bool Parser<T>::check_and_advance(TokenType type){
    if(!is_current_token_of_type(type)){
        return false;
    }
    get_next_token();
    return true;
}

template class Parser<char>;
template class Parser<wchar_t>;