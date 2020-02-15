#pragma once
#include "regula/type_traits.hpp"

#include <array>

namespace regula {
    // any_of
    template <typename T, std::size_t N>
    class any_of {
    public:
        template <typename... Options>
        constexpr any_of(Options... options) : m_options({options...}) {}

        constexpr auto begin() const { return m_options.begin(); }
        constexpr auto end() const { return m_options.end(); }

        constexpr auto size() const { return N; }

    private:
        std::array<T, N> m_options;
    };

    // Deduction guide for any_of
    template <typename... Options>
    any_of(Options... option)
        ->any_of<typename std::tuple_element<0, std::tuple<Options...>>::type,
                 sizeof...(Options)>;

    // any
    class any {};

    // optional_rep
    template <typename Pattern>
    class optional_rep {
    public:
        constexpr optional_rep(Pattern pattern) : m_pattern(pattern) {}

        template <std::size_t N, typename It, typename Predicate>
        constexpr It match(It begin, It end, Predicate pred) const {
            return m_pattern.template match<N>(begin, end, pred);
        }

    private:
        Pattern m_pattern;
    };
} // namespace regula