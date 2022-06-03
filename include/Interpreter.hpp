#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "CharType.hpp"
#include "IVisitor.hpp"

template<CharType T>
class Interpreter : public IVisitor<T>{
public:
    void visit(ReturnInstruction<T> &instr) override;
    void visit(AssignmentInstruction<T> &instr) override;
    void visit(VarDefinitionInstruction<T> &instr) override;
    void visit(IfInstruction<T> &instr) override;
    void visit(WhileInstruction<T> &instr) override;

    void visit(SingleArgExpression<T> &instr) override;
    void visit(TwoArgExpression<T> &instr) override;
    void visit(IntegerLiteralExpression<T> &instr) override;
    void visit(FloatingLiteralExpression<T> &instr) override;
    void visit(StringLiteralExpression<T> &instr) override;
    void visit(BooleanLiteralExpression<T> &instr) override;
    void visit(IdentifierExpression<T> &instr) override;
    void visit(FunctionCall<T> &instr) override;
    
    void visit(MatchOperation<T> &instr) override;
};

#endif // !INTERPRETER_HPP