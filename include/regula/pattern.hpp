#pragma once
#include "regula/modifiers.hpp"
#include "regula/range.hpp"

#include <vector>

namespace regula {
    template <typename... Pattern>
    class pattern {
        static constexpr std::size_t pattern_size = sizeof...(Pattern);

        template <std::size_t N>
        using get_Nth_type = get_Nth_type_of_types<N, Pattern...>;

    public:
        template <std::size_t N, typename It, typename Predicate>
        constexpr bool match(It& it, It end, Predicate pred) const {
            using type_n = get_Nth_type<N>;
            constexpr bool is_any_of = is_template<type_n, any_of<int, 2>>;
            constexpr bool is_any = std::is_same<type_n, any>::value;
            constexpr bool is_pattern = is_template<type_n, pattern<int, int>>;
            constexpr bool is_op_rep = is_template<type_n, optional_rep<int>>;

            constexpr auto N_DEBUG = N;
            constexpr auto pattern_size_DEBUG = pattern_size;

            // The current iterator of the container we are searching
            const auto current = it;

            // The current part of the pattern
            const auto& current_p = std::get<N>(m_pattern);

            // If we are at end of container pattern can't be found
            if (it == end) return false;

            // If any_of, search through all options
            if constexpr (is_any_of) {
                const auto if_pred = [&](const auto& x) {
                    return pred(x, *current);
                };

                ++it;

                if (std::find_if(current_p.begin(), current_p.end(), if_pred) ==
                    current_p.end()) {
                    return false;
                }
            }
            // Nested patterns are used for repetition
            else if constexpr (is_pattern || is_op_rep) {
                // Pattern has to be found at least once in non optional pattern
                if constexpr (is_pattern) {
                    if (!current_p.template match<0>(it, end, pred))
                        return false;
                }

                auto stored_it = it;
                for (;;) {
                    stored_it = it;
                    if (!current_p.template match<0>(it, end, pred)) break;
                }
                it = stored_it;
            }
            // If not any, check if current equal pattern
            else if constexpr (!is_any) {
                if (!pred(current_p, *(it++))) return false;
            }

            // This looks weird but clang (and maybe others) crash
            // if it is not written in this way \/

            // If there is more of pattern search next part
            if constexpr (N < (pattern_size - 1)) {
                return match<N + 1>(it, end, pred);
            }
            else {
                return true;
            }
        }

    public:
        constexpr pattern(Pattern... pattern) : m_pattern({pattern...}) {}

        // Returns span of all matches
        template <typename It, typename Predicate>
        std::vector<range<It>> get_matches(It begin, It end,
                                           Predicate pred) const {
            std::vector<range<It>> matches;
            for (auto it = begin; it < end;) {
                auto match_it = it;
                if (match<0>(it, end, pred)) {
                    matches.emplace_back(match_it, it);
                }
            }
            return matches;
        }

        // Returns true if range contains pattern
        template <typename It, typename Predicate>
        constexpr bool contains_match(It begin, It end, Predicate pred) const {
            for (auto it = begin; it != end;) {
                if (match<0>(it, end, pred)) return true;
            }
            return false;
        }

    private:
        std::tuple<Pattern...> m_pattern;
    };
} // namespace regula