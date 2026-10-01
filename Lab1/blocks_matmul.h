#pragma once

#include <random>


#include "matrix.h"


namespace matmul {
    /**
     * @brief Точечное перемножение квадратных матриц: C = A * B.
     *
     * @tparam T Числовой тип данных
     * @param A Входная матрица первого множителя (N x N).
     * @param B Вхотдная матрица второго множителя (N x N).
     * @param C Выходная матрица результата (N x N). Инициализируется нулями внутри функции.
     */
    template <typename T>
    void MatMul(const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C);

    /**
     * @brief Блочное перемножение квадратных матриц: C = A * B.
     *
     * Разбивает матрицы на подматрицы (блоки) размера r x r для эффективного
     * перемжожения
     *
     * @tparam T Числовой тип данных
     * @param A Входная матрица первого множителя (N x N).
     * @param B Вхотдная матрица второго множителя (N x N).
     * @param C Выходная матрица результата (N x N). Инициализируется нулями внутри функции.
     * @param r Размер стороны квадратного блока (подматрицы).
     */
    template <typename T>
    void BlockMatMul(const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C, std::size_t r);

    /**
     * @brief Заполнение матрицы случайными числами в диапазоне [min_val, max_val].
     *
     * Использует генератор псевдослучайных чисел std::mt19937.
     *
     * @tparam T Числовой тип данных.
     * @param mat Матрица для заполнения.
     * @param min_val Нижняя граница генерации (по умолчанию: -100).
     * @param max_val Верхняя граница генерации (по умолчанию: 100).
     */
    template <typename T>
    void RandomInit(Matrix<T>& mat, T min_val = static_cast<T>(-100), T max_val = static_cast<T>(100));
} // namespace matmul