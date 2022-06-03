#include "Expressions.hpp"

template<>
std::map<ExpressionType, std::basic_string<char>> IExpression<char>::expression_string_map{
    {ExpressionType::Expression,                    "Expression"},
    {ExpressionType::PlusExpression,                "PlusExpression"},
    {ExpressionType::MinusExpression,               "MinusExpression"},
    {ExpressionType::StrConcatExpression,           "StrConcatExpression"},
    {ExpressionType::MultiplicationExpression,      "MultiplicationExpression"},
    {ExpressionType::DivisionExpression,            "DivisionExpression"},
    {ExpressionType::ModuloExpression,              "ModuloExpression"},
    {ExpressionType::AndExpression,                 "AndExpression"},
    {ExpressionType::OrExpression,                  "OrExpression"},
    {ExpressionType::LtExpression,                  "LtExpression"},
    {ExpressionType::GtExpression,                  "GtExpression"},
    {ExpressionType::LteExpression,                 "LteExpression"},
    {ExpressionType::GteExpression,                 "GteExpression"},
    {ExpressionType::EqualsExpression,              "EqualsExpression"},
    {ExpressionType::NotEqualsExpression,           "NotEqualsExpression"},
    {ExpressionType::NotExpression,                 "NotExpression"},
    {ExpressionType::NegateNumberExpression,        "NegateNumberExpression"},
    {ExpressionType::MatchPlusExpression,           "MatchPlusExpression"},
    {ExpressionType::MatchMinusExpression,          "MatchMinusExpression"},
    {ExpressionType::MatchStrConcatExpression,      "MatchStrConcatExpression"},
    {ExpressionType::MatchMultiplicationExpression, "MatchMultiplicationExpression"},
    {ExpressionType::MatchDivisionExpression,       "MatchDivisionExpression"},
    {ExpressionType::MatchModuloExpression,         "MatchModuloExpression"},
    {ExpressionType::MatchAndExpression,            "MatchAndExpression"},
    {ExpressionType::MatchOrExpression,             "MatchOrExpression"},
    {ExpressionType::MatchLtExpression,             "MatchLtExpression"},
    {ExpressionType::MatchGtExpression,             "MatchGtExpression"},
    {ExpressionType::MatchLteExpression,            "MatchLteExpression"},
    {ExpressionType::MatchGteExpression,            "MatchGteExpression"},
    {ExpressionType::MatchEqualsExpression,         "MatchEqualsExpression"},
    {ExpressionType::MatchNotEqualsExpression,      "MatchNotEqualsExpression"},
    {ExpressionType::MatchNegateNumberExpression,   "MatchNegateNumberExpressio"},
    {ExpressionType::IntegerLiteralExpression,      "IntegerLiteralExpression"},
    {ExpressionType::FloatingLiteralExpression,     "FloatingLiteralExpression"},
    {ExpressionType::StringLiteralExpression,       "StringLiteralExpression"},
    {ExpressionType::FunctionCallExpression,        "FunctionCallExpression"},
    {ExpressionType::MatchExpression,               "MatchExpression"},
    {ExpressionType::UnderscoreExpression,          "UnderscoreExpression"},
    {ExpressionType::IdentifierExpression,          "IdentifierExpression"},
    {ExpressionType::BooleanLiteralExpression,      "BooleanLiteralExpression"}
};

template class SingleArgExpression<char>;
template class TwoArgExpression<char>;