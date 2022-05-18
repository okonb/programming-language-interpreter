#ifndef ISELF_PRINTABLE_HPP
#define ISELF_PRINTABLE_HPP

#include "CharType.hpp"
#include <sstream>

template<CharType T>
class ISelfPrintable{
public:
    void print_n_spaces(std::ostream &stream, size_t level) const{
        for(size_t i = 0; i < level; ++i)
            stream << " |";
    }
    virtual void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const = 0;
};

#endif // !ISELF_PRINTABLE_HPP