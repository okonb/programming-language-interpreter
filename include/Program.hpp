#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "FunctionDefinition.hpp"
#include "ISelfPrintable.hpp"
#include "CharType.hpp"
#include <vector>
#include <string>
#include <sstream>

template<CharType T = char>
class Program : public ISelfPrintable<T>{
public:
    Program(std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> functions) : function_definitions{std::move(functions)} {}
    const std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> &get_function_definitions() const{
        return function_definitions;
    };
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "Program\n";
        for(const auto &function : *function_definitions){
            function->print_self(stream, level + 1);
        }
        stream << "\n";
    }
private:
    std::unique_ptr<std::vector<std::unique_ptr<FunctionDefinition<T>>>> function_definitions;
};

#endif // PROGRAM_HPP