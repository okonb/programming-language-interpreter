#ifndef IINSTRUCTION_HPP
#define IINSTRUCTION_HPP

#include "CharType.hpp"
#include "Position.hpp"
#include "ISelfPrintable.hpp"
#include "IVisitable.hpp"

template<CharType T = char>
class IInstruction : public IVisitable<T>, public ISelfPrintable<T>{
public:
    IInstruction(const Position &pos) : position{pos} {}
    ~IInstruction() override = default;
    const Position &get_position() const {return position;}
private:
    Position position;
};

#endif // !IINSTRUCTION_HPP