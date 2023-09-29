#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "FunctionDefinition.hpp"
#include "CharType.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>
#include <stdexcept>


template<CharType T>
class ProgramInitializationError : public std::runtime_error{
public:
    ProgramInitializationError(const std::basic_string<T> &error_msg, const std::basic_string_view<T> function_name = "") :
        std::runtime_error{error_msg}, doubled_function{function_name} {}
    const std::basic_string<T> &get_doubled_function_name() const {return doubled_function;}
private:
    const std::basic_string<T> doubled_function;
};

// cannot have multiple functions of same name
template<CharType T = char>
class Program{
public:
    Program(std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> functions) :
        function_definitions{std::make_unique<std::unordered_map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>>>()} {
        
        if(!functions){
            throw ProgramInitializationError<T>("Null pointer provided!");
        }

        for(auto &function : *functions){
            const auto name = function->get_name();
            if(function_definitions->contains(name)){
                throw ProgramInitializationError<T>("Double function definition!", name);
            }
            function_definitions->insert(std::make_pair(name, std::move(function)));
        }
    }
    const std::unique_ptr<std::unordered_map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>>> &get_function_definitions() const {
        return function_definitions;
    };
    std::unique_ptr<std::unordered_map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>>> give_function_definitions(){
        return std::move(function_definitions);
    }
    
private:
    std::unique_ptr<std::unordered_map<std::basic_string<T>, std::unique_ptr<FunctionDefinition<T>>>> function_definitions;
};

#endif // PROGRAM_HPP