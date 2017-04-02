#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>

#include "ad/bottom_up.h"

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

int main()
{
    const double x = 107;
    const double k = 100;
    const double t = 0.5;
    const double s = 0.3;
    const double r = 0.1;

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

    const auto mgr 
        = ad::bu::create_variable_manager(x, k, t, s, r);
    const auto p
        = call_option(
            mgr.get_variable(x),
            mgr.get_variable(t),
            mgr.get_variable(k),
            mgr.get_variable(s),
            mgr.get_variable(r));

    std::cout
        << "price:" << price << "," << static_cast<double>(p) << std::endl
        << "delta:" << delta << "," << ad::bu::d(p).d(x) << std::endl
        << "vega:" << vega << "," << ad::bu::d(p).d(s) << std::endl
        << "theta:" << theta << "," << ad::bu::d(p).d(t) << std::endl;

    return 0;
}