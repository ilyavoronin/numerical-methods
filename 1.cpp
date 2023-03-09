#include "linalg.h"
#include <iostream>

int main() {
    using T = float;
    Array<T> a{1,2,3}, b{4,5,6}, c{7,8,9};
    Array<T> d(a+b+c);
    std::cout << (a+b+c) << std::endl;
    std::cout << evaluate(a+b+c) << std::endl;
    std::cout << d << "\n" << std::endl;

    Array<T> e{1, 2, 3, 4, 5}, f{5, 4, 3, 2, 1};
    std::cout << (e < f) << "\n" << std::endl;

    std::cout << all(e < f) << std::endl;
    std::cout << evaluate(all(e < f)) << std::endl;
    std::cout << all(a < b) << std::endl;
    std::cout << evaluate(all(a < b)) << "\n" << std::endl;

    std::cout << any(e < f) << std::endl;
    std::cout << evaluate(any(e < f)) << std::endl;
    std::cout << any(b < a) << std::endl;
    std::cout << evaluate(any(b < a)) <<  "\n" << std::endl;


    Array<T> g{1,2,10};
    std::cout << where(g < b, a + b, b + c) << std::endl;
    std::cout << evaluate(where(g < b, a + b, b + c)) << std::endl;
    return 0;
}