#include "blocks_matmul.h"

#include <cassert>


template<typename T>
void matmul::MatMul(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C) {
    C.fill(0);
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t k = 0; k < A.size(); ++k) {
            for (size_t j = 0; j < A.size(); ++j) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

template<typename T>
void matmul::BlockMatMul(const Matrix<T> &A, const Matrix<T> &B, Matrix<T> &C, const std::size_t r) {
    assert(r != 0, "Block size must be positive");
    C.fill(0);
    for (size_t i_block = 0; i_block * r < A.size(); ++i_block) {
        for (size_t k_block = 0; k_block * r < A.size(); ++k_block) {
            for (size_t j_block = 0; j_block * r < A.size(); ++j_block) {
                for (size_t i = i_block * r; i < std::min((i_block + 1) * r, A.size()); ++i) {
                    for (size_t k = k_block * r; k < std::min((k_block + 1) * r, A.size()); ++k) {
                        for (size_t j = j_block * r; j < std::min((j_block + 1) * r, A.size()); ++j) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

template<typename T>
void matmul::RandomInit(Matrix<T> &mat, T min_val, T max_val) {
    static std::mt19937 rnd(67);
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(min_val, max_val);
        for (size_t i = 0; i < mat.size(); ++i) {
            for (size_t j = 0; j < mat.size(); ++j) {
                mat[i][j] = dist(rnd);
            }
        }
    } else {
        std::uniform_real_distribution<T> dist(min_val, max_val);
        for (size_t i = 0; i < mat.size(); ++i) {
            for (size_t j = 0; j < mat.size(); ++j) {
                mat[i][j] = dist(rnd);
            }
        }
    }
}
