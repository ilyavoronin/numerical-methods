#include <iostream>
#include <vector>
#include <fstream>
#include <cfloat>


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

std::vector<Point> find_triangle(std::vector<Point> grid, int n, Point p) {
    for (int j = 0; j < n - 1; j++) {
        for (int i = 0; i < n - 1; i++) {
            std::vector<Point> t1{grid[i * n + j], grid[i * n + j + 1], grid[(i + 1) * n + j]};
            std::vector<Point> t2{grid[i * n + j + 1], grid[(i + 1) * n + j], grid[(i + 1) * n + j + 1]};

            if (is_inside(t1, p)) {
                return t1;
            }
            if (is_inside(t2, p)) {
                return t2;
            }
        }
    }
    return {};
}

Point interpolate(std::vector<Point> &t, Point &point) {
    double d = (t[1].y - t[2].y) * (t[0].x - t[2].x) + (t[2].x - t[1].x) * (t[0].y - t[2].y);
    double t1 = ((t[1].y - t[2].y) * (point.x - t[2].x) + (t[2].x - t[1].x) * (point.y - t[2].y)) / d;
    double t2 = ((t[2].y - t[0].y) * (point.x - t[2].x) + (t[0].x - t[2].x) * (point.y - t[2].y)) / d;
    double t3 = 1 - t1 - t2;

    return {point.x, point.y, t1 * t[0].z + t2 * t[1].z + t3 * t[2].z};
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
    std::ofstream out("output.xyz", std::ios::out);
    int n;
    in2 >> n;
    for (int i = 0; i < n; i++) {
        double x, y;
        in2 >> x >> y;
        auto point = Point(x, y, 0);

        auto triangle = find_triangle(points, 128, point);
        auto ipoint = interpolate(triangle, point);

        out << ipoint.x << " " << ipoint.y << " " << ipoint.z << std::endl;
    }

    in2.close();
    out.close();
}