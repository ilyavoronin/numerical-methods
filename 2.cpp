#include <iostream>
#include <cmath>

float polynomial(float x, const float* a, int n) {
    float res = a[n];
    for (int i = n - 1; i > -1; i--) {
        res = std::fma(res, x, a[i]);
    }
    return res;
}

int main() {
    float a[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float res = polynomial(2.0f, a, 3);
    std::cout << res << std::endl;
    return 0;
}

/** error for  a_0 + a_1 * x + a_2 * x*x + a_3 * x*x*x
((((a3 !* x) !+ a2) !* x !+ a1) !* x !+ a0 = (((a3 * x * (1 + q1) + a2) * (1 + q2) * x * (1 + q3)) + a1) * (1 + q4) * x * (1 + q_5) + a_0) * (1 + q_6)
**/
