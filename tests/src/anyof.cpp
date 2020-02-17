#include <catch2/catch.hpp>
#include <regula/regula.hpp>

#include "token.hpp"

#include <vector>

TEST_CASE("Pattern: anyof(foo, bar), bar, foobar", "[anyof]") {
    constexpr auto pattern =
        regula::pattern(regula::any_of(token_t::foo, token_t::bar),
                        token_t::bar, token_t::foobar);

    SECTION("One correct: first foo") {
        const std::vector<token_t> tokens = {token_t::foo, token_t::bar,
                                             token_t::foobar};

        const auto matches = pattern.get_matches(tokens.begin(), tokens.end());
        REQUIRE(matches.size() == 1);
    }

    SECTION("One correct: first bar") {
        const std::vector<token_t> tokens = {token_t::bar, token_t::bar,
                                             token_t::foobar};

        const auto matches = pattern.get_matches(tokens.begin(), tokens.end());
        REQUIRE(matches.size() == 1);
    }

    SECTION("One correct within") {
        const std::vector<token_t> tokens = {token_t::foo, token_t::foo,
                                             token_t::bar, token_t::foobar,
                                             token_t::foobar};

        const auto matches = pattern.get_matches(tokens.begin(), tokens.end());
        REQUIRE(matches.size() == 1);
    }

    SECTION("No correct") {
        const std::vector<token_t> tokens = {token_t::foo, token_t::barfoo,
                                             token_t::bar, token_t::foobar,
                                             token_t::foobar};

        const auto matches = pattern.get_matches(tokens.begin(), tokens.end());
        REQUIRE(matches.size() == 0);
    }
};