#include <algorithm>
#include <cmath>
#include <limits>

#pragma once

namespace real_nums {

template <typename T = double> class my_epsilon {
public:
    static T epsilon();
};

template <> class my_epsilon<long double> {
public:
    static long double epsilon() { return 1e-13; };
};

template <> class my_epsilon<double> {
public:
    static double epsilon() { return 1e-10; };
};

template <> class my_epsilon<float> {
public:
    static float epsilon() { return 1e-5; };
};

template <typename T = double> inline bool is_zero(T x) {
    T epsilon = my_epsilon<T>::epsilon();
    return std::fabs(x) < epsilon;
}

template <typename T = double> inline bool equal(T x, T y) {
    T epsilon = my_epsilon<T>::epsilon();
    return std::fabs(x - y) < epsilon;
}

template <typename T = double> inline bool is_more_zero(T x) {
    T epsilon = my_epsilon<T>::epsilon();
    return x > epsilon;
}

template <typename T = double> inline bool is_less_zero(T x) {
    T epsilon = my_epsilon<T>::epsilon();
    return x < -epsilon;
}

template <typename T = double> inline bool is_more_or_equal_zero(T x) {
    return is_more_zero<T>(x) || is_zero<T>(x);
}

template <typename T = double> inline bool is_less_or_equal_zero(T x) {
    return is_less_zero<T>(x) || is_zero<T>(x);
}

template <typename T = double> inline T max2(T x, T y) {
    return std::fmax(x, y);
}

template <typename T = double> inline T min2(T x, T y) {
    return std::fmin(x, y);
}

template <typename T = double> inline T max3(T x, T y, T z) {
    return std::fmax(std::fmax(x, y), z);
}

template <typename T = double> inline T min3(T x, T y, T z) {
    return std::fmin(std::fmin(x, y), z);
}

template <typename T = double>
inline double max6(T x1, T y1, T z1, T x2, T y2, T z2) {
    return max2(max3(x1, y1, z1), max3(x2, y2, z2));
}

template <typename T = double>
inline double max9(T x1, T y1, T z1, T x2, T y2, T z2, T x3, T y3, T z3) {
    return max3(max3(x1, y1, z1), max3(x2, y2, z2), max3(x3, y3, z3));
}

} // namespace real_nums