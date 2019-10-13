#pragma once
#include <tuple>
#include <cstdint>

namespace pm {
    template <std::size_t N, typename... Types>
    using get_Nth_type_of_types =
        typename std::tuple_element<N, std::tuple<Types...>>::type;

    template <typename, typename>
    constexpr bool is_template{false};

    template <template <typename...> typename T, typename... A, typename... B>
    constexpr bool is_template<T<A...>, T<B...>>{true};

    // Specialiazation for arrays and like
    template <template <typename, std::size_t> typename T, typename TA,
              std::size_t NA, typename TB, std::size_t NB>
    constexpr bool is_template<T<TA, NA>, T<TB, NB>>{true};
} // namespace pm