#ifndef OVERLOAD_HPP
#define OVERLOAD_HPP

template<typename ...Ts> struct overload : Ts... { using Ts::operator()...; };

template<typename ...Ts> overload(Ts...) -> overload<Ts...>;    //template argument deduction guide added for clang compatibility

#endif