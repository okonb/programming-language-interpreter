#ifndef EXPRESSIONS_FWD_HPP
#define EXPRESSIONS_FWD_HPP

#include "CharType.hpp"

template<CharType T>
class SingleArgExpression;

template<CharType T>
class TwoArgExpression;

template<CharType T>
class IntegerLiteralExpression;

template<CharType T>
class FloatingLiteralExpression;

template<CharType T>
class StringLiteralExpression;

template<CharType T>
class BooleanLiteralExpression;

template<CharType T>
class IdentifierExpression;

template<CharType T>
class FunctionCall;

template<CharType T>
class MatchOperation;
#endif