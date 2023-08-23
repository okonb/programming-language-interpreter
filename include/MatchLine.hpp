#ifndef MATCH_LINE_HPP
#define MATCH_LINE_HPP

#include "CharType.hpp"
#include "IExpression.hpp"
#include <memory>
#include <vector>

template<CharType T>
class MatchLine{
public:
    MatchLine(std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> p, std::unique_ptr<IExpression<T>> e) :
        pattern{std::move(p)}, expression{std::move(e)} {}
    const std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> &get_pattern() const {return pattern;}
    const std::unique_ptr<IExpression<T>> &get_expression() const {return expression;}
    
private:
    std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> pattern;
    std::unique_ptr<IExpression<T>> expression;
};

#endif // !MATCH_LINE_HPP