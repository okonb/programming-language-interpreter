#ifndef FUNCTION_DEFINITIOIN_HPP
#define FUNCTION_DEFINITIOIN_HPP

#include "Instructions.hpp"
#include "TypeIdentifier.hpp"
#include "ParameterDefinition.hpp"
#include "ISelfPrintable.hpp"
#include "CharType.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <memory>

template<CharType T = char>
class FunctionDefinition : public ISelfPrintable<T>{
public:
    FunctionDefinition(const std::basic_string<T> &n, std::unique_ptr<TypeIdentifier<T>> t, std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> p, std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> b) :
        name{n}, type{std::move(t)}, parameters{std::move(p)}, block{std::move(b)} {}
    FunctionDefinition(FunctionDefinition<T>&&) noexcept = default;
    FunctionDefinition& operator=(FunctionDefinition<T>&&) noexcept = default;
    FunctionDefinition(const FunctionDefinition<T>&) = delete;
    FunctionDefinition& operator=(const FunctionDefinition<T>&) = delete;
    const std::basic_string<T> &get_name() const {return name;}
    const std::unique_ptr<TypeIdentifier<T>> &get_type() const {return type;}
    const std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> &get_parameters() const {return parameters;}
    const std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> &get_block() const {return block;}
    void print_self(std::basic_ostream<T> &stream, const size_t level = 0) const override{
        this->print_n_spaces(stream, level);
        stream << "FunctionDefinition " << name << " : " << type->get_str_representation() << "\n";
        this->print_n_spaces(stream, level + 1);
        stream << "params:\n";
        for(const auto &param : *parameters){
            param->print_self(stream, level + 2);
            stream << ",\n";
        }
        this->print_n_spaces(stream, level + 1);
        stream << "block:\n";
        for(const auto &instruction : *block){
            instruction->print_self(stream, level + 2);
        }
        this->print_n_spaces(stream, level);
        stream << "\n";
    }
private:
    std::basic_string<T> name;
    std::unique_ptr<TypeIdentifier<T>> type;
    std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> parameters;
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> block;
};

//template class FunctionDefinition<char>;
//template class FunctionDefinition<wchar_t>;


#endif // FUNCTION_DEFINITIOIN_HPP