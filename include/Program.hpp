#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "FunctionDefinition.hpp"
#include "ISelfPrintable.hpp"
#include "CharType.hpp"
#include <map>
#include <string>
#include <sstream>

template<CharType T = char>
using FunctionDefinitionsMap = std::map<std::basic_string<T>, FunctionDefinition<T>>;

template<CharType T = char>
class Program : public ISelfPrintable<T>{
public:
    Program(FunctionDefinitionsMap<T> &&map) : function_definitions{std::move(map)} {}
    FunctionDefinition<T> &get_function(const std::basic_string<T> &key){
        return function_definitions.at(key);
    };
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "Program\n";
        for(const auto &[key, value] : function_definitions){
            value.print_self(stream, level + 1);
        }
        stream << "\n";
    }
private:
    FunctionDefinitionsMap<T> function_definitions;
};

#endif // PROGRAM_HPP