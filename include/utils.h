#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <vector>
#include <variant>
#include <string>

using Tuple = std::vector<std::variant<std::string, int, float>>;
template <typename T0, typename... Ts>
std::ostream &operator<<(std::ostream &s, std::variant<T0, Ts...> const &v)
{
    std::visit([&](auto &&arg) { s << arg; }, v);
    return s;
};
void printTuple(const Tuple &tuple);

#endif