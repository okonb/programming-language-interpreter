#ifndef ILEXER_HPP
#define ILEXER_HPP

#include "Token.hpp"

template<CharType T>
class ILexer{
public:
    virtual Token<T> get_next_token() = 0;
};

#endif // !ILEXER_HPP