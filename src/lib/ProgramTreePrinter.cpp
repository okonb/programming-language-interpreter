#include "ProgramTreePrinter.hpp"

#include "overload.hpp"
#include "Instructions.hpp"
#include "Expressions.hpp"
#include <variant>

template <CharType T>
void ProgramTreePrinter<T>::print_program(const Program<T> &program){
    stream << "Program\n";
    for(const auto &function : *program.get_function_definitions()){
        function.second->accept(*this);
    }
    stream << "\n";
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const FunctionDefinition<T> &instr)
{
    const auto guard = get_guard();
    print_spacing();
    const auto &name = instr.get_name();
    const auto &type = instr.get_type();
    const auto &parameters = instr.get_parameters();
    const auto &block = instr.get_block();
    stream << "FunctionDefinition " << name << " : " << type->get_str_representation() << "\n";
    print_spacing();
    stream << "params:\n";
    for(const auto &param : *parameters){
        print_spacing(1);
        stream << "ParameterDefinition " << param->get_name() << " : " << param->get_type()->get_str_representation();
        stream << ",\n";
    }
    print_spacing();
    stream << "block:\n";
    for(const auto &instruction : *block){
        instruction->accept(*this);
    }
    print_spacing();
    stream << "\n";
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const ReturnInstruction<T> &instr){
    const auto guard = get_guard();
    print_spacing();
    stream << "ReturnInstruction:\n";
    const auto &expression = instr.get_expression();
    if(expression){
        expression->accept(*this);
    }
    else{
        print_spacing(1);
        stream << "void\n";
    }
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const AssignmentInstruction<T> &instr){
    const auto guard = get_guard();
    print_spacing();
    stream << "AssignmentInstruction: " << instr.get_name() << " = \n";
    instr.get_expression()->accept(*this);
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const VarDefinitionInstruction<T> &instr){
    const auto guard = get_guard();
    print_spacing();
    stream << "VarDefinitionInstruction: " << instr.get_name() << " : " << instr.get_type()->get_str_representation() << " = \n";
    instr.get_expression()->accept(*this);
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const IfInstruction<T> &instr){
    const auto guard = get_guard();
    const auto &condition = instr.get_condition();
    const auto &code_block = instr.get_code_block();
    const auto &else_block = instr.get_else_block();

    if(condition){
        print_spacing();
        stream << "IfInstruction\n";
        print_spacing();
        stream << "condition:\n";
        condition->accept(*this);
    }
    print_spacing();
    stream << "block:\n";
    for(const auto &expression : *code_block){
        expression->accept(*this);
    }
    if(else_block){
        print_spacing();
        stream << "Else\n";
        else_block->accept(*this);
    }
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const WhileInstruction<T> &instr){
    const auto guard = get_guard();
    const auto &condition = instr.get_condition();
    const auto &code_block = instr.get_code_block();
    print_spacing();
    stream << "WhileInstruction\n";
    print_spacing();
    stream << "condition:\n";
    condition->accept(*this);
    print_spacing();
    stream << "block:\n";
    for(const auto &expression : *code_block){
        expression->accept(*this);
    }
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const SingleArgExpression<T> &expr){
    const auto guard = get_guard();
    print_spacing();
    stream << expr.get_string_repr() << "\n";

    const auto &expression = expr.get_expression();
    if(expression){
        print_spacing();
        stream << "value:\n";
        expression->accept(*this);
    }
    else{
        stream << "\n";
    }

}

template <CharType T>
void ProgramTreePrinter<T>::visit(const TwoArgExpression<T> &expr){
    const auto guard = get_guard();
    print_spacing();
    stream << expr.get_string_repr() << "\n";

    const auto &left_expression = expr.get_left_expression();
    const auto &right_expression = expr.get_right_expression();
    if(left_expression){
        print_spacing();
        stream << "left value:\n";
        left_expression->accept(*this);
    }
    if(right_expression){
        print_spacing();
        stream << "right value:\n";
        right_expression->accept(*this);
    }
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const LiteralExpression<T> &expr){
    const auto guard = get_guard();
    const auto &value = expr.get_value();
    print_spacing();
    stream << expr.get_string_repr() << ", value: ";
    std::visit(overload{
        [this](std::monostate){ this->stream << "<void>"; },
        [this](const auto &x) { this->stream << x; }}, value);
    stream << "\n";
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const IdentifierExpression<T> &expr){
    const auto guard = get_guard();
    print_spacing();
    stream << expr.get_string_repr() << ", value: " << expr.get_value() << "\n";
}

template <CharType T>
void ProgramTreePrinter<T>::visit(const FunctionCall<T> &expr){
    const auto guard = get_guard();
    const auto &arguments = expr.get_arguments();
    print_spacing();
    stream << expr.get_string_repr() << " " << expr.get_name() << "\n"; 

    print_spacing();
    stream <<"args:\n";
    for(const auto &expression : *arguments){
        expression->accept(*this);
    }

}

template <CharType T>
void ProgramTreePrinter<T>::visit(const MatchOperation<T> &expr){
    const auto guard = get_guard();
    const auto &arguments = expr.get_arguments();
    const auto &block = expr.get_block();

    print_spacing();
    stream << expr.get_string_repr() << "\n";
    print_spacing();
    stream << "args:\n";
    for(const auto &expression : *arguments){
        expression->accept(*this);
    }
    print_spacing();
    stream << "match block:\n";
    for(const auto &line : *block){
        const auto &pattern = line->get_pattern();
        const auto &expression = line->get_expression();
        print_spacing(1);
        stream << "MatchLine:\n";
        if(pattern){
            print_spacing(1);
            stream << "pattern:\n";
            for(const auto &exp : *pattern){
                exp->accept(*this);
            }
        }
        if(expression){
            print_spacing(1);
            stream << "expression:\n";
            expression->accept(*this);
        }
    }
}

template class ProgramTreePrinter<char>;