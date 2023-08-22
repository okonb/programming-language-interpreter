#ifndef IVISITABLE_HPP
#define IVISITABLE_HPP

#include "CharType.hpp"
#include "IVisitor.hpp"

template<CharType T>
class IVisitable{
public:
    virtual ~IVisitable() = default;
    virtual void accept(IVisitor<T> &visitor) = 0;
};


#endif