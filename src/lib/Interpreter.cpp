#include "Interpreter.hpp"

#include "Instructions.hpp"
#include "Expressions.hpp"

// intentional conversion warning suppression to facilitate expected behaviour
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
template<CharType T, NumericType P = int64_t, NumericType R = int64_t>
class Resolver{
public:
    inline const static light_map<ExpressionType, std::function<value_t<T>(P, R)>, 10UL> numeric_map{
        std::array<std::pair<ExpressionType, std::function<value_t<T>(P, R)>>, 10UL>{{
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

#pragma GCC diagnostic pop

/*
template<CharType T, NumericType P, NumericType R>
const std::map<ExpressionType, std::function<value_t<T>(P, R)>> Resolver<T, P, R>::numeric_map {
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
Interpreter<T>::Interpreter(std::unique_ptr<Program<T>> prog, std::basic_ostream<T> &o_stream, const std::vector<std::basic_string<T>> &args) :
    function_definitions{std::move(prog->give_function_definitions())},
    current_value{}, return_flag{false}, match_flag{false}, function_stack{},
    out_stream{o_stream}, program_arguments{args}, current_recursion_level{0UL},
    MAX_RECURSION_LEVEL{100} /*TODO*/ {
        add_builtins();
    }



template<CharType T>
int64_t Interpreter<T>::run(const std::basic_string<T> &to_start_name){
    FunctionCall<T> funcall{to_start_name, std::make_unique<std::vector<std::unique_ptr<IExpression<T>>>>(), Position()};
    funcall.accept(*this);
    if(std::holds_alternative<int64_t>(current_value)){
        return std::get<int64_t>(current_value);
    }
    return -1;
}  


template<CharType T>
void Interpreter<T>::visit(FunctionDefinition<T> &instr){
    auto &block = instr.get_block();
    if(block){
        current_value = {};
        execute_block(*block);
    }
    else{
        run_builtin(instr.get_name());
    }
    if(return_flag && !is_current_value_of_type(instr.get_type()->get_type())){
        throw ReturnValueTypeMismatchException<T>(instr.get_name(), *instr.get_type(), get_current_value_type(), instr.get_position());
    }
}

template<CharType T>
void Interpreter<T>::visit(ReturnInstruction<T> &instr){
    if(auto &expr = instr.get_expression()){
        expr->accept(*this);
    }
    else{
        current_value = {};
    }
    return_flag = true;
}

template<CharType T>
void Interpreter<T>::visit(AssignmentInstruction<T> &instr){
    auto &name = instr.get_name();
    auto var = get_variable_from_current_context(name);
    current_variable = var;
    if(!var){
        throw VariableNotDeclaredException<T>(name, instr.get_position());
    }
    if(var->is_const()){
        throw ConstVariableAssignmentException<T>(name, instr.get_position());
    }
    
    instr.get_expression()->accept(*this);

    if(!is_current_value_of_type(var->get_type().get_type())){
        throw VariableAssignmentTypeMismatchException<T>(name, var->get_type(), get_current_value_type(), instr.get_position());
    }
    var->set_value(current_value);
}

template<CharType T>
void Interpreter<T>::visit(VarDefinitionInstruction<T> &instr){
    auto &name = instr.get_name();
    if(instr.get_type()->get_type() == Type::Void){
        throw VoidVariableException<T>(name, instr.get_position());
    }
    if(is_variable_name_in_current_scope(name)){
        throw VariableRedefinitionException<T>(name, instr.get_position());
    }
    instr.get_expression()->accept(*this);
    if(!is_current_value_of_type(instr.get_type()->get_type())){
        throw VariableAssignmentTypeMismatchException<T>(name, *instr.get_type(), get_current_value_type(), instr.get_position());
    }
    add_variable(name, *instr.get_type());
}

template<CharType T>
void Interpreter<T>::visit(IfInstruction<T> &instr){
    if(!instr.get_condition()){
        if(instr.get_code_block()){
            execute_block(*instr.get_code_block());
            return;
        }
        throw NoConditionException<T>("if", instr.get_position());
    }
    instr.get_condition()->accept(*this);
    if(!is_current_value_bool()){
        throw NonBooleanConditionException<T>("if", instr.get_position());
    }
    auto condition = std::get<bool>(current_value);
    if(condition){
        execute_block(*instr.get_code_block());
    }
    else{
        if(instr.get_else_block()){
            instr.get_else_block()->accept(*this);
        }
    }
}

template<CharType T>
void Interpreter<T>::visit(WhileInstruction<T> &instr){
    if(!instr.get_condition()){
        throw NoConditionException<T>("while", instr.get_position());
    }
    instr.get_condition()->accept(*this);
    if(!is_current_value_bool()){
        throw NonBooleanConditionException<T>("while", instr.get_position());
    }
    
    auto condition = std::get<bool>(current_value);
    while(condition){
        execute_block(*instr.get_code_block());
        if(return_flag){
            return;
        }
        instr.get_condition()->accept(*this);
        condition = std::get<bool>(current_value);
    }
}

template<CharType T>
void Interpreter<T>::visit(SingleArgExpression<T> &expr){
    auto expr_type = expr.get_expression_type();
    if(expr_type == ExpressionType::UnderscoreExpression){
        if(match_flag){
            current_value = true;
            return;
        }
        throw UnderscoreOutsideMatchException<T>(expr.get_position());
    }
    expr.get_expression()->accept(*this);
    if(expr_type == ExpressionType::NegateNumberExpression || expr_type == ExpressionType::MatchNegateNumberExpression){
        auto val = std::visit(overload{
            [](NumericType auto v)  -> value_t<T> { return -v; },
            [](auto)                -> value_t<T> { return {}; },
        }, current_value);

        if(std::holds_alternative<std::monostate>(val)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Integer, Type::Floating}, {}, {get_current_value_type()}, expr.get_position());
        }
        current_value = val;
        return;
    }

    if(expr_type == ExpressionType::NotExpression || expr_type == ExpressionType::MatchNotExpression){
        const value_t<T> val = std::visit(overload{
            [](bool v)  -> value_t<T> { return !v; },
            [](auto)    -> value_t<T> { return {}; },
        }, current_value);

        if(std::holds_alternative<std::monostate>(val)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {}, {get_current_value_type()}, expr.get_position());
        }
        current_value = val;
        return;
    }
    if(is_expression_match(expr_type)){
        auto new_expression_type = map_from_match(expr_type);
        TwoArgExpression<T> new_expr{new_expression_type, nullptr, std::move(expr.get_expression()), expr.get_position()};
        new_expr.accept(*this);
    }
}

template<CharType T>
void Interpreter<T>::visit(TwoArgExpression<T> &expr){
    auto expr_type = expr.get_expression_type();
    value_t<T> left_value{};
    if(match_flag && !(expr.get_left_expression())){
        left_value = get_current_match_argument();
    }
    else if(expr.get_left_expression()){
        expr.get_left_expression()->accept(*this);
        left_value = current_value;
    }
    else{
        throw NullpointerException<T>();
    }
    if(expr_type == ExpressionType::AndExpression){
        if(!is_value_bool(left_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value)}, expr.get_position());
        }
        const bool left_value_bool = std::get<bool>(left_value);
        if(!left_value_bool){
            current_value = false;
            return;
        }
        expr.get_right_expression()->accept(*this);
        auto right_value = current_value;
        if(!is_value_bool(right_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        const bool right_value_bool = std::get<bool>(right_value);
        current_value = left_value_bool && right_value_bool;
        return;
    }

    if(expr_type == ExpressionType::OrExpression){
        if(!is_value_bool(left_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value)}, expr.get_position());
        }
        const bool left_value_bool = std::get<bool>(left_value);
        if(left_value_bool){
            current_value = true;
            return;
        }
        expr.get_right_expression()->accept(*this);
        auto right_value = current_value;
        if(!is_value_bool(right_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        const bool right_value_bool = std::get<bool>(right_value);
        current_value = left_value_bool || right_value_bool;
        return;
    }

    if(expr_type == ExpressionType::ModuloExpression){
        if(!is_value_int(left_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Integer}, {Type::Integer}, {get_value_type(left_value)}, expr.get_position());
        }
        const int64_t left_value_int = std::get<int64_t>(left_value);
        expr.get_right_expression()->accept(*this);
        auto right_value = current_value;
        if(!is_value_int(right_value)){
            throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {Type::Bool}, {Type::Bool}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
        }
        const int64_t right_value_int = std::get<int64_t>(right_value);
        if(right_value_int == 0){
            throw DivisionByZeroException<T>(expr.get_string_repr(), expr.get_position());
        }
        current_value = left_value_int % right_value_int;
        return;
    }


    expr.get_right_expression()->accept(*this);
    auto right_value = current_value;

    value_t<T> result{};

    try{
        result = std::visit(overload{
            [&expr_type](NumericType auto l, NumericType auto r)                        {return Resolver<T, decltype(l), decltype(r)>::resolve_numeric(expr_type)(l, r);},
            [&expr_type](const bool l, const bool r)                                    {return Resolver<T>::resolve_bool(expr_type)(l, r);},
            [&expr_type](const std::basic_string<T> &l, const std::basic_string<T> &r)  {return Resolver<T>::resolve_string(expr_type)(l, r);},
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
    if(std::holds_alternative<std::monostate>(result)){
        throw OperatorArgumentMismatchException<T>(expr.get_string_repr(), {}, {}, {get_value_type(left_value), get_value_type(right_value)}, expr.get_position());
    }
    current_value = result;
}

template<CharType T>
void Interpreter<T>::visit(LiteralExpression<T> &expr){
    current_value = expr.get_value();
}

template<CharType T>
void Interpreter<T>::visit(IdentifierExpression<T> &expr){
    auto name = expr.get_value();
    if(name == "EOF_MARKER"){   //no global variables :(
        current_value = "\x03";
        return;
    }
    auto var = get_variable_from_current_context(name);
    if(var){
        current_value = var->get_value();
        current_variable = var;
    }
    else{
        if(match_flag){
            auto &function = get_function(name);
            if(!function){
                throw FunctionNotDeclaredException<T>(name, expr.get_position());
            }
            auto &parameters = function->get_parameters();
           
            if(parameters->size() != 1 || get_current_match_arguments().size() == get_current_match_index()){
                throw InvalidPatternFunctionSignatureException<T>(expr.get_position());
            }
            std::vector<TypeIdentifier<T>> argument_types{};
            auto argument = get_current_match_argument();
            auto argument_type = TypeIdentifier<T>(get_value_type(argument), true);
            argument_types.push_back(argument_type);
            Scope<T> arguments_evaluated{};
            if(!is_argument_of_right_type(*(*parameters)[0]->get_type(), argument_type)){
                throw FunctionArgumentMismatchException<T>(name, {TypeIdentifier<T>(*(*parameters)[0]->get_type())}, argument_types, 1, expr.get_position());
            }
            
            arguments_evaluated.insert(std::make_pair((*parameters)[0]->get_name(), std::make_shared<Variable<T>>(argument, argument_type)));    
            
            push_context(Context<T>(arguments_evaluated));
            function->accept(*this);
            pop_context();
            return_flag = false;
        }
        else{
            throw VariableNotDeclaredException<T>(name, expr.get_position());
        }
    }
}

template<CharType T>
void Interpreter<T>::visit(FunctionCall<T> &expr){

    const recursion_level_guard guard{current_recursion_level};

    auto name = expr.get_name(); 
    auto &function = get_function(name);
    if(!function){
        throw FunctionNotDeclaredException<T>(name, expr.get_position());
    }

    
    if(is_recursion_level_exceeded()){
        throw RecursionLimitException<T>(current_recursion_level, name, expr.get_position());
    }

    auto &parameters = function->get_parameters();
    auto &arguments = expr.get_arguments();
    std::vector<TypeIdentifier<T>> param_types{};
    for(auto &param : *parameters){
        param_types.push_back(*param->get_type());
    }
    if(parameters->size() != arguments->size()){
        throw FunctionArgumentMismatchException<T>(name, param_types, {}, arguments->size(), expr.get_position());
    }
    std::vector<TypeIdentifier<T>> argument_types{};
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
    push_context(Context<T>(arguments_evaluated));
    function->accept(*this);
    pop_context();
    return_flag = false;
}

template<CharType T>
void Interpreter<T>::visit(MatchOperation<T> &instr){
    auto &arguments = instr.get_arguments();
    if(!arguments){
        throw NullpointerException<T>();
    }

    auto &argument_values = get_current_match_arguments();

    argument_values.clear();

    for(auto &arg : *arguments){
        arg->accept(*this);
        argument_values.push_back(current_value);
    }
    auto &block = instr.get_block();
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
        auto &line = (*block)[j];
        pattern_good = true;
        auto &pattern = line->get_pattern();
        set_current_match_index(0);
        if(pattern->size() != arguments_length){
            throw InvalidPatternLengthException<T>(instr.get_position());
        }
        
        for(size_t i = 0; i < arguments_length; ++i){
            (*pattern)[i]->accept(*this);
            match_flag = true;
            if(get_current_value_type() != get_value_type(argument_values[i])){
                if(is_current_value_bool() && std::get<bool>(current_value)){
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
    auto found = function_definitions->find(name);
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
bool Interpreter<T>::is_value_bool(const value_t<T> &v){
    return std::holds_alternative<bool>(v);
}

template<CharType T>
bool Interpreter<T>::is_value_int(const value_t<T> &v){
    return std::holds_alternative<int64_t>(v);
}

template<CharType T>
bool Interpreter<T>::is_value_float(const value_t<T> &v){
    return std::holds_alternative<double>(v);
}

template<CharType T>
bool Interpreter<T>::is_value_string(const value_t<T> &v){
    return std::holds_alternative<std::basic_string<T>>(v);
}

template<CharType T>
bool Interpreter<T>::is_current_value_bool(){
    return std::holds_alternative<bool>(current_value);
}

template<CharType T>
bool Interpreter<T>::is_current_value_int(){
    return std::holds_alternative<int64_t>(current_value);
}

template<CharType T>
bool Interpreter<T>::is_current_value_float(){
    return std::holds_alternative<double>(current_value);
}

template<CharType T>
bool Interpreter<T>::is_current_value_string(){
    return std::holds_alternative<std::basic_string<T>>(current_value);
}

template<CharType T>
bool Interpreter<T>::is_current_value_of_type(Type type){
    return get_current_value_type() == type || (type == Type::File && get_current_value_type() == Type::String);
}


template<CharType T>
bool Interpreter<T>::is_variable_name_in_current_scope(const std::basic_string<T> &name){
    auto var = get_current_scope().find(name);
    return var != get_current_scope().end();
}


template<CharType T>
bool Interpreter<T>::is_variable_name_in_current_context(const std::basic_string<T> &name){
    auto &scopes = get_current_context().get_scopes();
    for(auto iter = scopes.rbegin(); iter != scopes.rend(); ++iter){
        if(iter->contains(name)){
            return true;
        }
    }
    return false;
}

template<CharType T>
bool Interpreter<T>::is_argument_of_right_type(const TypeIdentifier<T> &param_t, const TypeIdentifier<T> &arg_t){
    return (param_t.get_type() == arg_t.get_type()) && !(!param_t.get_is_const() && arg_t.get_is_const());
}

template<CharType T>
bool Interpreter<T>::is_recursion_level_exceeded(){
    return current_recursion_level > MAX_RECURSION_LEVEL;
}

template<CharType T>
void Interpreter<T>::push_context(Context<T> context){
    function_stack.push_back(context);
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
void Interpreter<T>::execute_block(std::vector<std::unique_ptr<IInstruction<T>>> &block){

    push_scope();

    for(auto &instruction : block){
        instruction->accept(*this);
        if(return_flag){
            return;
        }
    }

    pop_scope();
}


template<CharType T>
void Interpreter<T>::add_builtins(){
    std::map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>> builtins{};  //TODO??
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
    if(builtins.size() != 0){
        throw FunctionRedefinitionException<T>(builtins.begin()->first, {});
    }
}


template<CharType T>
void Interpreter<T>::run_builtin(const std::basic_string<T> &name){
    const static std::map<std::basic_string_view<T>, std::function<void(void)>> lambda_map{ //TODO
        {"print",   [&](void) -> void {std::visit(overload{
                        [&](std::monostate) -> void {this->out_stream << "None";},  //TODO: what
                        [&](auto arg)       -> void {this->out_stream << arg;}
        }, get_variable_from_current_context("text")->get_value()); return_flag = false;}},
        {"to_str_int",  [&](void) -> void {std::visit(overload{
                            [&](int64_t arg)    -> void {current_value = std::to_string(arg);},
                            [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("int_v")->get_value()); return_flag = true;}},
        {"to_str_float",    [&](void) -> void {std::visit(overload{
                                [&](double arg)     -> void {current_value = std::to_string(arg);},
                                [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("float_v")->get_value()); return_flag = true;}},
        {"to_str_bool", [&](void) -> void {std::visit(overload{
                            [&](bool arg)   -> void {current_value = arg ? "true" : "false";},
                            [&](auto)       -> void {current_value = {};}
        }, get_variable_from_current_context("bool_v")->get_value()); return_flag = true;}},
        {"to_int_float",    [&](void) -> void {std::visit(overload{
                                [&](double arg) -> void {current_value = static_cast<int64_t>(arg);},
                                [&](auto)       -> void {current_value = {};}
        }, get_variable_from_current_context("float_v")->get_value()); return_flag = true;}},
        {"to_float_int",    [&](void) -> void {std::visit(overload{
                                [&](int64_t arg)    -> void {current_value = static_cast<double>(arg);},
                                [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("int_v")->get_value()); return_flag = true;}},
        {"open_file",   [&](void) -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {   if(!open_files.contains(arg)){
                                                                            open_files.insert(std::make_pair(arg, std::fstream{arg}));
                                                                        }
                                                                        current_value = arg;
                                                                    },
                            [&](auto)                       -> void {current_value = {};}  
        }, get_variable_from_current_context("name")->get_value()); return_flag = true;}},
        {"bad_file",    [&](void) -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {current_value = !bool(open_files.at(arg)); },
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); return_flag = true;}},
        {"read_line",   [&](void) -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {std::basic_string<T> tmp{};
                                                                    current_value = std::getline(open_files.at(arg), tmp) ? tmp : "\x03";},
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); return_flag = true;}},
        {"close_file",  [&](void) -> void {std::visit(overload{
                            [&](const std::basic_string<T> &arg)   -> void {open_files.erase(arg);
                                                                    current_value = true; },
                            [&](auto)                       -> void {current_value = {};}
        }, get_variable_from_current_context("handle")->get_value()); return_flag = true;}},
        {"arguments_number",    [&](void) -> void {current_value = int64_t(program_arguments.size()); return_flag = true;}},
        {"argument",    [&](void) -> void {std::visit(overload{
                            [&](int64_t index)  -> void {const auto uindex = static_cast<size_t>(index);
                                                        if(uindex >= program_arguments.size()){throw std::runtime_error{"Index out of bounds."};}
                                                        current_value = program_arguments[uindex]; },
                            [&](auto)           -> void {current_value = {};}
        }, get_variable_from_current_context("num")->get_value()); return_flag = true;}},
    };
    auto function = lambda_map.find(name);
    if(function == lambda_map.end()){
        throw FunctionNotDeclaredException<T>(name, Position());
    }
    function->second();
}



template<CharType T>
std::unique_ptr<FunctionDefinition<T>> Interpreter<T>::get_f_d( const std::basic_string<T> &name, Type ret_type, bool ret_const,
                                                const std::initializer_list<std::basic_string<T>> &p_names, //TODO memory copying...
                                                const std::initializer_list<Type> &p_types,
                                                const std::initializer_list<bool> &p_consts){

    auto return_t = std::make_unique<TypeIdentifier<T>>(ret_type, ret_const);
    auto parameters = std::make_unique<std::vector<std::unique_ptr<ParameterDefinition<T>>>>();
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
Type Interpreter<T>::get_value_type(const value_t<T> &v){
    return std::visit(overload{
        [](const bool){return Type::Bool;},
        [](const int64_t){return Type::Integer;},
        [](const double){return Type::Floating;},
        [](const std::basic_string<T>&){return Type::String;},
        [](std::monostate){return Type::Void;},
    }, v);
}

template<CharType T>
Type Interpreter<T>::get_current_value_type(){
    return get_value_type(current_value);
}

template<CharType T>
Scope<T> &Interpreter<T>::get_current_scope(){
    auto ptr = get_current_context().get_scopes().rbegin();
    if(ptr == get_current_context().get_scopes().rend()){
        throw std::runtime_error("No scope created.");
    }
    return *ptr;
}


template<CharType T>
Context<T> &Interpreter<T>::get_current_context(){
    auto ptr = function_stack.rbegin();
    if(ptr == function_stack.rend()){
        throw std::runtime_error("No context created.");
    }
    return *ptr;
}

template<CharType T>
size_t Interpreter<T>::get_current_match_index(){
    return get_current_context().get_match_index();
}

template<CharType T>
void Interpreter<T>::set_current_match_index(size_t i){
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
bool Interpreter<T>::is_expression_match(ExpressionType type) const{
    return match_expression_type_map.find(type) != match_expression_type_map.cend();
}

template<CharType T>
ExpressionType Interpreter<T>::map_from_match(ExpressionType type) const {
    return match_expression_type_map.at(type);
}

template<CharType T>
value_t<T> Interpreter<T>::get_current_match_argument(){
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
template class Resolver<char, int64_t, int64_t>;
template class Resolver<char, int64_t, double>;
template class Resolver<char, double, int64_t>;
template class Resolver<char, double, double>;