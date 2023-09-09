#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include "IExpression.hpp"
#include "CharType.hpp"
#include "MatchLine.hpp"
#include "IVisitor.hpp"
#include "ValueType.hpp"
#include <memory>
#include <vector>

template<CharType T>
class SingleArgExpression : public IExpression<T> {
public:
    SingleArgExpression(const ExpressionType t, std::unique_ptr<IExpression<T>> expr, const Position &pos) :
        IExpression<T>{t, pos}, expression{std::move(expr)} {}
    ~SingleArgExpression() override = default;
    std::unique_ptr<IExpression<T>> &get_expression() {return expression;}
    
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }

private:
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class TwoArgExpression : public IExpression<T> {
public:
    TwoArgExpression(const ExpressionType t, std::unique_ptr<IExpression<T>> l_expr, std::unique_ptr<IExpression<T>> r_expr, const Position &pos) :
        IExpression<T>{t, pos}, left_expression{std::move(l_expr)}, right_expression{std::move(r_expr)} {}
    ~TwoArgExpression() override = default;
    std::unique_ptr<IExpression<T>> const &get_left_expression() const {return left_expression;}
    std::unique_ptr<IExpression<T>> const &get_right_expression() const {return right_expression;}
    
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::unique_ptr<IExpression<T>> left_expression, right_expression;
};


template<CharType T>
class LiteralExpression : public IExpression<T> {
public:
    LiteralExpression(const ExpressionType t, const value_t<T> &v, const Position &pos) :
        IExpression<T>{t, pos}, value{v} {}
    ~LiteralExpression() override = default;
    value_t<T> get_value() const {return value;}
    
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    value_t<T> value;
};

template<CharType T>
class IdentifierExpression : public IExpression<T> {
public:
    IdentifierExpression(const std::basic_string<T> &v, const Position &pos) :
        IExpression<T>{ExpressionType::IdentifierExpression, pos}, value{v} {}
    ~IdentifierExpression() override = default;
    std::basic_string<T> get_value() const {return value;}
    
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::basic_string<T> value;
};

template<CharType T = char>
class FunctionCall : public IExpression<T>{
public:
    FunctionCall(const std::basic_string<T> &n, std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> args, const Position &pos) :
        IExpression<T>{ExpressionType::FunctionCallExpression, pos}, name{n}, arguments{std::move(args)} {}
    ~FunctionCall() override = default;
    
    const std::basic_string<T> &get_name() {return name;}
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> const &get_arguments() {return arguments;}
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::basic_string<T> name;
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> arguments;
};

template<CharType T = char>
class MatchOperation : public IExpression<T>{
public:
    MatchOperation(std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> args, std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> b, const Position &pos) :
        IExpression<T>{ExpressionType::MatchExpression, pos}, arguments{std::move(args)}, block(std::move(b)) {}
    ~MatchOperation() override = default;
    
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> const &get_arguments(){return arguments;}
    std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> const &get_block(){return block;}
    void accept(IVisitor<T> &visitor) override { visitor.visit(*this); }
private:
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> arguments;
    std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> block;
};
#endif // !EXPRESSIONS_HPP