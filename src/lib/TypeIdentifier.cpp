#include "TypeIdentifier.hpp"

template<>
constexpr light_map<Type, std::basic_string_view<char>, 6UL> TypeIdentifier<char>::type_map{
    std::array<std::pair<Type, std::basic_string_view<char>>, 6UL>{{
    {Type::Integer,     "int"},
    {Type::Floating,    "float"},
    {Type::String,      "str"},
    {Type::File,        "file"},
    {Type::Bool,        "bool"},
    {Type::Void,        "void"},
    }}
};

template class TypeIdentifier<char>;

/*
template<>
const std::map<Type, std::basic_string<wchar_t>> TypeIdentifier<wchar_t>::type_map{
    {Type::Integer,     L"int"},
    {Type::Floating,    L"float"},
    {Type::String,      L"str"},
    {Type::File,        L"file"},
    {Type::Bool,        L"bool"},
    {Type::Void,        L"void"},
};
*/