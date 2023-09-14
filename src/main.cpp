#include "Parser.hpp"
#include "CommentFilterLexer.hpp"
#include "Interpreter.hpp"
#include "NumericType.hpp"
#include "overload.hpp"
#include "ProgramTreePrinter.hpp"
#include <iostream>
#include <sstream>
#include <iterator>
#include <span>

constexpr bool PRINT_TREE_BEFORE_EXECUTION = true;


using namespace std::literals;


int main(int argc, char** argv){

    const auto main_args_size = static_cast<std::size_t>(argc);
    const auto main_args = std::span{argv, main_args_size};


    if(main_args_size < 2){
        std::cerr << "Please provide program filename as the argument.\n";
        return 1;
    }
    
    std::ifstream infile{main_args[1]};
    infile.unsetf(std::ios::skipws);

    std::vector<std::string> arguments{};

    std::size_t arg_index = 2;

    //skip additional program arguments
    while(arg_index < main_args_size and "--"s != main_args[arg_index]){
        ++arg_index;
    }
    //skip --
    ++arg_index;

    while(arg_index < main_args_size){
        arguments.emplace_back(main_args[arg_index]);
        ++arg_index;
    }

    int return_code = 1;

    Lexer lex(infile);
    CommentFilterLexer<char> filered_lex{lex};
    Parser parser(filered_lex);
    std::unique_ptr<Program<char>> program = nullptr;

    try{
        program = std::make_unique<Program<char>>(parser.parse());
        if constexpr(PRINT_TREE_BEFORE_EXECUTION){
            ProgramTreePrinter<char>{std::cout}.print_program(*program);
        }
        Interpreter<char> inter{std::move(program), std::cout, arguments};
        return_code = static_cast<int>(inter.run());
    }
    catch(TokenizationError<char> &e){
        std::cerr << e.what() << "\nAt position " << e.get_position().line << ":" << e.get_position().column << "\n";
    }
    catch(UnexpectedTokenException<char> &e){
        std::cerr << e.what() << " in parsing function " << e.get_function_throwing_name() << ". Expected:\n";
        for(const auto &token : e.get_expected_token_list()){
            std::cerr << Lexer<char>::get_token_text(token) << ", ";
        }
        std::cerr << "\ngot: " << Lexer<char>::get_token_text(e.get_received_token().get_type()) << " at "
            << e.get_received_token().get_position().line << ":" << e.get_received_token().get_position().column << "\n";
    }
    catch(SyntaxErrorException<char> &e){
        std::cerr << e.what() << " in parsing function " << e.get_function_throwing_name() << ". Expected production:\n";
        std::cerr << e.get_expected_production();
        std::cerr << "\ngot: " << Lexer<char>::get_token_text(e.get_received_token().get_type()) << " at "
            << e.get_received_token().get_position().line << ":" << e.get_received_token().get_position().column << "\n";
    }
    catch(ProgramInitializationError<char> &e){
        std::cerr << e.what() << " " << e.get_doubled_function_name() << "\n";
    }
    catch(TokenTypeValueMismatch &e){
        std::cerr << e.what() << "\n";
    }
    catch(ConstVoidTypeException &e){
        std::cerr << e.what() << "\n";
    }

    catch(VariableAssignmentTypeMismatchException<char> &e){
        std::cerr << e.what() << " Variable name: " << e.get_variable_name() << ", type: "<< e.get_variable_type().get_str_representation() << "\n";
        std::cerr << "Tried assigning " << TypeIdentifier<char>::get_type_text(e.get_value_type()) << " instead, at " <<e.get_position().line << ":" << e.get_position().column << "\n";
    }

    catch(ReturnValueTypeMismatchException<char> &e){
        std::cerr << e.what() << " Function name: " << e.get_function_name() << ", type: "<< e.get_return_type().get_str_representation() << "\n";
        std::cerr << "Tried returning " << TypeIdentifier<char>::get_type_text(e.get_value_type()) << " instead, at " <<e.get_position().line << ":" << e.get_position().column << "\n";
    }

    catch(FunctionArgumentMismatchException<char> &e){
        std::cerr << e.what() << " Function name: " << e.get_function_name() << ", parameter types:\n";
        for(const auto &type : e.get_expected_type_list()){
            std::cerr << type.get_str_representation() << ", ";
        }
        std::cerr << "\nGot instead:\n";
        for(const auto &type : e.get_gotten_type_list()){
            std::cerr << type.get_str_representation() << ", ";
        }
        std::cerr << "\nAt " <<e.get_position().line << ":" << e.get_position().column << "\n";
    }

    catch(OperatorArgumentMismatchException<char> &e){
        std::cerr << e.what() << " Oparator name: " << e.get_operator_name() << ", left expected types:\n";
        for(const auto &type : e.get_expected_type_list_left()){
            std::cerr << TypeIdentifier<char>::get_type_text(type) << ", ";
        }
        std::cerr << "\nright expected types:\n";
        for(const auto &type : e.get_expected_type_list_right()){
            std::cerr << TypeIdentifier<char>::get_type_text(type) << ", ";
        }
        std::cerr << "\ngotten types:\n";
        for(const auto &type : e.get_gotten_type_list()){
            std::cerr << TypeIdentifier<char>::get_type_text(type) << ", ";
        }
        std::cerr << "\nAt " <<e.get_position().line << ":" << e.get_position().column << "\n";
    }

    catch(SimpleTextException<char> &e){
        std::cerr << e.what() << " In " << e.get_name() << " at " << e.get_position().line << ":" <<  e.get_position().column << ".\n";
    }
    catch(SimpleException<char> &e){
        std::cerr << e.what() << " At " << e.get_position().line << ":" <<  e.get_position().column << ".\n";
    }
    catch(std::exception &e){
        std::cerr << e.what();
    }
    catch(...){
        std::cerr << "unknown exception\n";
    }
    return return_code;
}