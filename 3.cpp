#include <iostream>
#include <vector>
#include <iomanip>

float kahan_sum(const float* x, int n) {
    float s = 0;
    float c = 0;
    for (int i = 0; i < n; i++) {
        float y = x[i] - c;
        float t = s + y;
        c = (t - s) - y;
        s = t;
    }
    return s;
}

float simple_sum(const float* x, int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += x[i];
    }
    return res;
}

int main() {
    int n = 1000;
    std::vector<float> x(n);
    for (int i = 0; i < n; i++) {
        x[i] = 50 + 1.0f / 3;
    }
    std::cout << std::setprecision(10) << kahan_sum(x.data(), n) << " " << simple_sum(x.data(), n) << std::endl;
    // kahan: 50333.33203 simple: 50333.05078 correct: 50333 + 1/3

    for (int i = 0; i < n; i++) {
        x[i] = 50 + 1.0f / 7;
    }
    std::cout << std::setprecision(10) << kahan_sum(x.data(), n) << " " << simple_sum(x.data(), n) << std::endl;
    // kahan: 50142.85547 simple: 50143.31641 correct: 50142 + 6/7
    return 0;
}
