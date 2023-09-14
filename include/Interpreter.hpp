#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "CharType.hpp"
#include "Position.hpp"
#include "NumericType.hpp"
#include "IVisitor.hpp"
#include "Program.hpp"
#include "ValueType.hpp"
#include "overload.hpp"
#include "Variable.hpp"
#include "light_map.hpp"
#include <stdexcept>
#include <functional>
#include <map>
#include <variant>
#include <fstream>


template<CharType T>
class SimpleException : public std::runtime_error{
public:
    SimpleException(const std::basic_string<T> &text, const Position &pos) :
        std::runtime_error{text}, position{pos} {}
    const Position &get_position() const {return position;}
private:
    const Position position;
};

template<CharType T>
class UnderscoreOutsideMatchException : public SimpleException<T>{
public:
    UnderscoreOutsideMatchException(const Position &pos) :
        SimpleException<T>{"Encountered underscore outside of match expression.", pos} {}
};


template<CharType T>
class InvalidPatternFunctionSignatureException : public SimpleException<T>{
public:
    InvalidPatternFunctionSignatureException(const Position &pos) :
        SimpleException<T>{"Encountered identifier of a function with incorrect signature for match operation.", pos} {}
};

template<CharType T>
class InvalidPatternLengthException : public SimpleException<T>{
public:
    InvalidPatternLengthException(const Position &pos) :
        SimpleException<T>{"Encountered pattern with number of elements too small for match operation.", pos} {}
};

template<CharType T>
class NoMatchingPatternException : public SimpleException<T>{
public:
    NoMatchingPatternException(const Position &pos) :
        SimpleException<T>{"None of provided patterns match arguments.", pos} {}
};

template<CharType T>
class ZeroLengthMatchBlockException : public SimpleException<T>{
public:
    ZeroLengthMatchBlockException(const Position &pos) :
        SimpleException<T>{"No pattern line provided!", pos} {}
};

template<CharType T>
class NullpointerException : public SimpleException<T>{
public:
    NullpointerException() :
        SimpleException<T>{"Encountered null pointer, program tree invalid.", {}} {}
};

template<CharType T>
class SimpleTextException : public SimpleException<T>{
public:
    SimpleTextException(const std::basic_string<T> &text, const std::basic_string_view<T> n, const Position &pos) :
        SimpleException<T>{text, pos}, name{n} {}
    const std::basic_string<T> &get_name() const {return name;}
private:
    const std::basic_string<T> name;
};

template<CharType T>
class DivisionByZeroException : public SimpleTextException<T>{
public:
    DivisionByZeroException(const std::basic_string_view<T> operation_n, const Position &pos) :
        SimpleTextException<T>{"Division by zero.", operation_n, pos} {}
};

template<CharType T>
class VoidVariableException : public SimpleTextException<T>{
public:
    VoidVariableException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Variable type set to void.", var_n, pos} {}
};

template<CharType T>
class VariableRedefinitionException : public SimpleTextException<T>{
public:
    VariableRedefinitionException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Variable redefinition.", var_n, pos} {}
};

template<CharType T>
class FunctionRedefinitionException : public SimpleTextException<T>{
public:
    FunctionRedefinitionException(const std::basic_string_view<T> fun_n, const Position &pos) :
        SimpleTextException<T>{"Function redefinition.", fun_n, pos} {}
};

template<CharType T>
class VariableNotDeclaredException : public SimpleTextException<T>{
public:
    VariableNotDeclaredException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Variable was not declared in this scope.", var_n, pos} {}
};

template<CharType T>
class FunctionNotDeclaredException : public SimpleTextException<T>{
public:
    FunctionNotDeclaredException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Function was not declared.", var_n, pos} {}
};

template<CharType T>
class ConstVariableAssignmentException : public SimpleTextException<T>{
public:
    ConstVariableAssignmentException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Trying to assign to const variable.", var_n, pos} {}
};

template<CharType T>
class NonBooleanConditionException : public SimpleTextException<T>{
public:
    NonBooleanConditionException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Condition expression does not evaluate to boolean.", var_n, pos} {}
};

template<CharType T>
class NoConditionException : public SimpleTextException<T>{
public:
    NoConditionException(const std::basic_string_view<T> var_n, const Position &pos) :
        SimpleTextException<T>{"Condition expression not declared.", var_n, pos} {}
};

template<CharType T>
class RecursionLimitException : public SimpleTextException<T>{
public:
    RecursionLimitException(const std::size_t level, const std::basic_string<T> &f_name, const Position &pos) :
        SimpleTextException<T>{"Recursion limit hit.", "Level: " + std::to_string(level) + ", calling function " + f_name + ".", pos} {}
};

template<CharType T>
class VariableAssignmentTypeMismatchException : public std::runtime_error{
public:
    VariableAssignmentTypeMismatchException(const std::basic_string_view<T> var_n, const TypeIdentifier<T> &var_t, const Type val_t, const Position &pos) :
        std::runtime_error{"Trying to assign to variable of different type."}, variable_name{var_n}, variable_type{var_t}, value_type{val_t}, position{pos} {}
    const std::basic_string<T> &get_variable_name() const {return variable_name;}
    const TypeIdentifier<T> &get_variable_type(){return variable_type;}
    Type get_value_type() const {return value_type;}
    const Position &get_position() {return position;}
private:
    const std::basic_string<T> variable_name;
    const TypeIdentifier<T> variable_type;
    const Type value_type;
    const Position position;
};

template<CharType T>
class ReturnValueTypeMismatchException : public std::runtime_error{
public:
    ReturnValueTypeMismatchException(const std::basic_string_view<T> func_name, const TypeIdentifier<T> &ret_t, const Type val_t, const Position &pos) :
        std::runtime_error{"Trying to return variable of type different to declared."}, function_name{func_name}, return_type{ret_t}, value_type{val_t}, position{pos} {}
    const std::basic_string<T> &get_function_name() const {return function_name;}
    const TypeIdentifier<T> &get_return_type(){return return_type;}
    Type get_value_type() const {return value_type;}
    const Position &get_position() {return position;}
private:
    const std::basic_string<T> function_name;
    const TypeIdentifier<T> return_type;
    const Type value_type;
    const Position position;
};

template<CharType T>
class FunctionArgumentMismatchException : public std::runtime_error{
public:
    FunctionArgumentMismatchException(const std::basic_string_view<T> operation_n, const std::vector<TypeIdentifier<T>> &types_e, const std::vector<TypeIdentifier<T>> &types_g, size_t no, const Position &pos) :
        std::runtime_error{"Mismatch in argument types."}, operation_name{operation_n}, expected_types{types_e}, gotten_types{types_g}, number{no}, position{pos} {}
    const std::basic_string<T> &get_function_name() const {return operation_name;}
    const std::vector<TypeIdentifier<T>> &get_expected_type_list() const {return expected_types;}
    const std::vector<TypeIdentifier<T>> &get_gotten_type_list() const {return gotten_types;}
    const Position &get_position() {return position;}
    size_t get_number() {return number;}
private:
    const std::basic_string<T> operation_name;
    const std::vector<TypeIdentifier<T>> expected_types, gotten_types;
    const size_t number;
    const Position position;
};

template<CharType T>
class OperatorArgumentMismatchException : public std::runtime_error{
public:
    OperatorArgumentMismatchException(const std::basic_string_view<T> operation_n, const std::initializer_list<Type> &types_e_l, const std::initializer_list<Type> &types_e_r, const std::initializer_list<Type> &types_g, const Position &pos) :
        std::runtime_error{"Mismatch in argument types."}, operation_name{operation_n}, expected_types_left{types_e_l}, expected_types_right{types_e_r}, gotten_types{types_g}, position{pos} {}
    const std::basic_string<T> &get_operator_name() const {return operation_name;}
    const std::vector<Type> &get_expected_type_list_left() const {return expected_types_left;}
    const std::vector<Type> &get_expected_type_list_right() const {return expected_types_right;}
    const std::vector<Type> &get_gotten_type_list() const {return gotten_types;}
    const Position &get_position() {return position;}
private:
    const std::basic_string<T> operation_name;
    const std::vector<Type> expected_types_left, expected_types_right, gotten_types;
    const Position position;
};


template<CharType T>
using Scope = std::map<std::basic_string<T>, std::shared_ptr<Variable<T>>>;


template<CharType T>
class Context{
public:
    Context() : scopes{}, current_match_index{0}, current_match_arguments{} {scopes.push_back({});}
    Context(Scope<T> &scp) : scopes{}, current_match_index{0}, current_match_arguments{} {scopes.push_back(scp);}
    std::vector<Scope<T>> &get_scopes() {return scopes;}
    size_t get_match_index() const {return current_match_index;}
    void set_match_index(size_t i) {current_match_index = i;}
    std::vector<value_t<T>> &get_match_arguments() {return current_match_arguments;}
private:
    std::vector<Scope<T>> scopes;
    size_t current_match_index;
    std::vector<value_t<T>> current_match_arguments;
};


template<CharType T>
class Interpreter : public IVisitor<T>{
public:
    Interpreter(std::unique_ptr<Program<T>> prog, std::basic_ostream<T> &o_stream, const std::vector<std::basic_string<T>> &args);
    ~Interpreter() override = default;
    int64_t run(const std::basic_string<T> &to_start_name = "main");
    void visit(FunctionDefinition<T> &instr) override;
    void visit(ReturnInstruction<T> &instr) override;
    void visit(AssignmentInstruction<T> &instr) override;
    void visit(VarDefinitionInstruction<T> &instr) override;
    void visit(IfInstruction<T> &instr) override;
    void visit(WhileInstruction<T> &instr) override;

    void visit(SingleArgExpression<T> &expr) override;
    void visit(TwoArgExpression<T> &expr) override;
    void visit(LiteralExpression<T> &expr) override;
    void visit(IdentifierExpression<T> &expr) override;
    void visit(FunctionCall<T> &expr) override;
    
    void visit(MatchOperation<T> &instr) override;
private:
    std::unique_ptr<std::map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>>> function_definitions;
    value_t<T> current_value;
    std::shared_ptr<Variable<T>> current_variable;
    bool return_flag, match_flag;
    std::vector<Context<T>> function_stack;
    std::basic_ostream<T> &out_stream;
    const std::vector<std::basic_string<T>> program_arguments;
    std::map<std::basic_string<T>, std::basic_fstream<T>> open_files;
    size_t current_recursion_level;
    const size_t MAX_RECURSION_LEVEL;
    std::unique_ptr<FunctionDefinition<T>> get_f_d( const std::basic_string<T> &name, Type ret_type, bool ret_const,    //TODO change to string_view?
                                                const std::initializer_list<std::basic_string<T>> &p_names,
                                                const std::initializer_list<Type> &p_types,
                                                const std::initializer_list<bool> &p_consts);
    std::unique_ptr<FunctionDefinition<T>> &get_function(const std::basic_string<T> &name);
    std::shared_ptr<Variable<T>> get_variable_from_current_context(const std::basic_string<T> &name);
    bool is_function_pointer_valid(const std::unique_ptr<FunctionDefinition<T>> & ptr);
    bool is_value_bool(const value_t<T> &v);
    bool is_value_int(const value_t<T> &v);
    bool is_value_float(const value_t<T> &v);
    bool is_value_string(const value_t<T> &v);
    bool is_current_value_bool();
    bool is_current_value_int();
    bool is_current_value_float();
    bool is_current_value_string();
    bool is_current_value_of_type(Type type);
    bool is_variable_name_in_current_scope(const std::basic_string<T> &name);
    bool is_variable_name_in_current_context(const std::basic_string<T> &name);
    bool is_argument_of_right_type(const TypeIdentifier<T> &param_t, const TypeIdentifier<T> &arg_t);
    bool is_argument_a_file(const TypeIdentifier<T> &param_t, const TypeIdentifier<T> &arg_t);
    bool is_recursion_level_exceeded();
    void push_context(Context<T> context);
    void pop_context();
    void push_scope();
    void pop_scope();
    void execute_block(std::vector<std::unique_ptr<IInstruction<T>>> &block);
    void add_builtins();
    void run_builtin(const std::basic_string<T> &name);
    void add_variable(const std::basic_string<T> &name, const TypeIdentifier<T> &type);

    Type get_value_type(const value_t<T> &v);
    Type get_current_value_type();
    Scope<T> &get_current_scope();
    Context<T> &get_current_context();
    size_t get_current_match_index();
    void set_current_match_index(size_t i);
    void increment_current_match_index();
    std::vector<value_t<T>> &get_current_match_arguments();
    bool is_expression_match(ExpressionType type) const;
    ExpressionType map_from_match(ExpressionType type) const;
    value_t<T> get_current_match_argument();
    const static light_map<ExpressionType, ExpressionType, 14UL> match_expression_type_map;

    class recursion_level_guard{
    public:
        recursion_level_guard(std::size_t &l) : level{l}{
            level++;
        }
        ~recursion_level_guard(){
            level--;
        }
    private:
        std::size_t &level;
    };
};

#endif // !INTERPRETER_HPP