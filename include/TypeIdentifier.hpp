#ifndef TYPE_IDENTIFIER_HPP
#define TYPE_IDENTIFIER_HPP

#include <stdexcept>
#include <map>
#include <string>
#include <sstream>
#include "CharType.hpp"


class ConstVoidTypeException : public std::runtime_error {
public:
    ConstVoidTypeException() : std::runtime_error{"Error: non-returning function marked as const."} {}
};

enum class Type{
    Integer,
    Floating,
    String,
    File,
    Bool,
    Void,
};

template<CharType T>
class TypeIdentifier{
public:
    TypeIdentifier(const Type t, const bool is_c) :
        type{t}, is_const{is_c} {}
    Type get_type() const {return type;}
    bool get_is_const() const {return is_const;}
    std::basic_string<T> get_str_representation() const {
        std::basic_stringstream<T> s;
        if(get_is_const()){
            s << "const ";
        }
        s << type_map.at(get_type());
        return s.str();
    }
    bool operator==(const TypeIdentifier<T> &other) const {return type == other.get_type() && is_const == other.get_is_const();}
    static const std::basic_string<T> &get_type_text(const Type t) {return type_map.at(t);}
private:
    Type type;
    bool is_const;
    static std::map<Type, std::basic_string<T>> type_map;
};

#endif // TYPE_IDENTIFIER_HPP