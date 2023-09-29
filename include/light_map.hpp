#ifndef LIGHT_MAP_HPP
#define LIGHT_MAP_HPP

#include <cstdint>
#include <array>
#include <utility>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <compare>

// map with linear lookup, suited for small maps, usually well optimized by the compiler
// only features used in this project implemented
// inspired by https://www.youtube.com/watch?v=INn3xa4pMfg
template<std::three_way_comparable Key, typename Value, std::size_t Size>
class light_map{
private:
    const std::array<std::pair<Key, Value>, Size> data;
public:
    constexpr explicit light_map(std::array<std::pair<Key, Value>, Size> arr) noexcept : data{arr} {}
    [[nodiscard]]
    constexpr const Value &at(const Key &key) const{
        const auto result = find(key);
        if(result == data.cend()){
            throw std::runtime_error{"light_map::at() lookup failed"};
        }
        
        return result->second;
    }
    [[nodiscard]]
    constexpr const typename decltype(data)::const_iterator find(const Key &key) const noexcept{
        const auto result = std::find_if(this->data.cbegin(), this->data.cend(), [&key](const auto &entry){return entry.first == key;});
        return result;
    }
    [[nodiscard]]
    constexpr bool contains(const Key &key) const noexcept{
        return find(key) != this->data.cend();
    }
    [[nodiscard]]
    constexpr const typename decltype(data)::const_iterator cbegin() const noexcept{
        return data.cbegin();
    }
    [[nodiscard]]
    constexpr const typename decltype(data)::const_iterator cend() const noexcept{
        return data.cend();
    }
};

#endif