#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>

float pairwise_sum_simd_arr(float* x, float *y, int n) {
    if (n <= 8) {
        float sum = 0;
        for (int i = 0; i < n; i++) {
            sum += x[i];
        }
        return sum;
    }

    #pragma omp simd
    for (int i = 0; i < n; i += 8) {
        y[i / 2 + 0] = x[i + 0] + x[i + 4];
        y[i / 2 + 1] = x[i + 1] + x[i + 5];
        y[i / 2 + 2] = x[i + 2] + x[i + 6];
        y[i / 2 + 3] = x[i + 3] + x[i + 7];
    }

    int new_size = (n - n % 8) / 2;
    if (n % 8 < 4) {
        new_size += n % 4;
    } else {
        new_size += 4;
    }
    while (new_size % 8 != 0) {
        y[new_size] = 0;
        new_size++;
    }


    return pairwise_sum_simd_arr(y, x, new_size);
}

void prepare(std::vector<float>& x) {
    if (x.size() % 8 != 0) {
        x.resize((x.size() / 8 + 1) * 8);
    }
}

float pairwise_sum_simd(std::vector<float> &x, std::vector<float> &y, int n) {
    prepare(x);
    prepare(y);
    return pairwise_sum_simd_arr(x.data(), y.data(), n);
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

    std::cout << std::setprecision(10) << simple_sum(x.data(), n) << " " << pairwise_sum_simd(x, y, n) << std::endl;
    // simple: 51541.01953 pairwise: 51541.33594 correct: 51541 + 1/3

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
    float pairwise_res = pairwise_sum_simd(x, y, n);
    auto pairwise_time = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count());
    std::cout << simple_res << " " << pairwise_res << std::endl;
    std::cout << simple_time << " " << pairwise_time << std::endl;
    // simple time: ~17400 microseconds, pairwise time: ~12400 microseconds

    return 0;
}