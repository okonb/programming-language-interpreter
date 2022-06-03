#ifndef IINSTRUCTION_HPP
#define IINSTRUCTION_HPP

#include "CharType.hpp"
#include "ISelfPrintable.hpp"
#include "IVisitable.hpp"

template<CharType T = char>
class IInstruction : public IVisitable<T>, public ISelfPrintable<T>{};

#endif // !IINSTRUCTION_HPP