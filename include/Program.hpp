#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <map>
#include <string>
#include "FunctionDefinition.hpp"


template<typename T = char>
using FunctionDefinitionsMap = std::map<std::basic_string<T>, FunctionDefinition<T>>;

template<typename T = char>
class Program{
public:
    Program(FunctionDefinitionsMap<T> &map) : function_definitions{map} {}
    FunctionDefinition<T> &get_function(const std::basic_string<T> &key);
    void set_function(const std::basic_string<T> &key, FunctionDefinition<T> definition);
private:
    FunctionDefinitionsMap<T> function_definitions;
};

#endif // PROGRAM_HPP