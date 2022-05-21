#ifndef CHAR_TYPE_HPP
#define CHAR_TYPE_HPP

#include <concepts>

template<typename T>
concept CharType = std::same_as<T, char> or std::same_as<T, wchar_t>;

#endif // !CHAR_TYPE_HPP