#ifndef IEXPRESSION_HPP
#define IEXPRESSION_HPP

#include "CharType.hpp"
#include "IInstruction.hpp"
#include <map>
#include <string>

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
    FunctionCallExpression,
    IdentifierExpression,
    MatchExpression,
    UnderscoreExpression,
    BooleanLiteralExpression
};

template<CharType T>
class IExpression : public IInstruction<T> {
public:
    IExpression(ExpressionType t) : type{t} {}
    const std::basic_string<T> &get_string_repr() const {return expression_string_map.at(type);}
    ExpressionType get_expression_type() const {return type;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << this->get_string_repr() << "\n";
    }
private:
    ExpressionType type;
    static std::map<ExpressionType, std::basic_string<T>> expression_string_map;
};

#endif // !IEXPRESSION_HPP