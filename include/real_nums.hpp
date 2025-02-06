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

template <typename T = double, typename ... Args>
inline T max(T first, Args ... others) {
    return std::fmax(first, max(others ...));
}

template <typename T = double>
inline T max(T first) {
    return first;
}

template <typename T = double, typename ... Args>
inline T min(T first, Args ... others) {
    return std::fmin(first, max(others ...));
}

template <typename T = double>
inline T min(T first) {
    return first;
}

} // namespace real_nums