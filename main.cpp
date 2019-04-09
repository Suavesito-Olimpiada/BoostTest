#include <chrono>
#include <iostream>

#include "./matrix_exp.hpp"
#include "./matrix_sub.hpp"

namespace clock_nano {
    using time_point = std::chrono::high_resolution_clock::time_point;
    
    auto now () {
        return std::chrono::high_resolution_clock::now ();
    }
    
    using duration = std::chrono::duration <double, std::nano>;
    
    double time_span (std::chrono::duration <double> diff_time) {
        auto t_span = std::chrono::duration_cast <std::chrono::duration <double>> (diff_time);
        return double(t_span.count())/1.0e9;
    }
}

int main()
{
    clock_nano::time_point t_init, t_end;
    double time_exp = 0.0;
    double time_sub = 0.0;

    /* Testing Matrix with expression templates */
    {
        t_init = clock_nano::now();
        matrixlib::Matrix<int> a(2, 2);
        matrixlib::Matrix<int> b(2, 2);
        matrixlib::Matrix<int> d(2, 2);
        a(0, 0) = 2;
        a(1, 1) = 3;
        b(0, 0) = 2;
        b(1, 0) = 4;
        b(0, 1) = 5;
        d(0, 0) = 2;
        d(1, 0) = 4;
        d(0, 1) = 5;
        matrixlib::Matrix<int> c = a + b;
        matrixlib::Matrix<int> w = c * d;
        matrixlib::Matrix<int> e = a - b + c;
        matrixlib::Matrix<int> x = a - b + c * e;
        matrixlib::Matrix<int> y(a, 0);
        y += a;
        matrixlib::Matrix<int> z(a, 0);
        z -= a;
        t_end = clock_nano::now();
        time_exp = clock_nano::time_span(t_init-t_end);

        std::cout << a << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << b << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << c << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << d << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << w << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << e << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << x << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << y << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << z << std::endl;
    }

    /* Testing Matrix without expression templates */
    {
        t_init = clock_nano::now();
        matrixlibsub::Matrix<int> a(2, 2);
        matrixlibsub::Matrix<int> b(2, 2);
        matrixlibsub::Matrix<int> d(2, 2);
        a(0, 0) = 2;
        a(1, 1) = 3;
        b(0, 0) = 2;
        b(1, 0) = 4;
        b(0, 1) = 5;
        d(0, 0) = 2;
        d(1, 0) = 4;
        d(0, 1) = 5;
        matrixlibsub::Matrix<int> c = a + b;
        matrixlibsub::Matrix<int> w = c * d;
        matrixlibsub::Matrix<int> e = a - b + c;
        matrixlibsub::Matrix<int> x = a - b + c * e;
        matrixlibsub::Matrix<int> y(a, 0);
        y += a;
        matrixlibsub::Matrix<int> z(a, 0);
        z -= a;
        t_end = clock_nano::now();
        time_sub = clock_nano::time_span(t_init-t_end);

        std::cout << a << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << b << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << c << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << d << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << w << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << e << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << x << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << y << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << z << std::endl;
    }

    std::cout << "Time with expressions: " << time_exp << std::endl;
    std::cout << "Time without expressions: " << time_sub << std::endl;

    return 0;
}
