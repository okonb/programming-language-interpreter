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
    ~ProgramTreePrinter() override = default;

    void print_program(const Program<T> &program);

private:
    std::basic_ostream<T> &stream;
    size_t level;

    void print_spacing(const size_t additional_spaces = 0) const {
        for(size_t i = 0; i < level + additional_spaces; ++i)
            stream << " |";
    }

    void visit(FunctionDefinition<T> &instr) override;
    void visit(ReturnInstruction<T> &instr) override;
    void visit(AssignmentInstruction<T> &instr) override;
    void visit(VarDefinitionInstruction<T> &instr) override;
    void visit(IfInstruction<T> &instr) override;
    void visit(WhileInstruction<T> &instr) override;

    void visit(SingleArgExpression<T> &expr) override;
    void visit(TwoArgExpression<T> &expr) override;
    void visit(LiteralExpression<T> &expr) override;
    void visit(IdentifierExpression<T> &expr) override;
    void visit(FunctionCall<T> &expr) override;
    
    void visit(MatchOperation<T> &expr) override;

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
