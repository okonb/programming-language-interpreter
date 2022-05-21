#ifndef IINSTRUCTION_HPP
#define IINSTRUCTION_HPP

#include "CharType.hpp"
#include "ISelfPrintable.hpp"

template<CharType T = char>
class IInstruction : public ISelfPrintable<T>{};

#endif // !IINSTRUCTION_HPP