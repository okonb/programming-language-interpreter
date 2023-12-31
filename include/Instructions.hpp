#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "CharType.hpp"
#include "IInstruction.hpp"
#include "IExpression.hpp"
#include "IVisitor.hpp"
#include "TypeIdentifier.hpp"
#include <memory>
#include <vector>

template<CharType T>
class ReturnInstruction : public IInstruction<T>{
public:
    ReturnInstruction(std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, expression(std::move(e)) {}
    
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class AssignmentInstruction : public IInstruction<T>{
public:
    AssignmentInstruction(const std::basic_string_view<T> n, std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, name{n}, expression(std::move(e)) {}
    
    const std::basic_string<T> &get_name() const {return name;}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
private:
    std::basic_string<T> name;
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class VarDefinitionInstruction : public IInstruction<T>{
public:
    VarDefinitionInstruction(std::unique_ptr<TypeIdentifier<T>> &t, const std::basic_string_view<T> n, std::unique_ptr<IExpression<T>> e, const Position &pos) :
        IInstruction<T>{pos}, type{std::move(t)}, name{n}, expression(std::move(e)) {}
    
    const std::unique_ptr<TypeIdentifier<T>> &get_type() const {return type;}
    const std::basic_string<T> &get_name() const {return name;}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
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
    
    const std::unique_ptr<IExpression<T>> &get_condition() const {return condition;}
    const std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> &get_code_block() const {return code_block;}
    const std::unique_ptr<IInstruction<T>> &get_else_block() const {return else_block;}
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
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
    
    const std::unique_ptr<IExpression<T>> &get_condition() const {return condition;}
    const std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> &get_code_block() const {return code_block;}
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> condition;
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> code_block;
};


#endif // !INSTRUCTION_HPP