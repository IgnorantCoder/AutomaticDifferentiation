#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

#include "ad/dual.h"
#include "ad/bottom_up.h"
#include "ad/top_down.h"

template <typename T, typename M, typename S>
auto normal_distribution_cdf(
    const T& x,
    const M& mu,
    const S& sigma)
{
    return 0.5 * (1.0 + std::erf((x - mu) / std::sqrt(2 * sigma * sigma)));
}

template <typename T>
auto standard_distribution_cdf(const T& x)
{
    return normal_distribution_cdf(x, 0.0, 1.0);
}

template <typename X, typename T, typename K, typename S, typename R>
auto calc_d1(const X& x, const T& t, const K& k, const S& s, const R& r)
{
    return (std::log(x / k) + (r + s * s * 0.5) * t) / (s * std::sqrt(t));
}

template <typename X, typename T, typename K, typename S, typename R>
auto calc_d2(const X& x, const T& t, const K& k, const S& s, const R& r)
{
    return (std::log(x / k) + (r - s * s * 0.5) * t) / (s * std::sqrt(t));
}

/**
@brief call option price based on black-scholes model.
*/
template <typename X, typename T, typename K, typename S, typename R>
auto call_option(
    const X& x,
    const T& t,
    const K& k,
    const S& s,
    const R& r)
{
    const auto d1 = ad::dual::eval(calc_d1(x, t, k, s, r));
    const auto d2 = ad::dual::eval(calc_d2(x, t, k, s, r));
    const auto d = std::exp(- r * t);

    const auto ret 
        = x * standard_distribution_cdf(d1)
        - k * d * standard_distribution_cdf(d2);

    return ad::dual::eval(ret);
}

template <typename F>
void run_test(
    F&& test_method,
    const std::string& method_name,
    const std::size_t n)
{
    std::cout << "start test of " << method_name << std::endl;
    {
        const auto t_s = std::chrono::system_clock::now();
        test_method(n);
        const auto t_e = std::chrono::system_clock::now();
        const auto time
            = std::chrono::duration_cast<std::chrono::milliseconds>(t_e - t_s).count();
        std::cout << time << "[ms]" << std::endl;
    }
    std::cout << "end test of " << method_name << std::endl;
}

struct test {
private:
    static constexpr double x = 107;
    static constexpr double k = 100;
    static constexpr double t = 0.5;
    static constexpr double s = 0.3;
    static constexpr double r = 0.1;

public:
    static void shock_method(const std::size_t n);
    static void dual_method(const std::size_t n);
    static void bottom_up_method(const std::size_t n);
    static void top_down_method(const std::size_t n);
};

int main()
{
    //test data
    const double x = 107;
    const double k = 100;
    const double t = 0.5;
    const double s = 0.3;
    const double r = 0.1;

    //test count
    const std::size_t n = 100000;

    run_test(test::shock_method, "shock method", n);
    run_test(test::dual_method, "dual method", n);
    run_test(test::bottom_up_method, "bottom up method", n);
    run_test(test::top_down_method, "top down method", n);

    return 0;
}

inline void test::shock_method(const std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i) {
        const double h = 0.0001;
        const double price
            = call_option(x, t, k, s, r);
        const double delta
            = (call_option(x + h, t, k, s, r)
                - call_option(x - h, t, k, s, r)) / (2 * h);
        const double theta
            = (call_option(x, t + h, k, s, r)
                - call_option(x, t - h, k, s, r)) / (2 * h);
        const double vega
            = (call_option(x, t, k, s + h, r)
                - call_option(x, t, k, s - h, r)) / (2 * h);
    }
}

inline void test::dual_method(const std::size_t n)
{
    const auto mgr
        = ad::dual::create_variable_manager(x, k, t, s, r);
    const auto v_x = mgr.get_variable(x);
    const auto v_t = mgr.get_variable(t);
    const auto v_k = mgr.get_variable(k);
    const auto v_s = mgr.get_variable(s);
    const auto v_r = mgr.get_variable(r);

    for (std::size_t i = 0; i < n; ++i) {
        const auto p
            = call_option(v_x, v_t, v_k, v_s, v_r);
        const double price
            = static_cast<double>(p);
        const double delta
            = ad::dual::d(p).d(x);
        const double theta
            = ad::dual::d(p).d(t);
        const double vega
            = ad::dual::d(p).d(s);
    }
}

inline void test::bottom_up_method(const std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i) {
        auto mgr = ad::rt::bu::create_manager<double>();
        const auto v_x = mgr.to_variable(x);
        const auto v_t = mgr.to_variable(t);
        const auto v_k = mgr.to_variable(k);
        const auto v_s = mgr.to_variable(s);
        const auto v_r = mgr.to_variable(r);

        const auto p
            = call_option(v_x, v_t, v_k, v_s, v_r);
        const double price
            = static_cast<double>(p);
        const double delta = ad::rt::bu::d(p).d(x);
        const double theta = ad::rt::bu::d(p).d(t);
        const double vega = ad::rt::bu::d(p).d(s);
    }
}

inline void test::top_down_method(const std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i) {
        auto mgr = ad::rt::td::create_manager<double>();
        const auto v_x = mgr.to_variable(x);
        const auto v_t = mgr.to_variable(t);
        const auto v_k = mgr.to_variable(k);
        const auto v_s = mgr.to_variable(s);
        const auto v_r = mgr.to_variable(r);

        const auto p
            = call_option(v_x, v_t, v_k, v_s, v_r);
        const double price
            = static_cast<double>(p);
        const auto gradient = ad::rt::td::gradient(p);
        const double delta = gradient.in_direction_of(v_x);
        const double theta = gradient.in_direction_of(v_t);
        const double vega = gradient.in_direction_of(v_s);
    }
}
