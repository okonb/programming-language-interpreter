#ifndef IVISITOR_HPP
#define IVISITOR_HPP

#include "CharType.hpp"

#include "Instructions.fwd.hpp"
#include "Expressions.fwd.hpp"
#include "FunctionDefinition.fwd.hpp"


template<CharType T>
class IVisitor{
public:
    virtual ~IVisitor() = default;
    virtual void visit(const FunctionDefinition<T> &instr) = 0;

    virtual void visit(const ReturnInstruction<T> &instr) = 0;
    virtual void visit(const AssignmentInstruction<T> &instr) = 0;
    virtual void visit(const VarDefinitionInstruction<T> &instr) = 0;
    virtual void visit(const IfInstruction<T> &instr) = 0;
    virtual void visit(const WhileInstruction<T> &instr) = 0;
 
    virtual void visit(const SingleArgExpression<T> &instr) = 0;
    virtual void visit(const TwoArgExpression<T> &instr) = 0;
    virtual void visit(const LiteralExpression<T> &instr) = 0;
    virtual void visit(const IdentifierExpression<T> &instr) = 0;
    virtual void visit(const FunctionCall<T> &instr) = 0;
 
    virtual void visit(const MatchOperation<T> &instr) = 0;
};

#endif // !IVISITOR_HPP