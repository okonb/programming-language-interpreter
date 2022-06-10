#ifndef IVISITOR_HPP
#define IVISITOR_HPP

#include "CharType.hpp"

#include "Instructions.fwd.hpp"
#include "Expressions.fwd.hpp"

#include "Instructions.hpp"
#include "Expressions.hpp"

template<CharType T>
class FunctionDefinition;

#include "FunctionDefinition.hpp"

template<CharType T>
class IVisitor{
public:
    virtual void visit(FunctionDefinition<T> &instr) = 0;

    virtual void visit(ReturnInstruction<T> &instr) = 0;
    virtual void visit(AssignmentInstruction<T> &instr) = 0;
    virtual void visit(VarDefinitionInstruction<T> &instr) = 0;
    virtual void visit(IfInstruction<T> &instr) = 0;
    virtual void visit(WhileInstruction<T> &instr) = 0;

    virtual void visit(SingleArgExpression<T> &instr) = 0;
    virtual void visit(TwoArgExpression<T> &instr) = 0;
    virtual void visit(LiteralExpression<T> &instr) = 0;
    virtual void visit(IdentifierExpression<T> &instr) = 0;
    virtual void visit(FunctionCall<T> &instr) = 0;
    
    virtual void visit(MatchOperation<T> &instr) = 0;
};

#endif // !IVISITOR_HPP