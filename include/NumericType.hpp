#ifndef NUMERIC_TYPE_HPP
#define NUMERIC_TYPE_HPP

#include <concepts>
#include <cstdint>

template<typename T>
concept NumericType = std::same_as<T, int64_t> or std::same_as<T, double>;

#endif // !NUMERIC_TYPE_HPP