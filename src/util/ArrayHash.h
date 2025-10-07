struct ArrayHash 
{
    template <typename T, std::size_t N>
    std::size_t operator()(const std::array<T, N>& arr) const noexcept {
        std::size_t seed = 0;
        for (const auto& elem : arr) {
            // Combine hashes (standard hash combining trick)
            seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};