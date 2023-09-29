#ifndef FUNCTION_DEFINITIOIN_HPP
#define FUNCTION_DEFINITIOIN_HPP


#include "CharType.hpp"
#include "IVisitable.hpp"
#include "Instructions.hpp"
#include "TypeIdentifier.hpp"
#include "ParameterDefinition.hpp"
#include "IVisitable.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <memory>

template<CharType T = char>
class FunctionDefinition : public IVisitable<T>{
public:
    FunctionDefinition(const std::basic_string_view<T> n, std::unique_ptr<TypeIdentifier<T>> t, std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> p, std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> b, const Position &pos) :
        name{n}, return_type{std::move(t)}, parameters{std::move(p)}, block{std::move(b)}, position{pos} {}
    FunctionDefinition(FunctionDefinition<T>&&) noexcept = default;
    FunctionDefinition& operator=(FunctionDefinition<T>&&) noexcept = default;
    FunctionDefinition(const FunctionDefinition<T>&) = delete;
    FunctionDefinition& operator=(const FunctionDefinition<T>&) = delete;
    const std::basic_string<T> &get_name() const {return name;}
    const std::unique_ptr<TypeIdentifier<T>> &get_return_type() const {return return_type;}
    const std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> &get_parameters() const {return parameters;}
    const std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> &get_block() const {return block;}
    const Position &get_position() const {return position;}
    
    void accept(IVisitor<T> &visitor) const override { visitor.visit(*this); }
private:
    std::basic_string<T> name;
    std::unique_ptr<TypeIdentifier<T>> return_type;
    std::unique_ptr<std::vector<std::unique_ptr<ParameterDefinition<T>>>> parameters;
    std::unique_ptr<std::vector<std::unique_ptr<IInstruction<T>>>> block;
    Position position;
};

//template class FunctionDefinition<char>;
//template class FunctionDefinition<wchar_t>;


#endif // FUNCTION_DEFINITIOIN_HPP