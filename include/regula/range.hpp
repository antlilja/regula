#pragma once

namespace regula {
    template <typename It>
    class range {
    public:
        constexpr range(It begin, It end) : m_begin(begin), m_end(end) {}

        constexpr It begin() const noexcept { return m_begin; }
        constexpr It end() const noexcept { return m_end; }
        constexpr std::size_t size() const noexcept { return m_end - m_begin; }

    private:
        It m_begin, m_end;
    };
} // namespace regula