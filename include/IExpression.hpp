#ifndef IEXPRESSION_HPP
#define IEXPRESSION_HPP

#include "CharType.hpp"
#include "IInstruction.hpp"
#include "light_map.hpp"
#include <map>
#include <string>
#include <string_view>

enum class ExpressionType{
    Expression,
    PlusExpression,
    MinusExpression,
    StrConcatExpression,
    MultiplicationExpression,
    DivisionExpression,
    ModuloExpression,
    AndExpression,
    OrExpression,
    LtExpression,
    GtExpression,
    LteExpression,
    GteExpression,
    EqualsExpression,
    NotEqualsExpression,
    NotExpression,
    NegateNumberExpression,
    MatchPlusExpression,
    MatchMinusExpression,
    MatchStrConcatExpression,
    MatchMultiplicationExpression,
    MatchDivisionExpression,
    MatchModuloExpression,
    MatchAndExpression,
    MatchOrExpression,
    MatchLtExpression,
    MatchGtExpression,
    MatchLteExpression,
    MatchGteExpression,
    MatchEqualsExpression,
    MatchNotEqualsExpression,
    MatchNegateNumberExpression,
    IntegerLiteralExpression,
    FloatingLiteralExpression,
    StringLiteralExpression,
    BooleanLiteralExpression,
    FunctionCallExpression,
    IdentifierExpression,
    MatchExpression,
    UnderscoreExpression,
};

template<CharType T>
class IExpression : public IInstruction<T> {
public:
    IExpression(ExpressionType t, const Position &pos) : IInstruction<T>{pos}, type{t} {}
    ~IExpression() override = default;
    const std::basic_string_view<T> &get_string_repr() const {return expression_string_map.at(type);}
    static const std::basic_string_view<T> &get_string_repr(ExpressionType t) {return expression_string_map.at(t);}
    ExpressionType get_expression_type() const {return type;}
    
private:
    ExpressionType type;
    const static light_map<ExpressionType, std::basic_string_view<T>, 41UL> expression_string_map;
};

#endif // !IEXPRESSION_HPP