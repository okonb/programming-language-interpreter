#include "Interpreter.hpp"


template<CharType T>
void Interpreter<T>::visit(ReturnInstruction<T> &instr){
    instr.get_expression();
}

template<CharType T>
void Interpreter<T>::visit(AssignmentInstruction<T> &instr){
    instr.get_expression();
}

template<CharType T>
void Interpreter<T>::visit(VarDefinitionInstruction<T> &instr){
    instr.get_expression();
}

template<CharType T>
void Interpreter<T>::visit(IfInstruction<T> &instr){
    instr.get_condition();
}

template<CharType T>
void Interpreter<T>::visit(WhileInstruction<T> &instr){
    instr.get_condition();
}

template<CharType T>
void Interpreter<T>::visit(SingleArgExpression<T> &instr){
    instr.get_expression();
}

template<CharType T>
void Interpreter<T>::visit(TwoArgExpression<T> &instr){
    instr.get_left_expression();
}

template<CharType T>
void Interpreter<T>::visit(IntegerLiteralExpression<T> &instr){
    instr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(FloatingLiteralExpression<T> &instr){
    instr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(StringLiteralExpression<T> &instr){
    instr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(BooleanLiteralExpression<T> &instr){
    instr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(IdentifierExpression<T> &instr){
    instr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(FunctionCall<T> &instr){
    instr.get_name();
}

template<CharType T>
void Interpreter<T>::visit(MatchOperation<T> &instr){
    instr.get_arguments();
}

template class Interpreter<char>;