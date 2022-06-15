#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "CharType.hpp"
#include "TypeIdentifier.hpp"
#include "ValueType.hpp"

template<CharType T>
class Variable{
public:
    Variable(const value_t<T> &v, const TypeIdentifier<T> &t) : value{v}, type{t} {} 
    value_t<T> &get_value() { return value; }
    void set_value(const value_t<T> &v) { value = v; }
    TypeIdentifier<T> get_type() { return type; }
    bool is_const() const { return type.get_is_const(); }
private:
    value_t<T> value;
    TypeIdentifier<T> type;
};


#endif