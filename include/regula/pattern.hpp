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

        static constexpr auto default_pred = [](const auto a, const auto b) {
            return a == b;
        };

    public:
        template <std::size_t N, typename It, typename Predicate>
        constexpr It match(It it, It end, Predicate pred) const {
            using type_n = get_Nth_type<N>;
            constexpr bool is_any_of = is_template<type_n, any_of<int, 2>>;
            constexpr bool is_any = std::is_same<type_n, any>::value;
            constexpr bool is_pattern = is_template<type_n, pattern<int, int>>;
            constexpr bool is_op_rep = is_template<type_n, optional_rep<int>>;

            constexpr auto N_DEBUG = N;
            constexpr auto pattern_size_DEBUG = pattern_size;
            const auto distance_from_end_DEBUG = std::distance(it, end);

            // The current iterator of the container we are searching
            const auto current = it;

            // The current part of the pattern
            const auto& current_p = std::get<N>(m_pattern);

            // If we are at end of container pattern can't be found
            if (it == end) return end;

            // If any_of, search through all options
            if constexpr (is_any_of) {
                const auto if_pred = [&](const auto& x) {
                    return pred(x, *current);
                };

                if (std::find_if(current_p.begin(), current_p.end(), if_pred) ==
                    current_p.end()) {
                    return end;
                }
            }
            // Nested patterns are used for repetition
            else if constexpr (is_pattern || is_op_rep) {
                // Pattern has to be found at least once in non optional pattern
                if constexpr (is_pattern) {
                    if (it = current_p.template match<0>(it, end, pred);
                        it == end) {
                        return end;
                    }
                }

                auto stored_it = it;
                for (;;) {
                    stored_it = it;
                    if (it = current_p.template match<0>(it, end, pred);
                        it == end) {
                        break;
                    }
                }
                it = stored_it;
            }
            // If not any, check if current equal pattern
            else if constexpr (!is_any) {
                if (!pred(current_p, *current)) return end;
            }

            // If there is more of pattern then search next part
            if constexpr (N < (pattern_size - 1)) {
                return match<N + 1>(it + 1, end, pred);
            }
            else {
                return it;
            }
        }

    public:
        constexpr pattern(Pattern... pattern) : m_pattern({pattern...}) {}

        // Returns span of all matches
        template <typename It, typename Predicate = decltype(default_pred)>
        std::vector<range<It>>
        get_matches(It begin, It end, Predicate pred = default_pred) const {
            std::vector<range<It>> matches;
            for (auto it = begin; it < end; ++it) {
                if (const auto last = match<0>(it, end, pred); last != end) {
                    matches.emplace_back(it, last);
                    it = last;
                }
            }
            return matches;
        }

        // Returns number of matches
        template <typename It, typename Predicate = decltype(default_pred)>
        std::size_t count_matches(It begin, It end,
                                  Predicate pred = default_pred) const {
            std::size_t count;
            for (auto it = begin; it < end; ++it) {
                if (const auto last = match<0>(it, end, pred); last != end) {
                    ++count;
                    it = last;
                }
            }
            return count;
        }

        // Returns true if range contains pattern
        template <typename It, typename Predicate = decltype(default_pred)>
        constexpr bool contains_match(It begin, It end,
                                      Predicate pred = default_pred) const {
            for (auto it = begin; it != end; ++it) {
                if (match<0>(it, end, pred) != end) return true;
            }
            return false;
        }

    private:
        std::tuple<Pattern...> m_pattern;
    };
} // namespace regula