#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

float length(const float* x, int n) {
    float res = 0;
    float max = x[0];
    for (int i = 0; i < n; i++) {
        float fx = fabsf(x[i]);
        if (fx > max) {
            float nm = max / fx;
            res = res * nm * nm;
            max = fx;
        }
        if (max != 0) {
            float sq = x[i] / max;
            res += sq * sq;
        }
    }
    return max * sqrtf(res);
}

float length2(const float* x, int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += x[i] * x[i];
    }
    return sqrtf(res);
}

int main() {
    int n = 1 << 12;
    std::vector<float> x(n, 0);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            x[i] = float(i) + 1.0f / 71;
        } else {
            x[i] = -float(i) + 1.0f / 71;
        }
    }

    std::cout << std::setprecision(20) << length(x.data(), n) << " " << length2(x.data(), n) << std::endl;

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            x[i] = float(i) * 1e20f;
        } else {
            x[i] = float(i) * -1e20f;
        }
    }

    std::cout << std::setprecision(20) << length(x.data(), n) << " " << length2(x.data(), n) << std::endl;
    return 0;
}