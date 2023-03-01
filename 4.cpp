#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>


float pairwise_sum_simd(float* x, float *y, int n) {
    if (n == 1) {
        return x[0];
    } else if (n == 2) {
        return x[0] + x[1];
    } else if (n == 3) {
        return x[0] + x[1] + x[2];
    }

    #pragma omp simd
    for (int i = 0; i < n - 3; i += 4) {
        y[i / 2] = x[i] + x[i + 1];
        y[i / 2 + 1] = x[i + 2] + x[i + 3];
    }

    int i = n - n % 4;
    if (n % 4 == 1) {
        y[i / 2] = x[i];
    } else if (n % 4 == 2) {
        y[i / 2] = x[i] + x[i + 1];
    } else if (n % 4 == 3) {
        y[i / 2] = x[i] + x[i + 1];
        y[i / 2 + 1] = x[i + 2];
    }

    return pairwise_sum_simd(y, x, (n - 1) / 2 + 1);
}

float simple_sum(const float* x, int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += x[i];
    }
    return res;
}

int main() {
    int n = 1024;
    std::vector<float> x(n, 0);
    std::vector<float> y(n, 0);
    for (int i = 0; i < n; i++) {
        x[i] = 50 + 1.0f / 3;
    }

    std::cout << std::setprecision(10) << simple_sum(x.data(), n) << " " << pairwise_sum_simd(x.data(), y.data(), n) << std::endl;
    // simple: 51541.01953 pairwise: 51541.33203 correct: 51541 + 1/3

    n = 16 * 1024 * 1024;
    x.resize(n);
    y.resize(n);

    for (int i = 0; i < n; i++) {
        x[i] = 50 + 1.0f / 7;
    }
    auto start = std::chrono::steady_clock::now();
    float simple_res = simple_sum(x.data(), n);
    auto simple_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());

    start = std::chrono::steady_clock::now();
    float pairwise_res = pairwise_sum_simd(x.data(), y.data(), n);
    auto pairwise_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
    std::cout << simple_res << " " << pairwise_res << std::endl;
    std::cout << simple_time << " " << pairwise_time << std::endl;
    // simple time: ~17400 microseconds, pairwise time: ~12400 microseconds

    return 0;
}