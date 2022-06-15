#ifndef OVERLOAD_HPP
#define OVERLOAD_HPP

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

#endif