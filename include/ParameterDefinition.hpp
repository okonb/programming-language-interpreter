#ifndef PARAMETER_DEFINITION_HPP
#define PARAMETER_DEFINITION_HPP

#include "TypeIdentifier.hpp"
#include "ISelfPrintable.hpp"
#include "CharType.hpp"
#include <string>
#include <sstream>

template<CharType T = char>
class ParameterDefinition : public ISelfPrintable<T>{
public:
    explicit ParameterDefinition(const TypeIdentifier<T> &t, const std::basic_string<T> &n) :
        type{t}, name{n} {}
    
    const TypeIdentifier<T> &get_type() const {return type;}
    const std::basic_string<T> &get_name() const {return name;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "ParameterDefinition " << get_name() << " : " << type.get_str_representation();
    }
private:
    TypeIdentifier<T> type;
    std::basic_string<T> name;
};

//template class ParameterDefinition<char>;
//template class ParameterDefinition<wchar_t>;


#endif