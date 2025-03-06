#ifndef B150DCD8_00D0_43AE_9B07_41997CA65EC0
#define B150DCD8_00D0_43AE_9B07_41997CA65EC0

#include <array>

template <typename T>
class Span
{
public:
    // Constructors
    template <std::size_t N>
    constexpr Span(std::array<T, N> &arr) noexcept
        : data_(arr.data()), size_(N) {}

    template <std::size_t N>
    constexpr Span(const std::array<T, N> &arr) noexcept
        : data_(arr.data()), size_(N) {}

    // Getter functions
    constexpr T *data() noexcept { return data_; }
    constexpr const T *data() const noexcept { return data_; }
    constexpr std::size_t size() const noexcept { return size_; }

    // Indexing
    constexpr T &operator[](std::size_t index) { return data_[index]; }
    constexpr const T &operator[](std::size_t index) const { return data_[index]; }

    // Iterators
    constexpr T *begin() noexcept { return data_; }
    constexpr const T *begin() const noexcept { return data_; }
    constexpr T *end() noexcept { return data_ + size_; }
    constexpr const T *end() const noexcept { return data_ + size_; }

private:
    const T *data_;
    std::size_t size_;
};

#endif /* B150DCD8_00D0_43AE_9B07_41997CA65EC0 */
