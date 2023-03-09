#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

class Statistics {
public:
    void update(float x) {
        if (x < _min || _elems.empty()) {
            _min = x;
        }
        if (x > _max || _elems.empty()) {
            _max = x;
        }

        float y = x - _kc;
        float t = _sum + y;
        _kc = (t - sum()) - y;
        _sum = t;

        _elems.push_back(x);

        float new_m = _m + (x - _m) / _elems.size();
        _M = _M + (x - _m) * (x - new_m);
        _m = new_m;
        _d = _M / _elems.size();
    }
    int count() const noexcept {
        return _elems.size();
    }
    float min() const noexcept {
        return _min;
    }
    float max() const noexcept {
        return _max;
    }
    float sum() const noexcept {
        return _sum;
    }
    float mean() const noexcept {
        return _m;
    }
    float variance() const noexcept {
        return _d;
    }
private:
    std::vector<float> _elems;
    float _min = 0;
    float _max = 0;
    float _sum = 0;
    float _kc = 0;
    float _m = 0;
    float _M = 0;
    float _d = 0;
};

int main() {
    auto stats = Statistics();
    int n = 1024;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            stats.update(50 + 1.0f / 3);
        } else {
            stats.update(-50 - 1.0f / 3);
        }
    }

    std::cout << std::setprecision(10)
             << "cnt: " << stats.count() << "\n"
             << "min: " << stats.min() << "\n"
             << "max: " << stats.max() << "\n"
             << "sum: " << stats.sum() << "\n"
             << "mean: " << stats.mean() << "\n"
             << "var: " << stats.variance() << "\n" << std::endl;

    return 0;
}