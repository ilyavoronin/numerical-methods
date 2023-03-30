#include <iostream>
#include <vector>
#include <fstream>
#include <cfloat>
#include <cmath>


struct Point {
public:
    double x;
    double y;
    double z;

    Point(double x, double y, double z): x(x), y(y), z(z) {}

    Point operator-(Point &other) const {
        return {x - other.x, y - other.y, 0};
    }

    //cross product
    double operator%(const Point &other) const {
        return x * other.y - y * other.x;
    }
};

bool sign(double a) {
    if (a < 0) {
        return false;
    } else {
        return true;
    }
}

bool is_inside(std::vector<Point> &f, Point &p) {
    bool s = false;
    for (int i = 0; i < f.size(); i++) {
        int j = (i + 1) % f.size();
        bool cs = sign((f[j] - f[i]) % (p - f[i]));
        if (i != 0 && cs != s) {
            return false;
        }
        s = cs;
    }
    return true;
}

std::vector<Point> find_rectangle(std::vector<Point> grid, int n, Point p) {
    for (int j = 0; j < n - 1; j++) {
        for (int i = 0; i < n - 1; i++) {
            std::vector<Point> t1{grid[i * n + j], grid[i * n + j + 1], grid[(i + 1) * n + j + 1], grid[(i + 1) * n + j]};
            if (is_inside(t1, p)) {
                return t1;
            }
        }
    }
    return {};
}

double dist(Point &a, Point &b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double butterworth(Point &a, Point &b, double sigma, int p) {
    return 1 / (1 + std::pow(dist(a, b) / sigma, 2 * p));
}

double gauss(Point &a, Point &b, double sigma) {
    return std::exp(-std::pow((sigma * dist(a, b)), 2));
}

Point interpolate(std::vector<Point> &t, Point &point) {
    double f[5][5];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            f[i + 1][j + 1] = dist(t[i], t[j]);
        }
    }
    double g1 = t[0].z;
    double g2 = t[1].z;
    double g3 = t[2].z;
    double g4 = t[3].z;

    double d = 1-f[1][2]*f[2][1]-f[1][3]*f[3][1]+f[1][2]*f[2][3]*f[3][1]+f[1][3]*f[2][1]*f[3][2]-f[2][3]*f[3][2]-f[1][4]*f[4][1]+f[1][2]*f[2][4]*f[4][1]+f[1][4]*f[2][3]*f[3][2]*f[4][1]-f[1][3]*f[2][4]*f[3][2]*f[4][1]+f[1][3]*f[3][4]*f[4][1]-f[1][2]*f[2][3]*f[3][4]*f[4][1]+f[1][4]*f[2][1]*f[4][2]-f[2][4]*f[4][2]-f[1][4]*f[2][3]*f[3][1]*f[4][2]+f[1][3]*f[2][4]*f[3][1]*f[4][2]-f[1][3]*f[2][1]*f[3][4]*f[4][2]+f[2][3]*f[3][4]*f[4][2]+f[1][4]*f[3][1]*f[4][3]-f[1][2]*f[2][4]*f[3][1]*f[4][3]-f[1][4]*f[2][1]*f[3][2]*f[4][3]+f[2][4]*f[3][2]*f[4][3]-f[3][4]*f[4][3]+f[1][2]*f[2][1]*f[3][4]*f[4][3];
    double w1 = (g1-g2*f[2][1]-g3*f[3][1]+g2*f[2][3]*f[3][1]+g3*f[2][1]*f[3][2]-g1*f[2][3]*f[3][2]-g4*f[4][1]+g2*f[2][4]*f[4][1]+g4*f[2][3]*f[3][2]*f[4][1]-g3*f[2][4]*f[3][2]*f[4][1]+g3*f[3][4]*f[4][1]-g2*f[2][3]*f[3][4]*f[4][1]+g4*f[2][1]*f[4][2]-g1*f[2][4]*f[4][2]-g4*f[2][3]*f[3][1]*f[4][2]+g3*f[2][4]*f[3][1]*f[4][2]-g3*f[2][1]*f[3][4]*f[4][2]+g1*f[2][3]*f[3][4]*f[4][2]+g4*f[3][1]*f[4][3]-g2*f[2][4]*f[3][1]*f[4][3]-g4*f[2][1]*f[3][2]*f[4][3]+g1*f[2][4]*f[3][2]*f[4][3]-g1*f[3][4]*f[4][3]+g2*f[2][1]*f[3][4]*f[4][3]) / d;
    double w2 = (g2-g1*f[1][2]+g3*f[1][2]*f[3][1]-g2*f[1][3]*f[3][1]-g3*f[3][2]+g1*f[1][3]*f[3][2]+g4*f[1][2]*f[4][1]-g2*f[1][4]*f[4][1]-g4*f[1][3]*f[3][2]*f[4][1]+g3*f[1][4]*f[3][2]*f[4][1]-g3*f[1][2]*f[3][4]*f[4][1]+g2*f[1][3]*f[3][4]*f[4][1]-g4*f[4][2]+g1*f[1][4]*f[4][2]+g4*f[1][3]*f[3][1]*f[4][2]-g3*f[1][4]*f[3][1]*f[4][2]+g3*f[3][4]*f[4][2]-g1*f[1][3]*f[3][4]*f[4][2]-g4*f[1][2]*f[3][1]*f[4][3]+g2*f[1][4]*f[3][1]*f[4][3]+g4*f[3][2]*f[4][3]-g1*f[1][4]*f[3][2]*f[4][3]-g2*f[3][4]*f[4][3]+g1*f[1][2]*f[3][4]*f[4][3]) / d;
    double w3 = (g3-g1*f[1][3]-g3*f[1][2]*f[2][1]+g2*f[1][3]*f[2][1]-g2*f[2][3]+g1*f[1][2]*f[2][3]+g4*f[1][3]*f[4][1]-g3*f[1][4]*f[4][1]-g4*f[1][2]*f[2][3]*f[4][1]+g2*f[1][4]*f[2][3]*f[4][1]+g3*f[1][2]*f[2][4]*f[4][1]-g2*f[1][3]*f[2][4]*f[4][1]-g4*f[1][3]*f[2][1]*f[4][2]+g3*f[1][4]*f[2][1]*f[4][2]+g4*f[2][3]*f[4][2]-g1*f[1][4]*f[2][3]*f[4][2]-g3*f[2][4]*f[4][2]+g1*f[1][3]*f[2][4]*f[4][2]-g4*f[4][3]+g1*f[1][4]*f[4][3]+g4*f[1][2]*f[2][1]*f[4][3]-g2*f[1][4]*f[2][1]*f[4][3]+g2*f[2][4]*f[4][3]-g1*f[1][2]*f[2][4]*f[4][3]) / d;
    double w4 = g4 - w1 * f[1][4] - w2 * f[2][4] - w3 * f[3][4];

    //double z = w1 * butterworth(point, t[0], 2, 4) + w2 * butterworth(point, t[1], 2, 4) + w3 * butterworth(point, t[2], 2, 4) + w4 * butterworth(point, t[3], 2, 4);
    double z = w1 * gauss(point, t[0], 0.1) + w2 * gauss(point, t[1], 0.1) + w3 * gauss(point, t[2], 0.1) + w4 * gauss(point, t[3], 0.1);

    return {point.x, point.y, z};
}

int main() {
    std::vector<Point> points;
    std::ifstream in("surface.xyz", std::ios::in);
    for (int i = 0; i < 16384; i++) {
        double x, y, z;
        in >> x >> y >> z;
        points.emplace_back(x, y, z);
    }
    in.close();

    std::ifstream in2("points.xyz", std::ios::in);
    std::ofstream out("output2.xyz", std::ios::out);
    int n;
    in2 >> n;
    for (int i = 0; i < n; i++) {
        double x, y;
        in2 >> x >> y;
        auto point = Point(x, y, 0);

        auto rectangle = find_rectangle(points, 128, point);
        auto ipoint = interpolate(rectangle, point);

        out << ipoint.x << " " << ipoint.y << " " << ipoint.z << std::endl;
    }

    in2.close();
    out.close();
}