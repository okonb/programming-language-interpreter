#ifndef MATCH_LINE_HPP
#define MATCH_LINE_HPP

#include "CharType.hpp"
#include "IExpression.hpp"
#include "ISelfPrintable.hpp"
#include <memory>
#include <vector>

template<CharType T>
class MatchLine : public ISelfPrintable<T>{
public:
    MatchLine(std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> p, std::unique_ptr<IExpression<T>> e) :
        pattern{std::move(p)}, expression{std::move(e)} {}
    const std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> &get_pattern() const {return pattern;}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override {
        this->print_n_spaces(stream, level);
        stream << "MatchLine:\n";
        if(pattern){
            this->print_n_spaces(stream, level + 1);
            stream << "pattern:\n";
            for(const auto &exp : *pattern){
            exp->print_self(stream, level + 2);
            }
        }
        if(expression){
            this->print_n_spaces(stream, level + 1);
            stream << "expression:\n";
            expression->print_self(stream, level + 2);
        }
    }
private:
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> pattern;
    std::unique_ptr<IExpression<T>> expression;
};

#endif // !MATCH_LINE_HPP