#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

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
    const auto d1 = ad::bu::eval(calc_d1(x, t, k, s, r));
    const auto d2 = ad::bu::eval(calc_d2(x, t, k, s, r));
    const auto d = std::exp(- r * t);

    const auto ret 
        = x * standard_distribution_cdf(d1)
        - k * d * standard_distribution_cdf(d2);

    return ad::bu::eval(ret);
}

struct test {
    static constexpr double x = 107;
    static constexpr double k = 100;
    static constexpr double t = 0.5;
    static constexpr double s = 0.3;
    static constexpr double r = 0.1;

    static void shock_method(const std::size_t n)
    {
        for (std::size_t i = 0; i < n; ++i) {
            const double h = 0.0001;
            const double price
                = call_option(double(x), double(t), double(k), double(s), double(r));
            const double delta
                = (call_option(double(x) + h, double(t), double(k), double(s), double(r))
                    - call_option(double(x) - h, double(t), double(k), double(s), double(r))) / (2 * h);
            const double theta
                = (call_option(double(x), t + h, double(k), double(s), double(r))
                    - call_option(double(x), double(t) - h, double(k), double(s), double(r))) / (2 * h);
            const double vega
                = (call_option(double(x), double(t), double(k), double(s) + h, double(r))
                    - call_option(double(x), double(t), double(k), double(s) - h, double(r))) / (2 * h);
        }
    }

    static void bottom_up_method(const std::size_t n)
    {
        //const auto mgr
        //    = ad::bu::create_variable_manager(x, k, t, s, r);
        //const auto v_x = mgr.get_variable(x);
        //const auto v_t = mgr.get_variable(t);
        //const auto v_k = mgr.get_variable(k);
        //const auto v_s = mgr.get_variable(s);
        //const auto v_r = mgr.get_variable(r);
        //
        //for (std::size_t i = 0; i < n; ++i) {
        //    const auto p
        //        = call_option(v_x, v_t, v_k, v_s, v_r);
        //    const double price
        //        = static_cast<double>(p);
        //    const double delta
        //        = ad::bu::d(p).d(x);
        //    const double theta
        //        = ad::bu::d(p).d(t);
        //    const double vega
        //        = ad::bu::d(p).d(s);
        //}
    }

    static void top_down_method(const std::size_t n)
    {
        ad::td::detail::calculation_graph<double> tape;
        const auto v_x = ad::td::variable<double, double>(double(x), tape);
        const auto v_t = ad::td::variable<double, double>(double(t), tape);
        const auto v_k = ad::td::variable<double, double>(double(k), tape);
        const auto v_s = ad::td::variable<double, double>(double(s), tape);
        const auto v_r = ad::td::variable<double, double>(double(r), tape);

        for (std::size_t i = 0; i < n; ++i) {
            const auto p
                = call_option(v_x, v_t, v_k, v_s, v_r);
            const double price
                = static_cast<double>(p);
            const double delta
                = p(0);
            const double theta
                = p(1);
            const double vega
                = p(3);
        }
    }
};

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

int main()
{
    //test data
    const double x = 107;
    const double k = 100;
    const double t = 0.5;
    const double s = 0.3;
    const double r = 0.1;

    //test count
    const std::size_t n = 10000;

    run_test(test::shock_method, "shock method", n);
    run_test(test::bottom_up_method, "bottom up method", n);
    run_test(test::top_down_method, "top down method", n);

    return 0;
}