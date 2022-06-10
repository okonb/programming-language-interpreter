#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP


#include "CharType.hpp"
template<CharType T>
class IInstruction;

template<CharType T>
class IExpression;

template<CharType T>
class IVisitor;


#include "IInstruction.hpp"
#include "Expressions.hpp"
#include "TypeIdentifier.hpp"
#include "IVisitor.hpp"
#include <memory>
#include <vector>

template<CharType T>
class ReturnInstruction : public IInstruction<T>{
public:
    ReturnInstruction(std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, expression(std::move(e)) {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "ReturnInstruction:\n";
        if(expression){
            expression->print_self(stream, level + 1);
        }
        else{
            this->print_n_spaces(stream, level + 1);
            stream << "void\n";
        }
    }
    std::unique_ptr<IExpression<T>> const &get_expression(){return expression;}
    void accept(IVisitor<T> &visitor) override{ visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class AssignmentInstruction : public IInstruction<T>{
public:
    AssignmentInstruction(const std::basic_string<T> &n, std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, name{n}, expression(std::move(e)) {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "AssignmentInstruction: " << name << " = \n";
        expression->print_self(stream, level + 1);
    }
    const std::basic_string<T> &get_name() const {return name;}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void accept(IVisitor<T> &visitor) override{ visitor.visit(*this); }
private:
    std::basic_string<T> name;
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class VarDefinitionInstruction : public IInstruction<T>{
public:
    VarDefinitionInstruction(std::unique_ptr<TypeIdentifier<T>> &t, const std::basic_string<T> &n, std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, type{std::move(t)}, name{n}, expression(std::move(e)) {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "VarDefinitionInstruction: " << name << " : " << type->get_str_representation() << " = \n";
        expression->print_self(stream, level + 1);
    }
    const std::unique_ptr<TypeIdentifier<T>> &get_type() const {return type;}
    const std::basic_string<T> &get_name() const {return name;}
    std::unique_ptr<IExpression<T>> const &get_expression() const {return expression;}
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::unique_ptr<TypeIdentifier<T>> type;
    std::basic_string<T> name;
    std::unique_ptr<IExpression<T>> expression;
};


template<CharType T>
class IfInstruction : public IInstruction<T>{
public:
    IfInstruction(std::unique_ptr<IExpression<T>> cond, std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> block, std::unique_ptr<IInstruction<T>> else_b, const Position &pos) :
        IInstruction<T>{pos}, condition(std::move(cond)), code_block(std::move(block)), else_block{std::move(else_b)} {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        if(condition){
            this->print_n_spaces(stream, level);
            stream << "IfInstruction\n";
            this->print_n_spaces(stream, level + 1);
            stream << "condition:\n";
            condition->print_self(stream, level + 2);
        }
        this->print_n_spaces(stream, level + 1);
        stream << "block:\n";
        for(const auto &expression : *code_block){
            expression->print_self(stream, level + 2);
        }
        if(else_block){
            this->print_n_spaces(stream, level + 1);
            stream << "Else\n";
            else_block->print_self(stream, level + 1);
        }
    }
    std::unique_ptr<IExpression<T>> const &get_condition() const {return condition;}
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> const &get_code_block() const {return code_block;}
    std::unique_ptr<IInstruction<T>> const &get_else_block() const {return else_block;}
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> condition;
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> code_block;
    std::unique_ptr<IInstruction<T>> else_block;
};

template<CharType T>
class WhileInstruction : public IInstruction<T>{
public:
    WhileInstruction(std::unique_ptr<IExpression<T>> cond, std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> block, const Position &pos) :
        IInstruction<T>{pos}, condition(std::move(cond)), code_block(std::move(block)) {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "WhileInstruction\n";
        this->print_n_spaces(stream, level + 1);
        stream << "condition:\n";
        condition->print_self(stream, level + 2);
        this->print_n_spaces(stream, level + 1);
        stream << "block:\n";
        for(const auto &expression : *code_block){
            expression->print_self(stream, level + 2);
        }
    }
    std::unique_ptr<IExpression<T>> const &get_condition() const {return condition;}
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> const &get_code_block() const {return code_block;}
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> condition;
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> code_block;
};


#endif // !INSTRUCTION_HPP