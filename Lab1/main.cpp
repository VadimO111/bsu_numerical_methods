#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>

#include "matrix.h"
#include "blocks_matmul.h"

template <typename Func>
double measure_ms(Func&& func) {
    const auto start = std::chrono::high_resolution_clock::now();
    func();
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}

int main() {
    using DataType = int;
    constexpr int NUM_RUNS = 3;

    const std::vector<std::size_t> matrix_sizes = {
        256, 300, 400, 500, 512, 600, 700, 800, 900, 1000, 1024, 1100, 1200, 1300, 1400,
        1500, 1600, 1700, 1800, 1900, 2000, 2048
    };

    const std::vector<std::size_t> block_sizes = {
        1, 2, 5, 16, 32, 54, 55, 56, 64, 128, 256, 504
    };

    std::cout << "=== Запуск бенчмарка (усреднение по " << NUM_RUNS << " запускам) ===" << std::endl;

    // --- Прогрев процессора (Warm-up) ---
    std::cout << "[1/3] Прогрев процессора..." << std::endl;
    {
        matmul::Matrix<DataType> wA(256), wB(256), wC(256);
        matmul::RandomInit(wA);
        matmul::RandomInit(wB);
        matmul::MatMul(wA, wB, wC);
    }
    std::cout << "Прогрев завершен.\n" << std::endl;

    std::ofstream py_file("benchmark_results.py");
    py_file << "# Автоматически сгенерированные результаты замера времени (мс)\n";
    py_file << "results = {\n";

    // --- ОСНОВНЫЕ ЗАМЕРЫ ---
    for (std::size_t N : matrix_sizes) {
        std::cout << ">>> Тестирование N = " << N << "..." << std::endl;

        matmul::Matrix<DataType> A(N), B(N), C(N);
        matmul::RandomInit(A);
        matmul::RandomInit(B);

        py_file << "    " << N << ": {\n";

        // 1. Замер точечного алгоритма (усреднение)
        std::cout << "  - Точечный... " << std::flush;
        double sum_pointwise = 0.0;
        for (int run = 0; run < NUM_RUNS; ++run) {
            sum_pointwise += measure_ms([&]() {
                matmul::MatMul(A, B, C);
            });
        }
        double t_pointwise = sum_pointwise / NUM_RUNS;
        std::cout << std::fixed << std::setprecision(2) << t_pointwise << " ms" << std::endl;
        py_file << "        'pointwise': " << t_pointwise << ",\n";

        // 2. Замер блочного алгоритма с разными r (усреднение)
        py_file << "        'blocks': {\n";
        for (std::size_t r : block_sizes) {
            if (r > N) continue;

            std::cout << "  - Блок r = " << std::setw(3) << r << "... " << std::flush;
            double sum_block = 0.0;
            for (int run = 0; run < NUM_RUNS; ++run) {
                sum_block += measure_ms([&]() {
                    matmul::BlockMatMul(A, B, C, r);
                });
            }
            double t_block = sum_block / NUM_RUNS;
            std::cout << std::fixed << std::setprecision(2) << t_block << " ms" << std::endl;

            py_file << "            " << r << ": " << t_block << ",\n";
        }
        py_file << "        }\n";
        py_file << "    },\n";
        std::cout << std::endl;
    }

    py_file << "}\n";
    py_file.close();

    std::cout << "=== Замеры успешно сохранены в benchmark_results.py ===" << std::endl;
    return 0;
}