#include "Interpreter.hpp"

#include "Instructions.hpp"
#include "Expressions.hpp"

// intentional conversion warning suppression to facilitate expected behaviour
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
namespace{
template<CharType T, NumericType P = int64_t, NumericType R = int64_t>
class OperatorResolver{
public:
    inline const static light_map<ExpressionType, std::function<value_t<T>(P, R)>, 10UL> numeric_map{
        std::array<std::pair<ExpressionType, std::function<value_t<T>(P, R)>>, 10UL>{{
        {ExpressionType::PlusExpression,            [](const P l, const R r) -> value_t<T> {return l + r;}},
        {ExpressionType::MinusExpression,           [](const P l, const R r) -> value_t<T> {return l - r;}},
        {ExpressionType::MultiplicationExpression,  [](const P l, const R r) -> value_t<T> {return l * r;}},
        {ExpressionType::DivisionExpression,        [](const P l, const R r) -> value_t<T> {if(r==0){
                                                                                                throw DivisionByZeroException<T>("", {});
                                                                                            }
                                                                                            return l / r;}},
        {ExpressionType::LtExpression,              [](const P l, const R r) -> value_t<T> {return l < r;}},
        {ExpressionType::GtExpression,              [](const P l, const R r) -> value_t<T> {return l > r;}},
        {ExpressionType::LteExpression,             [](const P l, const R r) -> value_t<T> {return l <= r;}},
        {ExpressionType::GteExpression,             [](const P l, const R r) -> value_t<T> {return l >= r;}},
        {ExpressionType::EqualsExpression,          [](const P l, const R r) -> value_t<T> {return l == r;}},
        {ExpressionType::NotEqualsExpression,       [](const P l, const R r) -> value_t<T> {return l != r;}},
        }}
    };
    inline const static light_map<ExpressionType, std::function<value_t<T>(bool, bool)>, 2UL> bool_map{
        std::array<std::pair<ExpressionType, std::function<value_t<T>(const bool, const bool)>>, 2UL>{{
        {ExpressionType::EqualsExpression,          [](const bool l, const bool r) -> value_t<T> {return l == r;}},
        {ExpressionType::NotEqualsExpression,       [](const bool l, const bool r) -> value_t<T> {return l != r;}},
        }}
    };
    static const auto &resolve_numeric(ExpressionType type){
        return numeric_map.at(type);
    }
    static const auto &resolve_bool(ExpressionType type){
        return bool_map.at(type);
    }
    static std::function<value_t<T>(const std::basic_string<T>&, const std::basic_string<T>&)> resolve_string(ExpressionType type){
        if(type == ExpressionType::StrConcatExpression){
            return [](const std::basic_string<T> &l, const std::basic_string<T> &r){
                std::basic_stringstream<T> str{};
                str << l << r;
                return str.str();
            };
        }
        if(type == ExpressionType::EqualsExpression){
            return [](const std::basic_string<T> &l, const std::basic_string<T> &r){
                return l == r;
            };
        }
        if(type == ExpressionType::NotEqualsExpression){
            return [](const std::basic_string<T> &l, const std::basic_string<T> &r){
                return l != r;
            };
        }
    throw OperatorArgumentMismatchException<T>(IExpression<T>::get_string_repr(type), {}, {}, {Type::String, Type::String}, {});
    }
};


template class OperatorResolver<char, int64_t, int64_t>;
template class OperatorResolver<char, int64_t, double>;
template class OperatorResolver<char, double, int64_t>;
template class OperatorResolver<char, double, double>;
} //namespace

#pragma GCC diagnostic pop

/*
template<CharType T, NumericType P, NumericType R>
const std::unordered_map<ExpressionType, std::function<value_t<T>(P, R)>> OperatorResolver<T, P, R>::numeric_map {
        {ExpressionType::PlusExpression,            [](P l, R r) -> value_t<T> {return l + r;}},
        {ExpressionType::MinusExpression,           [](P l, R r) -> value_t<T> {return l - r;}},
        {ExpressionType::MultiplicationExpression,  [](P l, R r) -> value_t<T> {return l * r;}},
        {ExpressionType::DivisionExpression,        [](P l, R r) -> value_t<T> {if(r==0){
                                                                                    throw DivisionByZeroException<T>("", {});
                                                                                    }
                                                                                return l / r;}},
        {ExpressionType::LtExpression,              [](P l, R r) -> value_t<T> {return l < r;}},
        {ExpressionType::GtExpression,              [](P l, R r) -> value_t<T> {return l > r;}},
        {ExpressionType::LteExpression,             [](P l, R r) -> value_t<T> {return l <= r;}},
        {ExpressionType::GteExpression,             [](P l, R r) -> value_t<T> {return l >= r;}},
        {ExpressionType::EqualsExpression,          [](P l, R r) -> value_t<T> {return l == r;}},
        {ExpressionType::NotEqualsExpression,       [](P l, R r) -> value_t<T> {return l != r;}},
};
*/

template<CharType T>
Interpreter<T>::Interpreter(std::unique_ptr<Program<T>> prog, std::basic_ostream<T> &o_stream,
    const std::vector<std::basic_string<T>> &args, size_t max_recursion_depth) :
        mem_res{}, function_definitions{std::move(prog->give_function_definitions())},
        current_value{}, returned_flag{false}, match_flag{false}, function_stack{},
        out_stream{o_stream}, program_arguments{args}, current_recursion_level{0UL},
        MAX_RECURSION_LEVEL{max_recursion_depth} {
        add_builtins();
        function_stack.reserve(MAX_RECURSION_LEVEL + 1);
    }


template<CharType T>
int64_t Interpreter<T>::run(const std::basic_string<T> &to_start_name){
    const FunctionCall<T> funcall{to_start_name, std::make_unique<std::vector<std::unique_ptr<IExpression<T>>>>(), Position()};

    funcall.accept(*this);
    if(const auto *return_value_ptr = std::get_if<int64_t>(&current_value)){
        return *return_value_ptr;
    }
    return -1;
}  


template<CharType T>
void Interpreter<T>::visit(const FunctionDefinition<T> &instr){
    const auto &block = instr.get_block();
    if(block){
        current_value = {};
        execute_block(*block);
    }
    else{
        run_builtin(instr.get_name());
    }
    if(returned_flag){
        const auto &return_type_ptr = instr.get_return_type();
        if(!return_type_ptr){
            throw NullpointerException<T>{};
        }
        if(!is_current_value_of_type(return_type_ptr->get_type())){
            throw ReturnValueTypeMismatchException<T>(instr.get_name(), *instr.get_return_type(), get_current_value_type(), instr.get_position());
        }
    }
}

template<CharType T>
void Interpreter<T>::visit(const ReturnInstruction<T> &instr){
    if(const auto &expr = instr.get_expression()){
        expr->accept(*this);
    }
    else{
        current_value = {};
    }
    returned_flag = true;
}

template<CharType T>
void Interpreter<T>::visit(const AssignmentInstruction<T> &instr){
    const auto &name = instr.get_name();
    const auto variable_ptr = get_variable_from_current_context(name);
    //current_variable = var;
    if(!variable_ptr){
        throw VariableNotDeclaredException<T>(name, instr.get_position());
    }
    const auto &variable_type = variable_ptr->get_type();
    if(variable_type.get_is_const()){
        throw ConstVariableAssignmentException<T>(name, instr.get_position());
    }
    
    instr.get_expression()->accept(*this);

    if(!is_current_value_of_type(variable_type.get_type())){
        throw VariableAssignmentTypeMismatchException<T>(name, variable_type, get_current_value_type(), instr.get_position());
    }
    variable_ptr->set_value(current_value);
}

template<CharType T>
void Interpreter<T>::visit(const VarDefinitionInstruction<T> &instr){
    const auto &name = instr.get_name();
    const auto &variable_type_ptr = instr.get_type();
    if(!variable_type_ptr){
        throw NullpointerException<T>{};
    }
    if(variable_type_ptr->get_type() == Type::Void){
        throw VoidVariableException<T>(name, instr.get_position());
    }
    if(is_variable_name_in_current_scope(name)){
        throw VariableRedefinitionException<T>(name, instr.get_position());
    }
    instr.get_expression()->accept(*this);
    if(!is_current_value_of_type(variable_type_ptr->get_type())){
        throw VariableAssignmentTypeMismatchException<T>(name, *variable_type_ptr, get_current_value_type(), instr.get_position());
    }
    add_variable(name, *variable_type_ptr);
}

template<CharType T>
void Interpreter<T>::visit(const IfInstruction<T> &instr){
    const auto &condition_expr_ptr = instr.get_condition();
    const auto &code_block_ptr = instr.get_code_block();
    if(!condition_expr_ptr){
        if(code_block_ptr){ //else
            execute_block(*code_block_ptr);
            return;
        }
        throw NoConditionException<T>("if", instr.get_position());
    }
    condition_expr_ptr->accept(*this);

    const auto *condition_ptr = std::get_if<bool>(&current_value);
    if(!condition_ptr){
        throw NonBooleanConditionException<T>("if", instr.get_position());
    }
    
    if(*condition_ptr){
        if(!code_block_ptr){
            throw NullpointerException<T>{};
        }
        execute_block(*code_block_ptr);
    }
    else{
        if(const auto &else_block_ptr = instr.get_else_block()){
            else_block_ptr->accept(*this);
        }
    }
}

template<CharType T>
void Interpreter<T>::visit(const WhileInstruction<T> &instr){
    const auto &condition_expr_ptr = instr.get_condition();
    const auto &code_block_ptr = instr.get_code_block();
    if(!condition_expr_ptr){
        throw NoConditionException<T>("while", instr.get_position());
    }
    condition_expr_ptr->accept(*this);

    const auto *condition_ptr = std::get_if<bool>(&current_value);

    if(!condition_ptr){
        throw NonBooleanConditionException<T>("while", instr.get_position());
    }
    
    while(*condition_ptr){
        execute_block(*code_block_ptr);
        if(returned_flag){
            return;
        }
        condition_expr_ptr->accept(*this);
        condition_ptr = std::get_if<bool>(&current_value);
    }
}

template<CharType T>
void Interpreter<T>::visit(const SingleArgExpression<T> &expr){
    const auto expr_type = [this](const auto type){
        if(const auto m_type = this->map_from_match(type)){
            return *m_type;
        }
        return type;
    }(expr.get_expression_type());
    
    if(expr_type == ExpressionType::UnderscoreExpression){
        if(match_flag){
            current_value = true;
            return;
        }
        throw UnderscoreOutsideMatchException<T>(expr.get_position());
    }
    if(const auto &expression_ptr = expr.get_expression()){
        expression_ptr->accept(*this);
    }
    else{
        throw NullpointerException<T>{};
    }

    if(expr_type == ExpressionType::NegateNumberExpression){
        current_value = std::visit(overload{
            [](NumericType auto v)  -> value_t<T> { return -v; },
            [](auto)                -> value_t<T> { return {}; },
        }, current_value);

        if(std::holds_alternative<std::monostate>(current_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Integer, Type::Floating}, {}, {get_current_value_type()}, expr.get_position());
        }
        return;
    }

    if(expr_type == ExpressionType::NotExpression){
        current_value = std::visit(overload{
            [](const bool v)    -> value_t<T> { return !v; },
            [](const auto &)    -> value_t<T> { return {}; },
        }, current_value);

        if(std::holds_alternative<std::monostate>(current_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {}, {get_current_value_type()}, expr.get_position());
        }
        return;
    }
}

template<CharType T>
void Interpreter<T>::visit(const TwoArgExpression<T> &expr){
    const auto expr_type = [this](const auto type){
        if(const auto m_type = this->map_from_match(type)){
            return *m_type;
        }
        return type;
    }(expr.get_expression_type());

    value_t<T> left_value{};
    if(const auto &left_expression_ptr = expr.get_left_expression(); match_flag && !left_expression_ptr){
        left_value = get_current_match_argument();
    }
    else if(left_expression_ptr){
        left_expression_ptr->accept(*this);
        left_value = current_value;
    }
    else{
        throw NullpointerException<T>();
    }
    if(expr_type == ExpressionType::AndExpression){
        const auto *left_value_bool_ptr = std::get_if<bool>(&left_value);
        if(!left_value_bool_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value)}, expr.get_position());
        }
        if(!(*left_value_bool_ptr)){
            current_value = false;
            return;
        }
        if(const auto &right_expression_ptr = expr.get_right_expression()){
            right_expression_ptr->accept(*this);
        }
        else{
            throw NullpointerException<T>{};
        }
        
        const auto &right_value = current_value;
        const auto *right_value_bool_ptr = std::get_if<bool>(&right_value);
        if(!right_value_bool_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        
        current_value = *left_value_bool_ptr && *right_value_bool_ptr;
        return;
    }

    if(expr_type == ExpressionType::OrExpression){
        const auto *left_value_bool_ptr = std::get_if<bool>(&left_value);
        if(!left_value_bool_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value)}, expr.get_position());
        }
        
        if(*left_value_bool_ptr){
            current_value = true;
            return;
        }
        if(const auto &right_expression_ptr = expr.get_right_expression()){
            right_expression_ptr->accept(*this);
        }
        else{
            throw NullpointerException<T>{};
        }
        const auto &right_value = current_value;
        const auto *right_value_bool_ptr = std::get_if<bool>(&right_value);
        if(!right_value_bool_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        current_value = *left_value_bool_ptr || *right_value_bool_ptr;
        return;
    }

    if(expr_type == ExpressionType::ModuloExpression){
        const auto *left_value_int_ptr = std::get_if<int64_t>(&left_value);
        if(!left_value_int_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Integer}, {Type::Integer}, {get_value_type(left_value)}, expr.get_position());
        }
        if(const auto &right_expression_ptr = expr.get_right_expression()){
            right_expression_ptr->accept(*this);
        }
        else{
            throw NullpointerException<T>{};
        }
        const auto &right_value = current_value;
        const auto *right_value_int_ptr = std::get_if<int64_t>(&right_value);
        if(!right_value_int_ptr){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Integer}, {Type::Integer}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        if(*right_value_int_ptr == 0){
            throw DivisionByZeroException<T>(expr.get_string_repr(), expr.get_position());
        }
        current_value = *left_value_int_ptr % *right_value_int_ptr;
        return;
    }


    if(const auto &right_expression_ptr = expr.get_right_expression()){
        right_expression_ptr->accept(*this);
    }
    else{
        throw NullpointerException<T>{};
    }
    const auto &right_value = current_value;

    try{
        current_value = std::visit(overload{
            [&expr_type](NumericType auto l, NumericType auto r)                        {return OperatorResolver<T, decltype(l), decltype(r)>::resolve_numeric(expr_type)(l, r);},
            [&expr_type](const bool l, const bool r)                                    {return OperatorResolver<T>::resolve_bool(expr_type)(l, r);},
            [&expr_type](const std::basic_string<T> &l, const std::basic_string<T> &r)  {return OperatorResolver<T>::resolve_string(expr_type)(l, r);},
            [](auto, auto)                                                              {return value_t<T>{};},
            },
            left_value, right_value);
    }
    catch(DivisionByZeroException<T> &e){
        throw DivisionByZeroException<T>(expr.get_string_repr(), expr.get_position());
    }
    catch(...){
        throw;
    }
    if(std::holds_alternative<std::monostate>(current_value)){
        throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {}, {}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
    }
}

template<CharType T>
void Interpreter<T>::visit(const LiteralExpression<T> &expr){
    current_value = expr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(const IdentifierExpression<T> &expr){
    const auto &name = expr.get_value();
    if(name == "EOF_MARKER"){   //no global variables :(
        current_value = "\x03";
        return;
    }
    
    if(const auto variable_ptr = get_variable_from_current_context(name)){
        current_value = variable_ptr->get_value();
        current_variable = variable_ptr;
    }
    else if(match_flag){
        const auto &function = get_function(name);
        if(!function){
            throw FunctionNotDeclaredException<T>(name, expr.get_position());
        }
        const auto &parameters = function->get_parameters();
        
        if(parameters->size() != 1 || get_current_match_arguments().size() == get_current_match_index()){
            throw InvalidPatternFunctionSignatureException<T>(expr.get_position());
        }
        std::vector<TypeIdentifier<T>> argument_types{};
        const auto &argument = get_current_match_argument();
        const auto argument_type = TypeIdentifier<T>(get_value_type(argument), true);
        argument_types.push_back(argument_type);
        if(!is_argument_of_right_type(*(*parameters)[0]->get_type(), argument_type)){
            throw FunctionArgumentMismatchException<T>(name, {TypeIdentifier<T>(*(*parameters)[0]->get_type())}, argument_types, 1, expr.get_position());
        }
        
        Scope<T> arguments_evaluated{};
        arguments_evaluated.insert(std::make_pair((*parameters)[0]->get_name(), std::make_shared<Variable<T>>(argument, argument_type)));    
        
        push_context(Context<T>(std::move(arguments_evaluated)));
        function->accept(*this);
        pop_context();
        returned_flag = false;
    }
    else{
        throw VariableNotDeclaredException<T>(name, expr.get_position());
    }
}

template<CharType T>
void Interpreter<T>::visit(const FunctionCall<T> &expr){

    const recursion_level_guard guard{current_recursion_level};

    const auto &name = expr.get_name(); 
    const auto &function = get_function(name);
    if(!function){
        throw FunctionNotDeclaredException<T>(name, expr.get_position());
    }

    if(is_recursion_level_exceeded()){
        throw RecursionLimitException<T>(current_recursion_level, name, expr.get_position());
    }

    const auto &parameters = function->get_parameters();
    const auto &arguments = expr.get_arguments();
    std::vector<TypeIdentifier<T>> param_types{};
    param_types.reserve(parameters->size());
    for(const auto &param : *parameters){
        param_types.push_back(*param->get_type());
    }
    if(parameters->size() != arguments->size()){
        throw FunctionArgumentMismatchException<T>(name, param_types, {}, arguments->size(), expr.get_position());
    }
    std::vector<TypeIdentifier<T>> argument_types{};
    argument_types.reserve(parameters->size());
    Scope<T> arguments_evaluated{};
    for(size_t index = 0; index < parameters->size(); ++index){
        (*arguments)[index]->accept(*this);

        if((*arguments)[index]->get_expression_type() == ExpressionType::IdentifierExpression){
            arguments_evaluated.insert(std::make_pair((*parameters)[index]->get_name(), current_variable));
            argument_types.push_back(current_variable->get_type());
            current_variable = nullptr;
        }
        else{
            arguments_evaluated.insert(std::make_pair((*parameters)[index]->get_name(), std::make_shared<Variable<T>>(current_value, TypeIdentifier<T>(get_current_value_type(), true))));
            argument_types.push_back(TypeIdentifier<T>(get_current_value_type(), true));
        }
        if(!is_argument_of_right_type(param_types[index], argument_types[index])){
            throw FunctionArgumentMismatchException<T>(name, param_types, argument_types, arguments->size(), expr.get_position());
        }
    }
    push_context(Context<T>(std::move(arguments_evaluated)));
    function->accept(*this);
    pop_context();
    returned_flag = false;
}

template<CharType T>
void Interpreter<T>::visit(const MatchOperation<T> &instr){
    const auto &arguments = instr.get_arguments();
    if(!arguments){
        throw NullpointerException<T>();
    }

    auto &argument_values = get_current_match_arguments();

    argument_values.clear();
    argument_values.reserve(arguments->size());

    for(const auto &arg : *arguments){
        arg->accept(*this);
        argument_values.push_back(current_value);
    }
    const auto &block = instr.get_block();
    if(!block){
        throw NullpointerException<T>();
    }
    match_flag = true;
    bool pattern_good = true;

    const size_t arguments_length = argument_values.size();
    if(block->empty()){
        throw ZeroLengthMatchBlockException<T>(instr.get_position());
    }
    size_t good_line_index = 0;
    for(size_t j = 0; j < block->size(); ++j){
        const auto &line = (*block)[j];
        pattern_good = true;
        const auto &pattern = line->get_pattern();
        set_current_match_index(0);
        if(pattern->size() != arguments_length){
            throw InvalidPatternLengthException<T>(instr.get_position());
        }
        
        for(size_t i = 0; i < arguments_length; ++i){
            (*pattern)[i]->accept(*this);
            match_flag = true;
            if(get_current_value_type() != get_value_type(argument_values[i])){
                if(const auto *bool_ptr = std::get_if<bool>(&current_value); bool_ptr && *bool_ptr){
                    increment_current_match_index();
                    continue;
                }
                pattern_good = false;
                break;
            }
            if(current_value == argument_values[i]){
                increment_current_match_index();
                continue;
            }
            else{
                pattern_good = false;
                break;
            }
        }
        if(pattern_good){
            good_line_index = j;
            break;
        }
    }
    match_flag = false;
    argument_values.clear();
    set_current_match_index(0);

    if(pattern_good){
        (*block)[good_line_index]->get_expression()->accept(*this);
    }
    else{
        throw NoMatchingPatternException<T>(instr.get_position());
    }
}

template<CharType T>
std::unique_ptr<FunctionDefinition<T>> &Interpreter<T>::get_function(const std::basic_string<T> &name){
    const auto found = function_definitions->find(name);
    if(found == function_definitions->end()){
        throw FunctionNotDeclaredException<T>(name, Position());
    }    
    return found->second;
}


template<CharType T>
std::shared_ptr<Variable<T>> Interpreter<T>::get_variable_from_current_context(const std::basic_string<T> &name){
    auto &scopes = get_current_context().get_scopes();
    for(auto iter = scopes.rbegin(); iter != scopes.rend(); ++iter){
        auto var = iter->find(name);
        if(var != iter->end()){
            return var->second;
        }
    }
    return nullptr;
}

template<CharType T>
bool Interpreter<T>::is_current_value_of_type(const Type type) const {
    const auto current_value_type = get_current_value_type();
    return current_value_type == type || (type == Type::File && current_value_type == Type::String);
}


template<CharType T>
bool Interpreter<T>::is_variable_name_in_current_scope(const std::basic_string<T> &name) const{
    const auto &current_scope = get_current_scope();
    const auto var = current_scope.find(name);
    return var != current_scope.cend();
}


template<CharType T>
bool Interpreter<T>::is_variable_name_in_current_context(const std::basic_string<T> &name) const{
    auto &scopes = get_current_context().get_scopes();
    for(auto iter = scopes.rbegin(); iter != scopes.rend(); ++iter){
        if(iter->contains(name)){
            return true;
        }
    }
    return false;
}

template<CharType T>
bool Interpreter<T>::is_argument_of_right_type(const TypeIdentifier<T> &param_t, const TypeIdentifier<T> &arg_t) const{
    return (param_t.get_type() == arg_t.get_type()) && !(!param_t.get_is_const() && arg_t.get_is_const());
}

template<CharType T>
bool Interpreter<T>::is_recursion_level_exceeded() const{
    return current_recursion_level > MAX_RECURSION_LEVEL;
}

template<CharType T>
void Interpreter<T>::push_context(Context<T> &&context){
    function_stack.push_back(std::move(context));
}

template<CharType T>
void Interpreter<T>::pop_context(){
    function_stack.pop_back();
}

template<CharType T>
void Interpreter<T>::push_scope(){
    get_current_context().get_scopes().push_back({});
}

template<CharType T>
void Interpreter<T>::pop_scope(){
    get_current_context().get_scopes().pop_back();
}

template<CharType T>
void Interpreter<T>::execute_block(const std::vector<std::unique_ptr<IInstruction<T>>> &block){

    push_scope();

    for(const auto &instruction : block){
        instruction->accept(*this);
        if(returned_flag){    //no need to pop scopes as we're throwing the whole call context out
            return;
        }
    }

    pop_scope();
}


template<CharType T>
void Interpreter<T>::add_builtins(){
    std::unordered_map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>> builtins{};
    builtins.insert(std::make_pair("print", get_f_d("print", Type::Void, false,
        {"text"},
        {Type::String},
        {true})));
    builtins.insert(std::make_pair("to_str_int", get_f_d("to_str_int", Type::String, false,
        {"int_v"},
        {Type::Integer},
        {true})));
    builtins.insert(std::make_pair("to_str_float", get_f_d("to_str_float", Type::String, false,
        {"float_v"},
        {Type::Floating},
        {true})));
    builtins.insert(std::make_pair("to_str_bool", get_f_d("to_str_bool", Type::String, false,
        {"bool_v"},
        {Type::Bool},
        {true})));
    builtins.insert(std::make_pair("to_int_float", get_f_d("to_int_float", Type::Integer, false,
        {"float_v"},
        {Type::Floating},
        {true})));
    builtins.insert(std::make_pair("to_float_int", get_f_d("to_float_int", Type::Floating, false,
        {"int_v"},
        {Type::Integer},
        {true})));
    builtins.insert(std::make_pair("open_file", get_f_d("open_file", Type::File, false,
        {"name"},
        {Type::String},
        {true})));
    builtins.insert(std::make_pair("bad_file", get_f_d("bad_file", Type::Bool, false,
        {"handle"},
        {Type::File},
        {true})));
    builtins.insert(std::make_pair("read_line", get_f_d("read_line", Type::String, false,
        {"handle"},
        {Type::File},
        {true})));
    builtins.insert(std::make_pair("close_file", get_f_d("close_file", Type::Bool, false,
        {"handle"},
        {Type::File},
        {true})));
    builtins.insert(std::make_pair("arguments_number", get_f_d("arguments_number", Type::Integer, false,
        {},
        {},
        {})));
    builtins.insert(std::make_pair("argument", get_f_d("argument", Type::String, false,
        {"num"},
        {Type::Integer},
        {true})));
    
    function_definitions->merge(builtins);
    if(!builtins.empty()){
        throw FunctionRedefinitionException<T>(builtins.begin()->first, {});
    }
}


template<CharType T>
void Interpreter<T>::run_builtin(const std::basic_string<T> &name){
    const static std::unordered_map<std::basic_string_view<T>, std::function<void()>> lambda_map{
        {"print",   [&]() -> void {std::visit(overload{
                        [&](std::monostate) -> void {this->out_stream << "None";},
                        [&](auto arg)       -> void {this->out_stream << arg;}
        }, get_variable_from_current_context("text")->get_value()); returned_flag = false;}},
        {"to_str_int",  [&]() -> void {std::visit(overload{
                            [&](int64_t arg)    -> void {current_value = std::to_string(arg);},
                            [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("int_v")->get_value()); returned_flag = true;}},
        {"to_str_float",    [&]() -> void {std::visit(overload{
                                [&](double arg)     -> void {current_value = std::to_string(arg);},
                                [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("float_v")->get_value()); returned_flag = true;}},
        {"to_str_bool", [&]() -> void {std::visit(overload{
                            [&](bool arg)   -> void {current_value = arg ? "true" : "false";},
                            [&](auto)       -> void {current_value = {};}
        }, get_variable_from_current_context("bool_v")->get_value()); returned_flag = true;}},
        {"to_int_float",    [&]() -> void {std::visit(overload{
                                [&](double arg) -> void {current_value = static_cast<int64_t>(arg);},
                                [&](auto)       -> void {current_value = {};}
        }, get_variable_from_current_context("float_v")->get_value()); returned_flag = true;}},
        {"to_float_int",    [&]() -> void {std::visit(overload{
                                [&](int64_t arg)    -> void {current_value = static_cast<double>(arg);},
                                [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("int_v")->get_value()); returned_flag = true;}},
        {"open_file",   [&]() -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {   if(!open_files.contains(arg)){
                                                                            open_files.insert(std::make_pair(arg, std::fstream{arg}));
                                                                        }
                                                                        current_value = arg;
                                                                    },
                            [&](auto)                       -> void {current_value = {};}  
        }, get_variable_from_current_context("name")->get_value()); returned_flag = true;}},
        {"bad_file",    [&]() -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {current_value = !static_cast<bool>(open_files.at(arg)); },
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); returned_flag = true;}},
        {"read_line",   [&]() -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {std::basic_string<T> tmp{};
                                                                    current_value = std::getline(open_files.at(arg), tmp) ? tmp : "\x03";},
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); returned_flag = true;}},
        {"close_file",  [&]() -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {open_files.erase(arg);
                                                                    current_value = true; },
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); returned_flag = true;}},
        {"arguments_number",    [&]() -> void {current_value = static_cast<int64_t>(program_arguments.size()); returned_flag = true;}},
        {"argument",    [&]() -> void {std::visit(overload{
                            [&](int64_t index)  -> void {const auto uindex = static_cast<size_t>(index);
                                                        if(uindex >= program_arguments.size()){throw std::runtime_error{"Index out of bounds."};}
                                                        current_value = program_arguments[uindex]; },
                            [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("num")->get_value()); returned_flag = true;}},
    };
    const auto function = lambda_map.find(name);
    if(function == lambda_map.end()){
        throw FunctionNotDeclaredException<T>(name, Position());
    }
    function->second();
}



template<CharType T>
std::unique_ptr<FunctionDefinition<T>> Interpreter<T>::get_f_d(const std::basic_string_view<T> name, const Type ret_type, const bool ret_const,
                                                const std::initializer_list<std::basic_string_view<T>> p_names,
                                                const std::initializer_list<Type> &p_types,
                                                const std::initializer_list<bool> &p_consts) const{

    auto return_t = std::make_unique<TypeIdentifier<T>>(ret_type, ret_const);
    auto parameters = std::make_unique<std::vector<std::unique_ptr<ParameterDefinition<T>>>>();
    parameters->reserve(p_names.size());
    for(size_t index = 0; index < p_names.size(); ++index){
        auto type = std::make_unique<TypeIdentifier<T>>(p_types.begin()[index], p_consts.begin()[index]);
        parameters->push_back(std::make_unique<ParameterDefinition<T>>(std::move(type), p_names.begin()[index]));
    }
    return std::make_unique<FunctionDefinition<T>>(name, std::move(return_t), std::move(parameters), nullptr, Position());
}

template<CharType T>
void Interpreter<T>::add_variable(const std::basic_string<T> &name, const TypeIdentifier<T> &type){
    get_current_scope().insert(std::make_pair(name, std::make_shared<Variable<T>>(current_value, type)));
}

template<CharType T>
Type Interpreter<T>::get_value_type(const value_t<T> &v) const {
    return std::visit(overload{
        [](const bool){return Type::Bool;},
        [](const int64_t){return Type::Integer;},
        [](const double){return Type::Floating;},
        [](const std::basic_string<T>&){return Type::String;},
        [](std::monostate){return Type::Void;},
    }, v);
}

template<CharType T>
Type Interpreter<T>::get_current_value_type() const{
    return get_value_type(current_value);
}

template<CharType T>
Scope<T> &Interpreter<T>::get_current_scope(){
    auto &current_scopes = get_current_context().get_scopes();
    if(const auto ptr = current_scopes.rbegin(); ptr != current_scopes.rend()){
        return *ptr;
    }
    throw std::runtime_error("No scope created.");
}

template<CharType T>
const Scope<T> &Interpreter<T>::get_current_scope() const{
    const auto &current_scopes = get_current_context().get_scopes();
    if(const auto ptr = current_scopes.crbegin(); ptr != current_scopes.crend()){
        return *ptr;
    }
    throw std::runtime_error("No scope created.");
}


template<CharType T>
Context<T> &Interpreter<T>::get_current_context(){
    const auto ptr = function_stack.rbegin();
    if(ptr == function_stack.rend()){
        throw std::runtime_error("No context created.");
    }
    return *ptr;
}

template<CharType T>
const Context<T> &Interpreter<T>::get_current_context() const{
    const auto ptr = function_stack.crbegin();
    if(ptr == function_stack.crend()){
        throw std::runtime_error("No context created.");
    }
    return *ptr;
}

template<CharType T>
size_t Interpreter<T>::get_current_match_index() const{
    return get_current_context().get_match_index();
}

template<CharType T>
void Interpreter<T>::set_current_match_index(const size_t i){
    get_current_context().set_match_index(i);
}

template<CharType T>
void Interpreter<T>::increment_current_match_index(){
    get_current_context().set_match_index(get_current_match_index() + 1);
}

template<CharType T>
std::vector<value_t<T>> &Interpreter<T>::get_current_match_arguments(){
    return get_current_context().get_match_arguments();
}

template<CharType T>
const std::vector<value_t<T>> &Interpreter<T>::get_current_match_arguments() const{
    return get_current_context().get_match_arguments();
}

template<CharType T>
std::optional<ExpressionType> Interpreter<T>::map_from_match(const ExpressionType type) {
    if(const auto *ptr = match_expression_type_map.find(type); ptr != match_expression_type_map.cend()){
        return ptr->second;
    }
    return std::nullopt;
}

template<CharType T>
const value_t<T> &Interpreter<T>::get_current_match_argument() const{
    return get_current_match_arguments()[get_current_match_index()];
}



template<CharType T>
constexpr light_map<ExpressionType, ExpressionType, 14UL> Interpreter<T>::match_expression_type_map{
    std::array<std::pair<ExpressionType, ExpressionType>, 14UL>{{
    {ExpressionType::MatchGtExpression,               ExpressionType::GtExpression              },
    {ExpressionType::MatchGteExpression,              ExpressionType::GteExpression             },
    {ExpressionType::MatchLtExpression,               ExpressionType::LtExpression              },
    {ExpressionType::MatchLteExpression,              ExpressionType::LteExpression             },
    {ExpressionType::MatchEqualsExpression,           ExpressionType::EqualsExpression          },
    {ExpressionType::MatchNotEqualsExpression,        ExpressionType::NotEqualsExpression       },
    {ExpressionType::MatchPlusExpression,             ExpressionType::PlusExpression            },
    {ExpressionType::MatchMinusExpression,            ExpressionType::MinusExpression           },
    {ExpressionType::MatchMultiplicationExpression,   ExpressionType::MultiplicationExpression  },
    {ExpressionType::MatchDivisionExpression,         ExpressionType::DivisionExpression        },
    {ExpressionType::MatchModuloExpression,           ExpressionType::ModuloExpression          },
    {ExpressionType::MatchStrConcatExpression,        ExpressionType::StrConcatExpression       },
    {ExpressionType::MatchAndExpression,              ExpressionType::AndExpression             },
    {ExpressionType::MatchOrExpression,               ExpressionType::OrExpression              },
    }}
};


template class Interpreter<char>;