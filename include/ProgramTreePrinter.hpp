#ifndef PROGRAM_TREE_PRINTER_HPP
#define PROGRAM_TREE_PRINTER_HPP

#include "CharType.hpp"
#include "IVisitor.hpp"
#include "Program.hpp"

#include <ostream>

template<CharType T>
class ProgramTreePrinter : public IVisitor<T>{
public:
    ProgramTreePrinter(std::basic_ostream<T> &s) : stream{s}, level{0} {}

    void print_program(const Program<T> &program);

private:
    std::basic_ostream<T> &stream;
    size_t level;

    void print_spacing(const size_t additional_spaces = 0) const {
        for(size_t i = 0; i < level + additional_spaces; ++i)
            stream << " |";
    }

    void visit(const FunctionDefinition<T> &instr) override;
    void visit(const ReturnInstruction<T> &instr) override;
    void visit(const AssignmentInstruction<T> &instr) override;
    void visit(const VarDefinitionInstruction<T> &instr) override;
    void visit(const IfInstruction<T> &instr) override;
    void visit(const WhileInstruction<T> &instr) override;

    void visit(const SingleArgExpression<T> &expr) override;
    void visit(const TwoArgExpression<T> &expr) override;
    void visit(const LiteralExpression<T> &expr) override;
    void visit(const IdentifierExpression<T> &expr) override;
    void visit(const FunctionCall<T> &expr) override;
 
    void visit(const MatchOperation<T> &expr) override;

    class level_guard{
    public:
        level_guard(size_t &l) : level{l} { level++; }
        ~level_guard() { level--; }
    private:
        size_t &level;
    };

    level_guard get_guard(){
        return level_guard{level};
    }

};

#endif
