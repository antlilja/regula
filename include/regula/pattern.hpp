#pragma once
#include "regula/modifiers.hpp"

#include <vector>

namespace pm {
    template <typename Predicate, typename... Pattern>
    class pattern {
        static constexpr std::size_t pattern_size = sizeof...(Pattern);

        template <std::size_t N>
        using get_Nth_type = get_Nth_type_of_types<N, Pattern...>;

        template <std::size_t n, typename It>
        inline constexpr bool match(It& it, It end) const {
            using type_n = get_Nth_type<n>;
            constexpr bool is_any_of = is_template<type_n, any_of<int, 2>>;
            constexpr bool is_any = std::is_same<type_n, any>::value;

            // The current iterator of the container we are searching
            const auto current = it++;

            // The current part of the pattern
            const auto& current_p = std::get<n>(m_pattern);

            // If we are at end of container pattern can't be found
            if (it == end) return false;

            // If any_of, search through all options
            if constexpr (is_any_of) {
                const auto pred = [&](const auto& x) {
                    return m_pred(x, *current);
                };

                if (std::find_if(current_p.begin(), current_p.end(), pred) ==
                    current_p.end()) {
                    return false;
                }
            }
            // If not any, check if current equal pattern
            else if constexpr (!is_any) {
                if (m_pred(current_p, *current)) return false;
            }

            // This looks weird but clang (and maybe others) crash
            // if it is not written in this way \/

            // If there is more of pattern search next part
            if constexpr ((n + 2) != pattern_size) {
                return match<n + 1>(it, end);
            }
            else {
                ++it;
                return true;
            }
        }

    public:
        constexpr pattern(Predicate pred, Pattern... pattern)
            : m_pred(pred), m_pattern({pattern...}) {}

        // Returns iterators to all matches
        template <typename It>
        std::vector<It> get_matches(It begin, It end) const {
            std::vector<It> matches;
            for (auto it = begin; it < end; ++it) {
                const auto match_it = it;
                if (match<0>(it, end)) matches.emplace_back(match_it);
            }
            return matches;
        }

        // Returns true if range contains pattern
        template <typename It>
        constexpr bool contains_match(It begin, It end) const {
            for (auto it = begin; it != end; ++it) {
                if (match<0>(it, end)) return true;
            }
            return false;
        }

    private:
        Predicate m_pred;
        std::tuple<Pattern...> m_pattern;
    };
} // namespace pm