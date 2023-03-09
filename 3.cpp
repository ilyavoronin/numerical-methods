#include <utility>
#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include <iomanip>

struct SMatrix {
public:
    explicit SMatrix(std::vector<std::vector<long double>> matrix) {
        _n = matrix.size();
        _matrix = std::move(matrix);
    }

    long double det() const {
        std::vector<int> r;
        std::set<int> used;
        for (int i = 0; i < _n; i++) {
            used.insert(i);
        }

        return perm(r, used);
    }
private:
    int _n;
    std::vector<std::vector<long double>> _matrix;

    long double perm(std::vector<int>& r, std::set<int> &used) const {
        if (used.empty()) {
            long double res = 1;
            int sign = 0;
            for (int i = 0; i < _n; i++) {
                res *= _matrix[i][r[i]];
                for (int j = i + 1; j < _n; j++) {
                    if (r[i] > r[j]) {
                        sign++;
                    }
                }

            }

            if (sign % 2 == 1) {
                res = -res;
            }

            return res;
        }
        long double res = 0;
        for (auto index : std::vector<int>(used.begin(), used.end())) {
            used.erase(index);
            r.push_back(index);
            res += perm(r, used);
            r.pop_back();
            used.insert(index);
        }

        return res;
    }
};

int main() {
    auto m1 = SMatrix({
                             {1, 2, 3},
                             {4, -5, 6},
                             {-7, 8, -9}
    });
    auto m2 = SMatrix({
                              {-10, 23, 74, 48, 123},
                              {43, 23, 93, 128, 84},
                              {75, 381, -81, 971, 49},
                              {432, -38, 83, -128, 913},
                              {412, -17, -93, 97, 14},
    });

    std::cout << std::setprecision(20) << m1.det() << " " <<  m2.det() << std::endl;
    //                                    -24                 294911553369
    return 0;
}