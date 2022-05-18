#ifndef EXPRESSIONS
#define EXPRESSIONS

#include "IInstruction.hpp"
#include "IExpression.hpp"
#include "CharType.hpp"
#include "MatchLine.hpp"
#include <memory>
#include <vector>

template<CharType T>
class SingleArgExpression : public IExpression<T> {
public:
    SingleArgExpression(ExpressionType t, std::unique_ptr<IExpression<T>> expr) :
        IExpression<T>{t}, expression{std::move(expr)} {}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        IExpression<T>::print_self(stream, level);
        if(expression){
            this->print_n_spaces(stream, level + 1);
            stream << "value:\n";
            this->expression->print_self(stream, level + 2);
        }
        else{
            stream << "\n";
        }
    }

private:
    std::unique_ptr<IExpression<T>> expression;
};

template<CharType T>
class TwoArgExpression : public IExpression<T> {
public:
    TwoArgExpression(ExpressionType t, std::unique_ptr<IExpression<T>> l_expr, std::unique_ptr<IExpression<T>> r_expr) :
        IExpression<T>{t}, left_expression{std::move(l_expr)}, right_expression{std::move(r_expr)} {}
    std::unique_ptr<IExpression<T>> const &get_left_expression() const {return left_expression;}
    std::unique_ptr<IExpression<T>> const &get_right_expression() const {return right_expression;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << "\n";
        if(left_expression){
            this->print_n_spaces(stream, level + 1);
            stream << "left value:\n";
            left_expression->print_self(stream, level + 2);
        }
        if(right_expression){
            this->print_n_spaces(stream, level + 1);
            stream << "right value:\n";
            right_expression->print_self(stream, level + 2);
        }
    }
private:
    std::unique_ptr<IExpression<T>> left_expression, right_expression;
};


template<CharType T>
class IntegerLiteralExpression : public IExpression<T> {
public:
    IntegerLiteralExpression(int64_t v) :
        IExpression<T>{ExpressionType::IntegerLiteralExpression}, value{v} {}
    int64_t get_value() const {return value;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << ", value: " << value << "\n";
    }
private:
    int64_t value;
};

template<CharType T>
class FloatingLiteralExpression : public IExpression<T> {
public:
    FloatingLiteralExpression(double v) :
        IExpression<T>{ExpressionType::FloatingLiteralExpression}, value{v} {}
    double get_value() const {return value;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << ", value: " << value << "\n";
    }
private:
    double value;
};

template<CharType T>
class StringLiteralExpression : public IExpression<T> {
public:
    StringLiteralExpression(std::basic_string<T> v) :
        IExpression<T>{ExpressionType::StringLiteralExpression}, value{v} {}
    std::basic_string<T> get_value() const {return value;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << ", value: " << value << "\n";
    }
private:
    std::basic_string<T> value;
};

template<CharType T>
class IdentifierExpression : public IExpression<T> {
public:
    IdentifierExpression(std::basic_string<T> v) :
        IExpression<T>{ExpressionType::IdentifierExpression}, value{v} {}
    std::basic_string<T> get_value() const {return value;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << ", value: " << value << "\n";
    }
private:
    std::basic_string<T> value;
};

template<CharType T = char>
class FunctionCall : public IExpression<T>{
public:
    FunctionCall(std::basic_string<T> &n, std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> args) :
        IExpression<T>{ExpressionType::FunctionCallExpression}, name{n}, arguments{std::move(args)} {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << " " << name << "\n"; 
        this->print_n_spaces(stream, level + 1);
        stream <<"args:\n";
        for(const auto &expression : *arguments){
            expression->print_self(stream, level + 2);
        }
    }
    const std::basic_string<T> &get_name() {return name;}
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> const &get_arguments() {return arguments;}
private:
    std::basic_string<T> name;
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> arguments;
};

template<CharType T = char>
class MatchOperation : public IExpression<T>{
public:
    MatchOperation(std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> args, std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> b) :
        IExpression<T>{ExpressionType::MatchExpression}, arguments{std::move(args)}, block(std::move(b)) {}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << "\n";
        this->print_n_spaces(stream, level + 1);
        stream << "args:\n";
        for(const auto &expression : *arguments){
            expression->print_self(stream, level + 2);
        }
        this->print_n_spaces(stream, level + 1);
        stream << "match block:\n";
        for(const auto &line : *block){
            line->print_self(stream, level + 2);
        }
    }
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> const &get_arguments(){return arguments;}
    std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> const &get_block(){return block;}
private:
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> arguments;
    std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> block;
};
#endif // !EXPRESSIONS