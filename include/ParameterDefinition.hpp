#ifndef PARAMETER_DEFINITION_HPP
#define PARAMETER_DEFINITION_HPP

#include "TypeIdentifier.hpp"
#include "CharType.hpp"
#include <string>
#include <memory>

template<CharType T = char>
class ParameterDefinition{
public:
    ParameterDefinition(std::unique_ptr<TypeIdentifier<T>> t, const std::basic_string<T> &n) :
        type{std::move(t)}, name{n} {}
    
    const std::unique_ptr<TypeIdentifier<T>> &get_type() const {return type;}
    const std::basic_string<T> &get_name() const {return name;}
    
private:
    std::unique_ptr<TypeIdentifier<T>> type;
    std::basic_string<T> name;
};

//template class ParameterDefinition<char>;
//template class ParameterDefinition<wchar_t>;


#endif