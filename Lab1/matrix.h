#pragma once

#include <vector>
#include <cstddef>
#include <type_traits>

namespace matmul {

    /**
     * @brief Шаблонный класс квадратной матрицы N x N.
     * @tparam T Тип элементов
     */
    template <typename T = int>
    class Matrix {
        static_assert(std::is_arithmetic_v<T>, "Matrix elements must be numeric!");

    public:
        Matrix() = default;

        explicit Matrix(const std::size_t n, T init_val = T{0})
            : size_(n), data_(n * n, init_val) {}

        ~Matrix() = default;
        Matrix(const Matrix&) = default;
        Matrix(Matrix&&) noexcept = default;
        Matrix& operator=(const Matrix&) = default;
        Matrix& operator=(Matrix&&) noexcept = default;


        T & operator()(const std::size_t row, const std::size_t col) noexcept {
            return data_[row * size_ + col];
        }

        const T& operator()(const std::size_t row, const std::size_t col) const noexcept {
            return data_[row * size_ + col];
        }

        T * operator[](const std::size_t row) noexcept {
            return data_.data() + row * size_;
        }

        const T* operator[](const std::size_t row) const noexcept {
            return data_.data() + row * size_;
        }

        // Доступ к сырым указателям для возможности использования векторных инструкций
        [[nodiscard]] T * data() noexcept {
            return data_.data();
        }

        [[nodiscard]] const T* data() const noexcept {
            return data_.data();
        }

        [[nodiscard]] std::size_t size() const noexcept {
            return size_;
        }

        void fill(T val) {
            std::fill(data_.begin(), data_.end(), val);
        }

        void resize(const std::size_t n, T init_val = T{0}) {
            size_ = n;
            data_.assign(n * n, init_val);
        }

    private:
        std::size_t size_{0};
        std::vector<T> data_;
    };

} // namespace matmul