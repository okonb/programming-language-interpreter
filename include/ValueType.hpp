#ifndef VALUE_TYPE_HPP
#define VALUE_TYPE_HPP

#include "CharType.hpp"
#include <variant>
#include <string>
#include <cstdint>

template<CharType T>
using value_t = std::variant<std::monostate, std::basic_string<T>, int64_t, double, bool>;


#endif