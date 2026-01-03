#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>
#include <tuple>

template<int Index, typename Char, typename Traits, typename... Types>
struct TuplePrinter {
    static void printer(std::basic_ostream<Char, Traits>& out,
                      const std::tuple<Types...>& tpl) {
        TuplePrinter<Index - 1, Char, Traits, Types...>::printer(out, tpl);
        out << std::get<Index>(tpl) << " ";
    }
};

template<typename Char, typename Traits, typename... Types>
struct TuplePrinter<0, Char, Traits, Types...> {
    static void printer(std::basic_ostream<Char, Traits>& out,
                      const std::tuple<Types...>& tpl) {
        out << std::get<0>(tpl) << " ";
    }
};

template<typename Char, typename Traits, typename... Types>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& out,
           const std::tuple<Types...>& tpl) {
    out << "( ";
    if constexpr (sizeof...(Types) > 0) {
        TuplePrinter<sizeof...(Types) - 1, Char, Traits, Types...>::printer(out, tpl);
    }
    return out << ")";
}

#endif
