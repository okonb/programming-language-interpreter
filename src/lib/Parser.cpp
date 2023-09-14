#include "Parser.hpp"

#include "Instructions.hpp"
#include "Expressions.hpp"
#include "Program.hpp"
#include <optional>


template<CharType T>
constexpr light_map<TokenType, Type, 6UL> ParserBase<T>::type_map{
    std::array<std::pair<TokenType, Type>, 6UL>{{
    {TokenType::Integer_type,   Type::Integer},
    {TokenType::Floating_type,  Type::Floating},
    {TokenType::String_type,    Type::String},
    {TokenType::File_type,      Type::File},
    {TokenType::Bool_type,      Type::Bool},
    {TokenType::Void_type,      Type::Void},
    }}
};

template<CharType T>
constexpr light_map<TokenType, ExpressionType, 19UL> ParserBase<T>::expression_type_map{
    std::array<std::pair<TokenType, ExpressionType>, 19UL>{{
    {TokenType::Gt,                 ExpressionType::GtExpression},
    {TokenType::Gte,                ExpressionType::GteExpression},
    {TokenType::Lt,                 ExpressionType::LtExpression},
    {TokenType::Lte,                ExpressionType::LteExpression},
    {TokenType::Equals,             ExpressionType::EqualsExpression},
    {TokenType::Not_equals,         ExpressionType::NotEqualsExpression},
    {TokenType::Plus,               ExpressionType::PlusExpression},
    {TokenType::Minus,              ExpressionType::MinusExpression},
    {TokenType::Multiplication,     ExpressionType::MultiplicationExpression},
    {TokenType::Division,           ExpressionType::DivisionExpression},
    {TokenType::Modulo,             ExpressionType::ModuloExpression},
    {TokenType::String_concat,      ExpressionType::StrConcatExpression},
    {TokenType::And,                ExpressionType::AndExpression},
    {TokenType::Or,                 ExpressionType::OrExpression},
    {TokenType::Not,                ExpressionType::NotExpression},
    {TokenType::Integer_literal,    ExpressionType::IntegerLiteralExpression},
    {TokenType::Floating_literal,   ExpressionType::FloatingLiteralExpression},
    {TokenType::String_literal,     ExpressionType::StringLiteralExpression},
    {TokenType::Boolean_literal,    ExpressionType::BooleanLiteralExpression},
    }}
};

template<CharType T>
constexpr light_map<ExpressionType, ExpressionType, 15UL> ParserBase<T>::match_expression_type_map{
    std::array<std::pair<ExpressionType, ExpressionType>, 15UL>{{
    {ExpressionType::GtExpression,              ExpressionType::MatchGtExpression},
    {ExpressionType::GteExpression,             ExpressionType::MatchGteExpression},
    {ExpressionType::LtExpression,              ExpressionType::MatchLtExpression},
    {ExpressionType::LteExpression,             ExpressionType::MatchLteExpression},
    {ExpressionType::EqualsExpression,          ExpressionType::MatchEqualsExpression},
    {ExpressionType::NotEqualsExpression,       ExpressionType::MatchNotEqualsExpression},
    {ExpressionType::NotExpression,             ExpressionType::MatchNotExpression},
    {ExpressionType::PlusExpression,            ExpressionType::MatchPlusExpression},
    {ExpressionType::MinusExpression,           ExpressionType::MatchMinusExpression},
    {ExpressionType::MultiplicationExpression,  ExpressionType::MatchMultiplicationExpression},
    {ExpressionType::DivisionExpression,        ExpressionType::MatchDivisionExpression},
    {ExpressionType::ModuloExpression,          ExpressionType::MatchModuloExpression},
    {ExpressionType::StrConcatExpression,       ExpressionType::MatchStrConcatExpression},
    {ExpressionType::AndExpression,             ExpressionType::MatchAndExpression},
    {ExpressionType::OrExpression,              ExpressionType::MatchOrExpression},
    }}
};

// start_symbol = {function_definition};
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> Parser<T>::parse(){
    auto function_definitions = std::make_unique<std::vector<std::unique_ptr<FunctionDefinition<T>>>>();
    while(auto function_definiton = this->try_parse_function_definition()){
        function_definitions->push_back(std::move(function_definiton));
    }
    return function_definitions;
}

// function_definition = Function_keywd, Identifier, Opening_parenth, parameter_list_definition, Closing_parenth,
//                       Colon, (type_identifier | Void_type), code_block;
template<CharType T>
std::unique_ptr<FunctionDefinition<T>> ParserBase<T>::try_parse_function_definition(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::Function_keywd)){
        return nullptr;
    }

    expect(TokenType::Identifier);
    const auto function_name = get_string_from_current();

    advance_token();
    
    expect_and_advance(TokenType::Opening_parenth);

    auto parameter_list = try_parse_parameter_list_definition();

    expect_and_advance(TokenType::Closing_parenth);
    expect_and_advance(TokenType::Colon);

    auto function_type = try_parse_type_identifier();
    expect_not_null(function_type, "type_identifier");

    auto block = try_parse_code_block();
    expect_not_null(block, "code_block");

    return std::make_unique<FunctionDefinition<T>>(function_name, std::move(function_type), std::move(parameter_list), std::move(block), start_position);
}

// parameter_list_definition = [parameter_definition, {Comma, parameter_definition}];
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> ParserBase<T>::try_parse_parameter_list_definition(){
    auto parameter = try_parse_parameter_definition();
    auto parameter_list = std::make_unique<std::vector<std::unique_ptr<ParameterDefinition<T>>>>();
    if(!parameter){
        return parameter_list;
    }
    parameter_list->push_back(std::move(parameter));
    while(check_and_advance(TokenType::Comma)){
        parameter = try_parse_parameter_definition();
        expect_not_null(parameter, "parameter_definition");
        parameter_list->push_back(std::move(parameter));
    }
    return parameter_list;
}

// parameter_definition = Identifier, Colon, type_identifier;
template<CharType T>
std::unique_ptr<ParameterDefinition<T>> ParserBase<T>::try_parse_parameter_definition(){
    if(!is_current_token_of_type(TokenType::Identifier)){
        return nullptr;
    }
    const auto parameter_name = get_string_from_current();
    advance_token();

    expect_and_advance(TokenType::Colon);

    auto parameter_type = try_parse_type_identifier();
    if(!parameter_type || parameter_type->get_type() == Type::Void){
        throw get_syntax_error_exception("type_identifier");
    }
    return std::make_unique<ParameterDefinition<T>>(std::move(parameter_type), parameter_name);
}

// type_identifier = [Const_keywd], (Integer_type  |
//                                   Floating_type |
//                                   String_type   |
//                                   File_type     |
//                                   Bool_type     );
template<CharType T>
std::unique_ptr<TypeIdentifier<T>> ParserBase<T>::try_parse_type_identifier(){
    const bool is_const = check_and_advance(TokenType::Const_keywd);
    if(is_const && !is_current_token_a_type()){
        throw get_unexpected_token_exception({TokenType::Integer_type, 
                                              TokenType::Floating_type,
                                              TokenType::String_type,  
                                              TokenType::File_type,    
                                              TokenType::Bool_type});
    }
    if(!is_current_token_a_type()){
        return nullptr;
    }
    auto type = std::make_unique<TypeIdentifier<T>>(map_to_type(current_token.get_type()), is_const);
    advance_token();
    return type;
}

//code_block = Opening_curly, {statement_or_control_block}, Closing_curly;
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> ParserBase<T>::try_parse_code_block(){
    if(!check_and_advance(TokenType::Opening_curly)){
        return nullptr;
    }
    auto instructions = std::make_unique<std::vector<std::unique_ptr<IInstruction<T>>>>();
    while(auto stmt_or_ctrl = try_parse_statement_or_control_block()){
        instructions->push_back(std::move(stmt_or_ctrl));
    }

    expect_and_advance(TokenType::Closing_curly);

    return instructions;
}

// statement_or_control_block = statement | control_block;
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_statement_or_control_block(){
    if(auto statement = try_parse_statement()){
        return statement;
    }
    
    if(auto control_block = try_parse_control_block()){
        return control_block;
    }
    
    return nullptr;    
}

// statement = (var_def_assign_or_funcall | return_statement), Semicolon;
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_statement(){
    if(auto var_def_assign_or_funcall = try_parse_var_def_assign_or_funcall()){
        expect_and_advance(TokenType::Semicolon);
        return var_def_assign_or_funcall;
    }
    
    if(auto return_statement = try_parse_return_statement()){
        expect_and_advance(TokenType::Semicolon);
        return return_statement;
    }
    return nullptr;
}

// var_def_assign_or_funcall = Identifier, (([Colon, type_identifier], Assign, (expression | match_operation)) | parenths_and_args);
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_var_def_assign_or_funcall(){
    const auto start_position = get_current_position();
    if(!is_current_token_of_type(TokenType::Identifier)){
        return nullptr;
    }
    const auto name = get_string_from_current();
    advance_token();
    if(auto arguments = try_parse_parenths_and_args()){
        return std::make_unique<FunctionCall<T>>(name, std::move(arguments), start_position);
    }

    std::unique_ptr<TypeIdentifier<T>> type{nullptr};
    if(check_and_advance(TokenType::Colon)){
        type = try_parse_type_identifier();
        if(!type || type->get_type() == Type::Void){
            throw get_syntax_error_exception("type_identifier");
        }
    }

    expect_and_advance(TokenType::Assign);

    auto rest = try_parse_expression();
    if(!rest){
        rest = try_parse_match_operation();
    }

    expect_not_null(rest, "expression");

    if(type){
        return std::make_unique<VarDefinitionInstruction<T>>(type, name, std::move(rest), start_position);
    }
    return std::make_unique<AssignmentInstruction<T>>(name, std::move(rest), start_position);
}

// parenths_and_args = Opening_parenth, argument_list, Closing_parenth;
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> ParserBase<T>::try_parse_parenths_and_args(){
    if(!check_and_advance(TokenType::Opening_parenth)){
        return nullptr;
    }
    auto arguments = try_parse_argument_list();

    expect_and_advance(TokenType::Closing_parenth);

    return arguments;
}

// argument_list = [expression, {Comma, expression}];
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> ParserBase<T>::try_parse_argument_list(){
    auto arguments = std::make_unique<std::vector<std::unique_ptr<IExpression<T>>>>();
    auto argument = try_parse_expression();
    if(!argument){
        return arguments;
    }
    arguments->push_back(std::move(argument));
    while(check_and_advance(TokenType::Comma)){
        argument = try_parse_expression();
        expect_not_null(argument, "expression");

        arguments->push_back(std::move(argument));
    }
    return arguments;
}

// control_block = if_block | while_block | match_operation;
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_control_block(){
    if(auto if_block = try_parse_if_block()){
        return if_block;
    }
    if(auto while_block = try_parse_while_block()){
        return while_block;
    }
    if(auto match_operation = try_parse_match_operation()){
        return match_operation;
    }
    return nullptr;
}

//if_block = If_keywd, Opening_parenth, expression, Closing_parenth, code_block, [else_block];
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_if_block(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::If_keywd)){
        return nullptr;
    }
    auto condition = try_parse_condition();
    expect_not_null(condition, "condition");

    auto code_block = try_parse_code_block();
    expect_not_null(code_block, "code_block");

    auto else_block = try_parse_else_block();
    return std::make_unique<IfInstruction<T>>(std::move(condition), std::move(code_block), std::move(else_block), start_position);
}

// else_block = Else_keywd, (if_block | code_block);
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_else_block(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::Else_keywd)){
        return nullptr;
    }
    if(auto if_st = try_parse_if_block()){
        return if_st;
    }
    auto code_block = try_parse_code_block();
    expect_not_null(code_block, "code_block");

    return std::make_unique<IfInstruction<T>>(nullptr, std::move(code_block), nullptr, start_position);
}

// while_block = While_keywd, condition, code_block;
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_while_block(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::While_keywd)){
        return nullptr;
    }
    auto condition = try_parse_condition();
    expect_not_null(condition, "condition");

    auto code_block = try_parse_code_block();
    expect_not_null(code_block, "code_block");

    return std::make_unique<WhileInstruction<T>>(std::move(condition), std::move(code_block), start_position);
}

// condition = Opening_parenth, expression, Closing_parenth;
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_condition(){
    if(!check_and_advance(TokenType::Opening_parenth)){
        return nullptr;
    }
    auto expression = try_parse_expression();
    expect_not_null(expression, "expression");


    expect_and_advance(TokenType::Closing_parenth);

    return expression;
}

// expression = logic_factor, {Or, logic_factor};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_expression(){
    const auto start_position = get_current_position();
    auto logic_factor_l = try_parse_logic_factor();
    if(!logic_factor_l){
        return nullptr;
    }
    while(check_and_advance(TokenType::Or)){
        auto logic_factor_r = try_parse_logic_factor();
        expect_not_null(logic_factor_r, "logic_factor");

        logic_factor_l = std::make_unique<TwoArgExpression<T>>(ExpressionType::OrExpression, std::move(logic_factor_l), std::move(logic_factor_r), start_position);
    }
    return logic_factor_l;
}

// logic_factor = relation, {And, relation};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_logic_factor(){
    const auto start_position = get_current_position();
    auto relation_l = try_parse_relation();
    if(!relation_l){
        return nullptr;
    }
    while(check_and_advance(TokenType::And)){
        auto relation_r = try_parse_relation();
        expect_not_null(relation_r, "relation");

        relation_l = std::make_unique<TwoArgExpression<T>>(ExpressionType::AndExpression, std::move(relation_l), std::move(relation_r), start_position);
    }
    return relation_l;
}

// relation = math_expression, [relation_operator, math_expression];
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_relation(){
    const auto start_position = get_current_position();
    auto math_expression_l = try_parse_math_expression();
    if(!math_expression_l){
        return nullptr;
    }
    if(is_current_token_relation_operator()){
        const auto type = map_current_to_expression_type();
        advance_token();
        auto math_expression_r = try_parse_math_expression();
        expect_not_null(math_expression_r, "math_expression");

        math_expression_l = std::make_unique<TwoArgExpression<T>>(type, std::move(math_expression_l), std::move(math_expression_r), start_position);
    }
    return math_expression_l;
}

// math_expression = factor, {(Plus | Minus | String_concat), factor};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_math_expression(){
    const auto start_position = get_current_position();
    auto factor_l = try_parse_factor();
    if(!factor_l){
        return nullptr;
    }
    while(is_current_token_additive_operator()){
        const auto type = map_current_to_expression_type();
        advance_token();
        auto factor_r = try_parse_factor();
        expect_not_null(factor_r, "factor");

        factor_l = std::make_unique<TwoArgExpression<T>>(type, std::move(factor_l), std::move(factor_r), start_position);
    }

    return factor_l;
}

// factor = term, {(Multiplication | Division | Modulo), term};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_factor(){
    const auto start_position = get_current_position();
    auto term_l = try_parse_term();
    if(!term_l){
        return nullptr;
    }
    while(is_current_token_multiplicative_operator()){
        const auto type = map_current_to_expression_type();
        advance_token();
        auto term_r = try_parse_term();
        expect_not_null(term_r, "term");

        term_l = std::make_unique<TwoArgExpression<T>>(type, std::move(term_l), std::move(term_r), start_position);
    }

    return term_l;
}

// term = [Not | Minus], (literal
//                      | identifier_or_funcall
//                      | ( Opening_parenth, expression, Closing_parenth));
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_term(){
    const auto start_position = get_current_position();
    std::optional<ExpressionType> unary_expr_type{};
    if(check_and_advance(TokenType::Not)){
        unary_expr_type = ExpressionType::NotExpression;
    }
    else if(check_and_advance(TokenType::Minus)){
        unary_expr_type = ExpressionType::NegateNumberExpression;
    }

    std::unique_ptr<IExpression<T>> term{nullptr};
    
    term = try_parse_literal();
    if(!term){
        term = try_parse_identifier_or_funcall();
    }
    if(!term && check_and_advance(TokenType::Opening_parenth)){
        term = try_parse_expression();
        if(!term || !check_and_advance(TokenType::Closing_parenth)){
            throw get_syntax_error_exception("(expression)");
        }
    }
    
    if(unary_expr_type){
        if(term){
            term = std::make_unique<SingleArgExpression<T>>(*unary_expr_type, std::move(term), start_position);
        }
        else{
            throw get_syntax_error_exception("term");
        }
    }

    return term;
}

// match_expression = match_logic_factor, {Or, match_logic_factor};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_expression(){
    const auto start_position = get_current_position();
    auto logic_factor_l = try_parse_match_logic_factor();
    if(!logic_factor_l){
        return nullptr;
    }
    while(check_and_advance(TokenType::Or)){
        auto logic_factor_r = try_parse_match_logic_factor();
        expect_not_null(logic_factor_r, "match_logic_factor");

        logic_factor_l = std::make_unique<TwoArgExpression<T>>(ExpressionType::MatchOrExpression, std::move(logic_factor_l), std::move(logic_factor_r), start_position);
    }
    return logic_factor_l;
}

// match_logic_factor = match_relation, {And, match_relation};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_logic_factor(){
    const auto start_position = get_current_position();
    auto relation_l = try_parse_match_relation();
    if(!relation_l){
        return nullptr;
    }
    while(check_and_advance(TokenType::And)){
        auto relation_r = try_parse_match_relation();
        expect_not_null(relation_r, "match_relation");

        relation_l = std::make_unique<TwoArgExpression<T>>(ExpressionType::MatchAndExpression, std::move(relation_l), std::move(relation_r), start_position);
    }
    return relation_l;
}

// match_relation = relation_operator, match_math_expression;
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_relation(){
    const auto start_position = get_current_position();
    if(!is_current_token_relation_operator()){
        return nullptr;
    }
    const auto type = map_current_to_match_expression_type();
    advance_token();
    auto math_expression = try_parse_match_math_expression();
    expect_not_null(math_expression, "match_math_expression");

    return std::make_unique<SingleArgExpression<T>>(type, std::move(math_expression), start_position);
}

// match_math_expression = match_factor, {(Plus | Minus | String_concat), match_factor};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_math_expression(){
    const auto start_position = get_current_position();
    auto factor_l = try_parse_match_factor();
    if(!factor_l){
        return nullptr;
    }
    while(is_current_token_additive_operator()){
        const auto type = map_current_to_match_expression_type();
        advance_token();
        auto factor_r = try_parse_match_factor();
        expect_not_null(factor_r, "match_factor");

        factor_l = std::make_unique<TwoArgExpression<T>>(type, std::move(factor_l), std::move(factor_r), start_position);
    }

    return factor_l;
}

// match_factor  = match_term, {(Multiplication | Division | Modulo), match_term};
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_factor(){
    const auto start_position = get_current_position();
    auto term_l = try_parse_match_term();
    if(!term_l){
        return nullptr;
    }
    while(is_current_token_multiplicative_operator()){
        const auto type = map_current_to_match_expression_type();
        advance_token();
        auto term_r = try_parse_match_term();
        expect_not_null(term_r, "match_term");

        term_l = std::make_unique<TwoArgExpression<T>>(type, std::move(term_l), std::move(term_r), start_position);
    }
    return term_l;
}

// match_term = [Not | Minus], (literal
//                            | identifier_or_funcall
//                            | (Opening_parenth, match_expression, Closing_parenth));

template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_term(){
    const auto start_position = get_current_position();
    std::optional<ExpressionType> unary_expr_type{};
    if(check_and_advance(TokenType::Not)){
        unary_expr_type = ExpressionType::MatchNotExpression;
    }
    else if(check_and_advance(TokenType::Minus)){
        unary_expr_type = ExpressionType::MatchNegateNumberExpression;
    }
    
    std::unique_ptr<IExpression<T>> term = nullptr;
    
    term = try_parse_literal();
    if(!term){
        term = try_parse_identifier_or_funcall();
    }
    if(!term && check_and_advance(TokenType::Opening_parenth)){
        term = try_parse_match_expression();
        if(!term || !check_and_advance(TokenType::Closing_parenth)){
            throw get_syntax_error_exception("(match_expression)");
        }
    }

    if(unary_expr_type){
        if(term){
            term = std::make_unique<SingleArgExpression<T>>(*unary_expr_type, std::move(term), start_position);
        }
        else{
            throw get_syntax_error_exception("match_term");
        }
    }

    return term;
}

// literal = Integer_literal | Floating_literal | String_literal | Boolean_literal;
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_literal(){
    const auto start_position = get_current_position();
    if(is_current_token_a_literal()){
        const auto value = current_token.get_value();
        const auto type = map_current_to_expression_type();
        advance_token();
        return std::make_unique<LiteralExpression<T>>(type, value, start_position);
    }

    return nullptr;
}

// identifier_or_funcall = Identifier, [parenths_and_args];
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_identifier_or_funcall(){
    const auto start_position = get_current_position();
    if(!is_current_token_of_type(TokenType::Identifier)){
        return nullptr;
    }
    const auto identifier = get_string_from_current();
    advance_token();
    auto parenths_and_args = try_parse_parenths_and_args();
    if(parenths_and_args){
        return std::make_unique<FunctionCall<T>>(identifier, std::move(parenths_and_args), start_position);
    }
    return std::make_unique<IdentifierExpression<T>>(identifier, start_position);
}

// return_statement = Return_keywd, [expression | match_operation];
template<CharType T>
std::unique_ptr<IInstruction<T>> ParserBase<T>::try_parse_return_statement(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::Return_keywd)){
        return nullptr;
    }
    if(auto expression = try_parse_expression()){
        return std::make_unique<ReturnInstruction<T>>(std::move(expression), start_position);
    }
    if(auto match_operation = try_parse_match_operation()){
        return std::make_unique<ReturnInstruction<T>>(std::move(match_operation), start_position);
    }
    //TODO production handled by try_parse_statement handles this somewhat... no peeking!
    if(is_current_token_of_type(TokenType::Semicolon)){
        return std::make_unique<ReturnInstruction<T>>(nullptr, start_position);
    }
    throw get_syntax_error_exception("expression | match_operation");
}

// match_operation  = Match_keywd, Opening_parenth, expression, {Comma, expression}, Closing_parenth, match_block;
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_match_operation(){
    const auto start_position = get_current_position();
    if(!check_and_advance(TokenType::Match_keywd)){
        return nullptr;
    }
    auto args = try_parse_parenths_and_args();
    expect_not_null(args, "parenths_and_args");

    if(args->size() == 0UL){
        throw get_syntax_error_exception(">0 arguments");
    }
    auto match_block = try_parse_match_block();
    expect_not_null(match_block, "match_block");

    return std::make_unique<MatchOperation<T>>(std::move(args), std::move(match_block), start_position);
}

// match_block = Opening_curly, match_line, {Comma, match_line}, Closing_curly;
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<MatchLine<T>>>> ParserBase<T>::try_parse_match_block(){
    if(!check_and_advance(TokenType::Opening_curly)){
        return nullptr;
    }
    auto match_block = std::make_unique<std::vector<std::unique_ptr<MatchLine<T>>>>();
    auto match_line = try_parse_match_line();
    expect_not_null(match_line, "match_line");

    match_block->push_back(std::move(match_line));
    while(check_and_advance(TokenType::Comma)){
        match_line = try_parse_match_line();
        expect_not_null(match_line, "match_line");

        match_block->push_back(std::move(match_line));
    }

    expect_and_advance(TokenType::Closing_curly);

    return match_block;
}

// match_line = pattern, Colon, expression;
template<CharType T>
std::unique_ptr<MatchLine<T>> ParserBase<T>::try_parse_match_line(){
    auto pattern = try_parse_pattern();
    if(!pattern){
        return nullptr;
    }

    expect_and_advance(TokenType::Colon);

    auto expression = try_parse_expression();
    expect_not_null(expression, "expression");

    return std::make_unique<MatchLine<T>>(std::move(pattern), std::move(expression));

}

// pattern = pattern_element, {Comma, pattern_element};
template<CharType T>
std::unique_ptr<std::vector<std::unique_ptr<IExpression<T>>>> ParserBase<T>::try_parse_pattern(){
    auto pattern_elem = try_parse_pattern_element();
    if(!pattern_elem){
        return nullptr;
    }
    auto pattern = std::make_unique<std::vector<std::unique_ptr<IExpression<T>>>>();
    pattern->push_back(std::move(pattern_elem));
    while(check_and_advance(TokenType::Comma)){
        pattern_elem = try_parse_pattern_element();
        expect_not_null(pattern_elem, "pattern_element");

        pattern->push_back(std::move(pattern_elem));
    }
    return pattern;
}

// pattern_element = expression | match_expression | Underscore;    TODO wut match_expression how
template<CharType T>
std::unique_ptr<IExpression<T>> ParserBase<T>::try_parse_pattern_element(){
    const auto start_position = get_current_position();
    if(auto match_expr = try_parse_match_expression()){
        return match_expr;
    }
    if(auto expr = try_parse_expression()){
        return expr;
    }
    if(check_and_advance(TokenType::Underscore)){
        return std::make_unique<SingleArgExpression<T>>(ExpressionType::UnderscoreExpression, nullptr, start_position);
    }
    return nullptr;
}


template<CharType T>
UnexpectedTokenException<T> ParserBase<T>::get_unexpected_token_exception(const std::initializer_list<TokenType> &types, const std::source_location &location) const {
    return UnexpectedTokenException<T>(location.function_name(), current_token, types);
}

template<CharType T>
SyntaxErrorException<T> ParserBase<T>::get_syntax_error_exception(const std::basic_string_view<T> text, const std::source_location &location) const {
    return SyntaxErrorException<T>(location.function_name(), current_token, text);
}

template<CharType T>
Type ParserBase<T>::map_to_type(TokenType type) const {
    return type_map.at(type);
}

template<CharType T>
ExpressionType ParserBase<T>::map_to_expression_type(TokenType type) const {
    return expression_type_map.at(type);
}

template<CharType T>
ExpressionType ParserBase<T>::map_to_match(ExpressionType type) const {
    return match_expression_type_map.at(type);
}

template <CharType T>
ExpressionType ParserBase<T>::map_current_to_expression_type() const {
    return map_to_expression_type(current_token.get_type());
}

template <CharType T>
ExpressionType ParserBase<T>::map_current_to_match_expression_type() const {
    return map_to_match(map_current_to_expression_type());
}

template<CharType T>
bool ParserBase<T>::is_current_token_a_type() const {
    return type_map.contains(current_token.get_type());
}

template<CharType T>
void ParserBase<T>::advance_token(){
    current_token = lexer.get_next_token();
}

template<CharType T>
Position ParserBase<T>::get_current_position() const{
    return current_token.get_position();
}

template<CharType T>
bool ParserBase<T>::is_current_token_of_type(TokenType type) const {
    return current_token.get_type() == type;
}

template<CharType T>
bool ParserBase<T>::is_current_token_additive_operator() const {
    const TokenType ttype = current_token.get_type();
    return (ttype == TokenType::Plus ||
            ttype == TokenType::Minus ||
            ttype == TokenType::String_concat);
}

template<CharType T>
bool ParserBase<T>::is_current_token_multiplicative_operator() const {
    const TokenType ttype = current_token.get_type();
    return (ttype == TokenType::Multiplication ||
            ttype == TokenType::Division ||
            ttype == TokenType::Modulo);
}

template<CharType T>
bool ParserBase<T>::is_current_token_relation_operator() const {
    const TokenType ttype = current_token.get_type();
    return (ttype == TokenType::Gt ||
            ttype == TokenType::Gte ||
            ttype == TokenType::Lt ||
            ttype == TokenType::Lte ||
            ttype == TokenType::Equals ||
            ttype == TokenType::Not_equals);
}

template<CharType T>
bool ParserBase<T>::is_current_token_a_literal() const {
    const TokenType ttype = current_token.get_type();
    return (ttype == TokenType::Integer_literal ||
            ttype == TokenType::Floating_literal ||
            ttype == TokenType::Boolean_literal ||
            ttype == TokenType::String_literal);
}

template <CharType T>
std::basic_string<T> ParserBase<T>::get_string_from_current() const
{
    return std::get<std::basic_string<T>>(current_token.get_value());
}

template<CharType T>
bool ParserBase<T>::check_and_advance(TokenType type){
    if(!is_current_token_of_type(type)){
        return false;
    }
    advance_token();
    return true;
}

template<CharType T>
void ParserBase<T>::expect(TokenType type, const std::source_location &location) const {
    if(!is_current_token_of_type(type)){
        throw get_unexpected_token_exception({type}, location);
    }
}

template<CharType T>
void ParserBase<T>::expect_and_advance(TokenType type, const std::source_location &location){
    if(!check_and_advance(type)){
        throw get_unexpected_token_exception({type}, location);
    }
}

template<CharType T>
template<typename P>
void ParserBase<T>::expect_not_null(std::unique_ptr<P> &pointer, const std::basic_string_view<T> err_message, const std::source_location &location) const {
    if(!pointer){
        throw get_syntax_error_exception(err_message, location);
    }
}

template class ParserBase<char>;
//template class ParserBase<wchar_t>;
template class Parser<char>;
//template class Parser<wchar_t>;